#include <stdio.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char **argv)
{

    MPI_Init(&argc, &argv);

    int rank, size;
    int coords[2], dims[2] = {2, 2}, periods[2] = {0, 0};

    MPI_Comm cart_comm;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4)
    {
        MPI_Finalize();
        return 1;
    }
    /// int MPI_Cart_create(MPI_Comm comm_old, int ndims, int dims[], int periods[], int reorder, MPI_Comm *comm_cart);
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart_comm);

    MPI_Cart_coords(cart_comm, rank, 2, coords);

    printf("Process %d have coordinates %d %d \n", rank, coords[0], coords[1]);

    MPI_Finalize();

    return 0;
}