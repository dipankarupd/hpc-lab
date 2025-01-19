// #include <stdio.h>
// #include <stdlib.h>
// #include <mpi.h>

// #define TAG 100

// int main(int argc, char **argv)
// {

//     int rank, size;
//     int sendData = 10;
//     int recvData;

//     MPI_Request req[2];
//     MPI_Status stat[2];

//     MPI_Init(&argc, &argv);

//     MPI_Comm_size(MPI_COMM_WORLD, &size);
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);

//     if (size != 2)
//     {
//         MPI_Finalize();
//         return -1;
//     }

//     if (rank == 0)
//     {
//         MPI_Send(
//             &sendData,
//             1,
//             MPI_INT,
//             1,
//             TAG,
//             MPI_COMM_WORLD);

//         printf("P1 Sent data %d\n ", sendData);

//         MPI_Irecv(
//             &recvData,
//             1,
//             MPI_INT,
//             1,
//             TAG,
//             MPI_COMM_WORLD,
//             &req[0]);
//         printf("P1 start non blocking recv \n");

//         MPI_Wait(&req[0], &stat[0]);
//         printf("P1 receive data \n");
//     }
//     else if (rank == 1)
//     {

//         MPI_Isend(
//             &sendData,
//             1,
//             MPI_INT,
//             1,
//             TAG,
//             MPI_COMM_WORLD,
//             &req[0]);
//         printf("P2 start non blocking send\n");

//         MPI_Recv(
//             &recvData,
//             1,
//             MPI_INT,
//             0,
//             TAG,
//             MPI_COMM_WORLD,
//             &stat[1]);
//         printf("P2 receive %d \n", recvData);

//         MPI_Wait(&req[0], &stat[0]);
//         printf("P2 finish sending data\n");
//     }

//     MPI_Finalize();
// }

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void blocking_communication(int rank, int numtasks);
void nonblocking_communication(int rank, int numtasks);

int main(int argc, char *argv[])
{
    int numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Perform blocking communication
    blocking_communication(rank, numtasks);

    // Synchronize before non-blocking communication
    MPI_Barrier(MPI_COMM_WORLD);

    // Perform non-blocking communication
    nonblocking_communication(rank, numtasks);

    MPI_Finalize();
    return 0;
}

void blocking_communication(int rank, int numtasks)
{
    int rc, inmsg, next, prev;
    double st_blocking, et_blocking;
    MPI_Status Stat;

    st_blocking = MPI_Wtime(); // Start time for blocking communication

    // Determine next and previous ranks for circular communication
    if (rank == 0)
        prev = numtasks - 1;
    else
        prev = rank - 1;

    if (rank == numtasks - 1)
        next = 0;
    else
        next = rank + 1;

    if (rank % 2 == 0)
    {
        // Even ranks send and receive
        printf("Rank %d (Blocking): Sending message '%d' to Rank %d\n", rank, rank, next);
        rc = MPI_Send(&rank, 1, MPI_INT, next, 0, MPI_COMM_WORLD);

        rc = MPI_Recv(&inmsg, 1, MPI_INT, prev, 1, MPI_COMM_WORLD, &Stat);
        printf("Rank %d (Blocking): Received message '%d' from Rank %d\n", rank, inmsg, prev);
    }
    else
    {
        // Odd ranks receive and send
        rc = MPI_Recv(&inmsg, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &Stat);
        printf("Rank %d (Blocking): Received message '%d' from Rank %d\n", rank, inmsg, prev);

        printf("Rank %d (Blocking): Sending message '%d' to Rank %d\n", rank, rank, next);
        rc = MPI_Send(&rank, 1, MPI_INT, next, 1, MPI_COMM_WORLD);
    }

    et_blocking = MPI_Wtime(); // End time for blocking communication

    // Synchronize all processes before printing the time
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
    {
        // Print the time for blocking communication
        printf("\nTime taken by Blocking send/receive: %lf seconds\n", et_blocking - st_blocking);
    }
}

void nonblocking_communication(int rank, int numtasks)
{
    int inmsg, next, prev;
    double st_nonblocking, et_nonblocking;
    MPI_Request reqs[2];

    st_nonblocking = MPI_Wtime(); // Start time for non-blocking communication

    // Initialize next and previous ranks for circular communication
    if (rank == 0)
        prev = numtasks - 1;
    else
        prev = rank - 1;

    if (rank == numtasks - 1)
        next = 0;
    else
        next = rank + 1;

    // Non-blocking operations
    printf("Rank %d (Non-blocking): Non-blocking receive from Rank %d\n", rank, prev);
    MPI_Irecv(&inmsg, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &reqs[0]);

    printf("Rank %d (Non-blocking): Non-blocking send to Rank %d\n", rank, next);
    MPI_Isend(&rank, 1, MPI_INT, next, 0, MPI_COMM_WORLD, &reqs[1]);

    // Wait for the non-blocking operations to complete
    MPI_Wait(&reqs[0], MPI_STATUS_IGNORE);
    MPI_Wait(&reqs[1], MPI_STATUS_IGNORE);

    // printf("Rank %d (Non-blocking): Received message '%d' from Rank %d\n", rank, inmsg, prev);

    et_nonblocking = MPI_Wtime(); // End time for non-blocking communication

    // Synchronize all processes before printing the time
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
    {
        // Print the time for non-blocking communication
        printf("Time taken by Non-blocking send/receive: %lf seconds\n", et_nonblocking - st_nonblocking);
    }
}
