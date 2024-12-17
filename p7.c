#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    float recv;
    float send[100];

    if (rank == 0)
    {
        printf("Before scatter: \n");
        for (int i = 0; i < size; i++)
        {
            srand(i);
            send[i] = (float)(rand() % 100 + 1) / 10;
            printf("%.1f ", send[i]);
        }
    }

    MPI_Scatter(
        send,
        1,
        MPI_FLOAT,
        &recv,
        1,
        MPI_FLOAT,
        0,
        MPI_COMM_WORLD);

    printf("Rank %d received %.1f \n", rank, recv);

    MPI_Finalize();
}