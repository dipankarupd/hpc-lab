#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define SIZE 2000

// Function to take input for matrices
void initializeMatrices(int **a, int **b)
{
    omp_set_num_threads(64);
    srand(time(NULL));
#pragma omp parallel for
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            a[i][j] = rand() % 100 + 1;
            b[i][j] = rand() % 100 + 1;
        }
    }
}

void multiplySeq(int **a, int **b, int **res)
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            res[i][j] = 0;
            for (int k = 0; k < SIZE; k++)
            {
                res[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void multiplyParallel(int **a, int **b, int **res)
{
    omp_set_num_threads(64);
#pragma omp parallel for
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            res[i][j] = 0;
            for (int k = 0; k < SIZE; k++)
            {
                res[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int main()
{
    int **a = (int **)malloc(SIZE * sizeof(int *));
    int **b = (int **)malloc(SIZE * sizeof(int *));
    int **res = (int **)malloc(SIZE * sizeof(int *));

    for (int i = 0; i < SIZE; i++)
    {
        a[i] = (int *)malloc(SIZE * sizeof(int));
        b[i] = (int *)malloc(SIZE * sizeof(int));
        res[i] = (int *)malloc(SIZE * sizeof(int));
    }

    // Initialize matrices
    initializeMatrices(a, b);

    double start, end;

    start = clock();
    multiplyParallel(a, b, res);
    end = clock();
    printf("The time taken for parallel is: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();
    multiplySeq(a, b, res);
    end = clock();
    printf("The time taken for sequential is: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

    for (int i = 0; i < SIZE; i++)
    {
        free(a[i]);
        free(b[i]);
        free(res[i]);
    }
    free(a);
    free(b);
    free(res);

    return 0;
}
