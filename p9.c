#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define TAG 100

int main(int argc, char **argv)
{

    int rank, size;
    int sendData = 10;
    int recvData;

    MPI_Request req[2];
    MPI_Status stat[2];

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size != 2)
    {
        MPI_Finalize();
        return -1;
    }

    if (rank == 0)
    {
        MPI_Send(
            &sendData,
            1,
            MPI_INT,
            1,
            TAG,
            MPI_COMM_WORLD);

        printf("P1 Sent data %d\n ", sendData);

        MPI_Irecv(
            &recvData,
            1,
            MPI_INT,
            1,
            TAG,
            MPI_COMM_WORLD,
            &req[0]);
        printf("P1 start non blocking recv \n");

        MPI_Wait(&req[0], &stat[0]);
        printf("P1 receive data \n");
    }
    else if (rank == 1)
    {

        MPI_Isend(
            &sendData,
            1,
            MPI_INT,
            1,
            TAG,
            MPI_COMM_WORLD,
            &req[0]);
        printf("P2 start non blocking send\n");

        MPI_Recv(
            &recvData,
            1,
            MPI_INT,
            0,
            TAG,
            MPI_COMM_WORLD,
            &stat[1]);
        printf("P2 receive %d \n", recvData);

        MPI_Wait(&req[0], &stat[0]);
        printf("P2 finish sending data\n");
    }

    MPI_Finalize();
}