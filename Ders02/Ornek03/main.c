#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "math.h"

/***************************************************
* derleme:    mpicc main.c -o main.x
* calistirma: mpirun -np 4 ./a.out 345 22
*
* int MPI_Send( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm )
* int MPI_Recv( void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status )
*
* pi sayisini hesapla
* 
* 
*
**************************/

#define MASTER 0
#define PROCESS_0 0
#define PROCESS_1 1
#define PROCESS_2 2
#define PROCESS_3 3
#define TAG_1 777
#define TAG_2 778
#define TAG_3 779

#define PI (acos(-1))
#define N 1000000


/* 
 Serial case
    for (i = 0; i < N; i++)
    {
        toplam += f(i * h);
    }
    toplam *= h;
    printf("Gercek PI   : %.16f\n", PI);
    printf("Yaklasik PI : %.16f\n", toplam);
*/


double f(double x)
{
    return 4.0 / (1 + x * x);
}

int main(void)
{
    int i;
    double toplam = 0.0;
    double h = 1.0 / N;
    int rank, size;
    double pi = 0, local_pi = 0;
    MPI_Status *stat;
    
    MPI_Init(NULL, NULL);
    
    MPI_Comm_size(MPI_COMM_WORLD, &size); // toplam process sayisi
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    
    int chunk = N / size;

    for (i = rank * chunk + 1; i <= (rank + 1) * chunk; i++)
    {
        toplam += f(i * h);
        
    }
    toplam *= h;
    printf("Yaklasik PI : %.16f\n", toplam);


    if (rank != 0)
    {
        for (i = 1; i < 4; i++)
        {
            MPI_Send(&toplam, 1, MPI_DOUBLE, 0, TAG_1 + i, MPI_COMM_WORLD);
        }
    }
    else 
    {
        for (i = 1; i < 4; i++)
        {
            MPI_Recv(&local_pi, 1, MPI_DOUBLE, i, TAG_1 + 1, MPI_COMM_WORLD, stat);
            pi += local_pi;
        }
        
    }
    if (rank == 0)
    {
        pi += toplam;
        printf("pi: %f\n", pi);
    }

    MPI_Finalize();
    return 0;
}