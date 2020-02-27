#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include <math.h>



#define MASTER 0
#define N 100


float *create1DArray(int n) {
    float *T = (float *)malloc(n * sizeof(float));
    return T;
}


/*
* Problem: Paralel varyans hesaplama
*/
int main(void) {

    int rank, size, i;
    float mean, varience;

    MPI_Init(NULL, NULL);

    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int chunk = N / size;

    float local_toplam = 0.0;
    float toplam = 0.0;

    // Initialization
    float *dizi;
    float *local_dizi;

    if (rank == MASTER) {
        dizi = create1DArray(N);
        //memset(dizi, 1.0, N);
        
        for (i = 0; i < N; i++)
            dizi[i] = 1.0;
        

    }
    local_dizi = create1DArray(chunk);

    // Scatter (data distribution)
    MPI_Scatter(dizi, chunk, MPI_FLOAT, local_dizi, chunk, MPI_FLOAT, MASTER, MPI_COMM_WORLD);

    // Local computation
    for (i = 0; i < chunk; i++)
        local_toplam += local_dizi[i];
        
    // Gather data with reduction
    MPI_Reduce(&local_toplam, &toplam, 1, MPI_FLOAT, MPI_SUM, MASTER, MPI_COMM_WORLD);

    // MASTER prints out the average
    if (rank == MASTER)
    {
        printf("Ortalama = %f\n", toplam / N);
    }
    mean = (float)toplam / (float)N;
    
    MPI_Barrier(MPI_COMM_WORLD);
    local_toplam = 0;
    toplam = 0;
    for (i = 0; i < chunk; i++)
    {
        local_toplam += (local_dizi[i] - mean) * (local_dizi[i] - mean);
        printf("local_toplam = %f\n", mean);
    }

        
    MPI_Reduce(&local_toplam, &toplam, 1, MPI_FLOAT, MPI_SUM, MASTER, MPI_COMM_WORLD);

    
    
    if (rank == MASTER)
    {
        varience = toplam * (1 - (N - 1));
        printf("Varience = %f\n", varience);
        printf("Std      = %f\n", varience);
    }

    MPI_Finalize();

}