#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include <math.h>

#define MASTER 0

float *create1DArray(int n)
{
    float *T = (float *)malloc(n * sizeof(float));
    return T;
}

void fillArray(float *T, int n)
{
    int i;
    for (i = 1; i <= n; i++)
    {
        T[i] = i; //1.0;
    }
}

void printArray(float *T, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%.2f ", T[i]);
    }
    puts("");
}

/*
* derleme:    mpicc main.c -o out
* calistirma: mpirun -np 4 ./out
* Problem: --
*/
int main(int argc, char *argv[])
{
    int n1 = atoi(argv[1]);
    int n2 = atoi(argv[2]);
    int rank, size, i, chunk, c_chunk;
    float *A, *r, *c;

    MPI_Init(NULL, NULL);


    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status stat;

    if (rank == MASTER)
    {
      A = create1DArray(n1 * n2);
      fillArray(A, n1 * n2);
    }

    chunk = n1 / size;
    r = create1DArray(n2);
    r2 = create1DArray(chunk * n2);

    MPI_Datatype colType;
    int block_length = 1, stride = n2, count = n1;
    MPI_Type_vector(count, block_length, stride, MPI_FLOAT, &colType);
    MPI_Type_commit(&colType);

    //P2P Communication
    if (rank == MASTER)
    {
        MPI_Send(&A[1], 1, colType, MASTER + 1, 777, MPI_COMM_WORLD);
    }
    else if (rank == MASTER + 1)
    {
        MPI_Recv(r, n2, MPI_FLOAT, MASTER, 666, MPI_COMM_WORLD, &stat);
    }
    else if (rank == MASTER + 2)
    {
        MPI_Recv(r, n2, MPI_FLOAT, MASTER, 777, MPI_COMM_WORLD, &stat);
    }

    /*
    if (rank == MASTER + 2)
    {
        printArray(r, n2);
    }
    */

    // Collective communication using rowType
    //MPI_Scatter(A, chunk, rowType, r2, chunk * n2, MPI_FLOAT, MASTER, MPI_COMM_WORLD);
    MPI_Scatter(A, chunk, rowType, r2, chunk * n2, rowType, MASTER, MPI_COMM_WORLD);

    if (rank == 2)
    {
      printArray(r2, n2 * chunk);
    }


    MPI_Finalize();

    return 0;
}
