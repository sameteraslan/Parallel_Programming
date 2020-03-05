#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include <math.h>
#include <time.h>

#define MASTER 0
#define N 3000000

float *create1DArray(int n) {
    float *T = (float *)malloc(n * sizeof(float));
    return T;
}

/*
* Problem: Computation of varience, standard deviation and average
*/
int main(void) {
    int rank, size, i, chunk;
    float mean, varience, local_total, global_total, *local_arr, *global_arr;
    double t1;
    srand(time(0));

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Initialization
    local_total = 0.0;
    global_total = 0.0;
    chunk = N / size;

    if (rank == MASTER) {
        global_arr = create1DArray(N);
        for (i = 0; i < N; i++)
            //*(global_arr + i) = 1.0;
            *(global_arr + i) = rand() % 10;
    }
    local_arr = create1DArray(chunk);

    t1 = MPI_Wtime();
    // Scatter (data distribution)
    MPI_Scatter(global_arr, chunk, MPI_FLOAT, local_arr, chunk, MPI_FLOAT, MASTER, MPI_COMM_WORLD);

    // Local computation
    for (i = 0; i < chunk; i++)
        local_total += *(local_arr + i);

    // Gather data with reduction
    MPI_Reduce(&local_total, &global_total, 1, MPI_FLOAT, MPI_SUM, MASTER, MPI_COMM_WORLD);

    // MASTER prints out the average
    if (rank == MASTER)
        mean = global_total / N;

    // Update value of mean with broadcast
    MPI_Bcast(&mean, 1, MPI_FLOAT, MASTER, MPI_COMM_WORLD);

    local_total = global_total = 0.0;
    for (i = 0; i < chunk; i++)
        local_total += (*(local_arr + i) - mean) * (*(local_arr + i) - mean);

    // Gather data with reduction
    MPI_Reduce(&local_total, &global_total, 1, MPI_FLOAT, MPI_SUM, MASTER, MPI_COMM_WORLD);

    if (rank == MASTER)
    {
        varience = fabs(global_total * (1 - (N - 1)));
        printf("Process Size = %d\n", size);
        printf("Array Size   = %d\n", 3000000);
        printf("Time         = %.8f\n", MPI_Wtime() - t1);
        printf("Mean         = %.8f\n", mean);
        printf("Varience     = %.8f\n", varience);
        printf("Std          = %.8f\n", sqrt(varience));
    }
    MPI_Finalize();
    return 0;
}
