#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void main()
{
    int n = 7;
    double cgpa[n];

    srand(time(NULL));

    for (int i = 0; i < n; i++)
    {
        cgpa[i] = ((double)rand() / RAND_MAX) * 10.0;
    }

    for (int i = 0; i < n; i++)
    {
        printf("%.2f ", cgpa[i]);
    }
    printf("\n");

    double maxCgpa = 0.0;

#pragma omp parallel reduction(max : maxCgpa)
    {
        for (int i = 0; i < n; i++)
        {
            /* code */
            if (cgpa[i] > maxCgpa)
            {
                /* code */
                maxCgpa = cgpa[i];
            }
        }
    }

    printf("Max cgpa is: %.2f \n", maxCgpa);
}