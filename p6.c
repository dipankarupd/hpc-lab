///
/// Assume you have n robots which pick mangoes in a farm.
/// Write a program to calculatethe total number of
/// mangoes picked by n robots parallelly using MPI.

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{

    int rank, size, mangoesPicked, totalMangoes;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(time(NULL) + rank);

    mangoesPicked = rand() % 50 + 1;
    printf("Robot %d picked %d mangoes\n", rank, mangoesPicked);

    MPI_Reduce(
        &mangoesPicked, // value of individual process
        &totalMangoes,  // val to add at the end
        1,              // no of elem whose val we are printing
        MPI_INT,        // data type
        MPI_SUM,        // operation
        0,              // root process
        MPI_COMM_WORLD  // commn channel
    );

    if (rank == 0)
    {
        printf("Total mangoes picked = %d\n", totalMangoes);
    }

    MPI_Finalize();
    return 0;
}