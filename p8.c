// #include <stdio.h>
// #include <mpi.h>

// int main(int argc, char **argv)
// {
//     MPI_Init(&argc, &argv);

//     int rank, size;
//     MPI_Comm_size(MPI_COMM_WORLD, &size);
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);

//     int dims[2] = {2, 2}, coords[2], north, south, east, west;
//     int periods[2] = {0, 0};
//     MPI_Comm cart_comm;

//     // Ensure that the number of processes is divisible by the number of dimensions
//     if (size != 4)
//     {
//         MPI_Finalize();
//         return -1;
//     }

//     MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart_comm);
//     MPI_Cart_coords(cart_comm, rank, 2, coords);

//     MPI_Cart_shift(cart_comm, 0, 1, &north, &south);
//     MPI_Cart_shift(cart_comm, 1, 1, &west, &east);

//     printf("P%d has coords: %d %d and neighbours: %d %d %d %d\n", rank, coords[0], coords[1], north, south, east, west);

//     MPI_Finalize();
//     return 0;
// }

#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, numtasks;
    MPI_Comm cart_comm;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Dynamically calculate grid dimensions based on numtasks (assuming a square grid)
    int dims[2] = {0, 0}; // Allow MPI to decide the best dimensions
    MPI_Dims_create(numtasks, 2, dims);

    // Ensure grid is non-periodic
    int periods[2] = {0, 0};
    int coords[2], nbrs[4]; // Neighbors: UP, DOWN, LEFT, RIGHT

    // Create a Cartesian communicator
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart_comm);
    MPI_Cart_coords(cart_comm, rank, 2, coords);

    // Get neighbors in all directions
    MPI_Cart_shift(cart_comm, 0, 1, &nbrs[0], &nbrs[1]); // UP, DOWN
    MPI_Cart_shift(cart_comm, 1, 1, &nbrs[2], &nbrs[3]); // LEFT, RIGHT

    printf("Rank %d at coords (%d, %d) has neighbors: UP=%d, DOWN=%d, LEFT=%d, RIGHT=%d\n",
           rank, coords[0], coords[1], nbrs[0], nbrs[1], nbrs[2], nbrs[3]);

    int outbuf = 15, inbuf[4] = {-1, -1, -1, -1}; // Send 15 to all neighbors
    MPI_Request reqs[8];

    // Initiate non-blocking sends and receives
    for (int i = 0; i < 4; i++)
    {
        if (nbrs[i] != MPI_PROC_NULL)
        {
            MPI_Isend(&outbuf, 1, MPI_INT, nbrs[i], 0, cart_comm, &reqs[i]);
            MPI_Irecv(&inbuf[i], 1, MPI_INT, nbrs[i], 0, cart_comm, &reqs[i + 4]);
        }
        else
        {
            reqs[i] = MPI_REQUEST_NULL;
            reqs[i + 4] = MPI_REQUEST_NULL;
        }
    }
    MPI_Waitall(8, reqs, MPI_STATUSES_IGNORE);
    printf("Rank %d received data from neighbors: UP=%d, DOWN=%d, LEFT=%d, RIGHT=%d\n",
           rank, inbuf[0], inbuf[1], inbuf[2], inbuf[3]);
    MPI_Finalize();
    return 0;
}
