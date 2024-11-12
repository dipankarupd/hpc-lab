#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void main()
{
    int n;
    printf("Enter the number of elements you want in series: ");
    scanf("%d", &n);

    int arr[n];
    arr[0] = 0;
    arr[1] = 1;

    // set 2 threads

    omp_set_num_threads(2);

// parallel execution:
#pragma omp parallel
    {

#pragma omp critical
        {
            if (omp_get_thread_num() == 0)
            {
                printf("Generating fib series:\n");
                for (int i = 2; i < n; i++)
                {
                    arr[i] = arr[i - 2] + arr[i - 1];
                }
            }
        }

#pragma omp critical
        {
            if (omp_get_thread_num() == 1)
            {
                printf("Printing fib series:\n");
                for (int i = 0; i < n; i++)
                {
                    printf("%d ", arr[i]);
                }
            }
        }
    }
}