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
    for (i = 0; i < n; i++)
    {
        T[i] = 1.0;
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

float innerProduct(float *u, float *v, int n)
{
    int i;
    float sum = 0.0;
    for (i = 0; i < n; i++)
    {
        sum += u[i] * v[i];
    }
    return sum;
}

float *matrix_vector_multiplication(float *M, float *v, int n1, int n2)
{
    int i;
    float *r = create1DArray(n1);
    for (i = 0; i < n1; i++)
    {
        r[i] = innerProduct(&M[i * n2], v, n2);
    }
    return r;
}

/*
* derleme:    mpicc main.c -o out
* calistirma: mpirun -np 4 ./out
* Problem: Parallel A X = b
*/
int main(int argc, char *argv[])
{
    int n1 = atoi(argv[1]);
    int n2 = atoi(argv[2]);
    int rank, size, i, chunk;
    float *A, *x, *b, *A_local, *b_local;
    double t_start, t_end;

    MPI_Init(NULL, NULL);

    t_start = MPI_Wtime();

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    chunk = n1 / size;
    x = create1DArray(n2);
    A_local = create1DArray(chunk * n2);
    b_local = create1DArray(chunk);

    if (rank == MASTER)
    {
      A = create1DArray(n1 * n2);
      fillArray(A, n1 * n2);
      fillArray(x, n2);
      b = create1DArray(n1);
    }

    // Broadcast x vector to all processes
    MPI_Bcast(x, n2, MPI_FLOAT, MASTER, MPI_COMM_WORLD);

    // Scatter A to each process
    MPI_Scatter(A, chunk * n2, MPI_FLOAT, A_local, chunk * n2, MPI_FLOAT, MASTER, MPI_COMM_WORLD);

    // Local computation
    b_local = matrix_vector_multiplication(A_local, x, chunk, n2);

    // Gather b_local's into MASTER's b
    MPI_Gather(b_local, chunk, MPI_FLOAT, b, chunk, MPI_FLOAT, MASTER, MPI_COMM_WORLD);

    t_end = MPI_Wtime();

    if (rank == MASTER)
    {
      //printArray(b, n1);
      printf("Time Elapsed: %f\n", t_end - t_start);
    }

    MPI_Finalize();

    return 0;
}
