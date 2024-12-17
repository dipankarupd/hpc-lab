
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

double calculatePiSerial(int num_steps);
double calculatePiParallel(int num_steps);

int main()
{
    int num_steps;
    printf("Enter the number of steps: ");
    scanf("%d", &num_steps);

    // Run and time the serial version
    clock_t start_serial = clock();
    double pi_serial = calculatePiSerial(num_steps);
    clock_t end_serial = clock();
    double time_serial = (double)(end_serial - start_serial) / CLOCKS_PER_SEC;

    // Run and time the parallel version
    double start_parallel = omp_get_wtime();
    double pi_parallel = calculatePiParallel(num_steps);
    double end_parallel = omp_get_wtime();
    double time_parallel = end_parallel - start_parallel;

    // Print results
    printf("\nSerial Execution:\n");
    printf("Calculated Pi = %.16lf\n", pi_serial);
    printf("Time Taken = %lf seconds\n", time_serial);

    printf("\nParallel Execution:\n");
    printf("Calculated Pi = %.16lf\n", pi_parallel);
    printf("Time Taken = %lf seconds\n", time_parallel);

    return 0;
}

// Serial calculation of Pi
double calculatePiSerial(int num_steps)
{
    double step = 1.0 / (double)num_steps;
    double pi = 0.0;

    for (int i = 0; i < num_steps; i++)
    {
        double x = (i + 0.5) * step;
        pi += (4.0 / (1.0 + x * x)) * step;
    }

    return pi;
}

// Parallel calculation of Pi using OpenMP
double calculatePiParallel(int num_steps)
{
    double step = 1.0 / (double)num_steps;
    double pi = 0.0;

#pragma omp parallel for reduction(+ : pi)
    for (int i = 0; i < num_steps; i++)
    {
        double x = (i + 0.5) * step;
        pi += (4.0 / (1.0 + x * x)) * step;
    }

    return pi;
}
