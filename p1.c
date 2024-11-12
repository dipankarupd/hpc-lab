/*
Given a nxn matrix A and a vector x of length n, their product y=A-x,
write a program to implement the multiplication using OpenMP PARALLEL directive.
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
void getInput(int n, int a[n][n], int x[n])
{
    printf("Generating random values for a and x: \n");
    for (int i = 0; i < n; i++)
    {
        x[i] = rand() % 100;
        for (int j = 0; j < n; j++)
        {
            a[i][j] = rand() % 100;
        }
    }
    printf("Matrix a: \n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }

    printf("vector x: \n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\n", x[i]);
    }
}

void matrixMultiplicationSerial(int n, int a[n][n], int x[n], int res[n])
{
    printf("Performing the multiplication: \n");
    for (int i = 0; i < n; i++)
    {
        res[i] = 0;
        for (int j = 0; j < n; j++)
        {
            res[i] += a[i][j] * x[j];
        }
    }
}

void matrixMultiplicationParallel(int n, int a[n][n], int x[n], int res[n])
{
    omp_set_num_threads(n);
    printf("Performing the multiplication parallely: \n");

#pragma omp parallel for
    for (int i = 0; i < n; i++)
    {
        res[i] = 0;
        for (int j = 0; j < n; j++)
        {
            res[i] += a[i][j] * x[j];
        }
    }
}

void printResult(int n, int res[n])
{
    printf("Printing the result: \n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\n", res[i]);
    }
}

void main()
{

    int n;
    printf("Enter the value of n: ");
    scanf("%d", &n);

    // create a n*n matrix:
    int a[n][n];
    int x[n];
    int res[n];

    // populate the matrices:
    getInput(n, a, x);

    // perform the multiplication:
    double startP = clock();
    matrixMultiplicationParallel(n, a, x, res);
    double endP = clock();

    double startS = clock();
    matrixMultiplicationSerial(n, a, x, res);
    double endS = clock();
    // get the result
    printResult(n, res);

    // print the time for execution:
    printf("Time taken for serial execution: %f", (double)(endS - startS) / CLOCKS_PER_SEC);
    printf("Time taken for Parallel execution: %f", (double)(endP - startP) / CLOCKS_PER_SEC);
}