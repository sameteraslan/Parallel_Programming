#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/***************************************************
* derleme:    mpicc main.c -o main.x
* calistirma: mpirun -np 4 ./a.out 345 22
*
* int MPI_Send( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm )
* int MPI_Recv( void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status )
*
* 60 elemanli bir dizi olsun.
* 15 er elemanli alt dizi acip elemanlari
* sifirlayan ve
*
**************************/

#define MASTER 0
#define N 60
#define CHUNK (N / 4)
#define PROCESS_0 0
#define PROCESS_1 1
#define PROCESS_2 2
#define PROCESS_3 3
#define TAG_1 777
#define TAG_2 778
#define TAG_3 779


void printArray(int *d, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d\t", d[i]);
    }
    printf("\n");
}

int main(void)
{
    MPI_Init(NULL, NULL);
    
    MPI_Status *stat;

    int rank, size;

    MPI_Comm_size(MPI_COMM_WORLD, &size); // toplam process sayisi
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 

    int alt_dizi[CHUNK] = {0};

    if (rank == MASTER)
    {
        int *dizi = malloc(N * sizeof(int));
        for (int i = 0; i < N; i++)
        {
            dizi[i] = i + 1;
        }
        MPI_Send(&dizi[15], CHUNK, MPI_INT, PROCESS_1, TAG_1, MPI_COMM_WORLD);
        MPI_Send(&dizi[30], CHUNK, MPI_INT, PROCESS_2, TAG_2, MPI_COMM_WORLD);
        MPI_Send(&dizi[45], CHUNK, MPI_INT, PROCESS_3, TAG_3, MPI_COMM_WORLD);
    }
    //int *alt_dizi = malloc(15 * sizeof(int));
    
    if (rank == 1)
    {
        MPI_Recv(alt_dizi, CHUNK, MPI_INT, PROCESS_0, TAG_1, MPI_COMM_WORLD, stat);
        printArray(alt_dizi, CHUNK);
    }
    else if (rank == 2)
    {
        MPI_Recv(alt_dizi, CHUNK, MPI_INT, PROCESS_0, TAG_2, MPI_COMM_WORLD, stat);
        printArray(alt_dizi, CHUNK);
    }
    else if (rank == 3)
    {
        MPI_Recv(alt_dizi, CHUNK, MPI_INT, PROCESS_0, TAG_3, MPI_COMM_WORLD, stat);
        printArray(alt_dizi, CHUNK);
    }
    
    

    MPI_Finalize();
    return 0;
}