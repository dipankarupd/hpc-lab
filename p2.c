
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

typedef struct Section
{
    int id;
    int itemCount;
    int *cost;
    double sectionCost; // Changed to double for consistency
} Section;

void sectionInput(Section section[], int n)
{
    for (int i = 0; i < n; i++)
    {
        section[i].id = i;
        section[i].itemCount = rand() % 10;

        // Allocate memory for costs
        section[i].cost = (int *)malloc(section[i].itemCount * sizeof(int));

        for (int j = 0; j < section[i].itemCount; j++)
        {
            section[i].cost[j] = rand() % 100;
        }
    }
}

void calculate(Section sec[], double *finalCost, int n)
{
    *finalCost = 0; // Initialize finalCost to 0

#pragma omp parallel
    {
        double localTotal = 0; // Local variable for each thread's total

#pragma omp for
        for (int i = 0; i < n; i++)
        {
            sec[i].sectionCost = 0; // Initialize each section's cost

            for (int j = 0; j < sec[i].itemCount; j++)
            {
                sec[i].sectionCost += sec[i].cost[j]; // Accumulate section cost
            }

            localTotal += sec[i].sectionCost; // Accumulate to local total here
        }

// Combine local totals into finalCost
#pragma omp atomic
        *finalCost += localTotal; // Update finalCost atomically
    }

    // Display each section:
    for (int i = 0; i < n; i++)
    {
        printf("---------------------------------------------------\n");
        printf("Section id: %d\n", sec[i].id);
        printf("Section item count: %d\n", sec[i].itemCount);
        printf("Section item costs: ");
        for (int j = 0; j < sec[i].itemCount; j++)
        {
            printf("%d ", sec[i].cost[j]);
        }
        printf("\nSection total: %f\n", sec[i].sectionCost);
        printf("---------------------------------------------------\n");
    }
}

int main()
{
    srand(time(NULL)); // Seed the random number generator

    int n;
    double finalBill = 0; // Initialize finalBill to 0
    printf("Enter the number of sections: \n");
    scanf("%d", &n);

    Section section[n];

    sectionInput(section, n);

    calculate(section, &finalBill, n);

    printf("The total cost is: %f\n", finalBill);

    // Free allocated memory
    for (int i = 0; i < n; i++)
    {
        free(section[i].cost);
    }

    return 0; // Added return statement
}