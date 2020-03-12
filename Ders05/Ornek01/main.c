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
    char   ad[5];
    int    d_tarih[3];
    double boy_kilo[2]; 
    int rank, size, i, chunk, c_chunk;
    int position = 0;
    char buffer[sizeof(ad) + sizeof(d_tarih) + sizeof(boy_kilo)];

    MPI_Init(NULL, NULL);
    MPI_Comm_size( MPI_COMM_WORLD, &size);
    MPI_Comm_rank( MPI_COMM_WORLD, &rank);
    MPI_Status stat;

    if (rank == MASTER)
    {
        strcpy(ad, "samet");
        d_tarih[0] = 7;
        d_tarih[1] = 11;
        d_tarih[2] = 1997;

        boy_kilo[0] = 180;
        boy_kilo[1] = 75;

        MPI_Pack(ad, sizeof(ad), MPI_CHAR, buffer, sizeof(buffer), &position, MPI_COMM_WORLD);
        MPI_Pack(d_tarih, sizeof(d_tarih), MPI_CHAR, buffer, sizeof(buffer), &position, MPI_COMM_WORLD);
        MPI_Pack(boy_kilo, sizeof(boy_kilo), MPI_CHAR, buffer, sizeof(buffer), &position, MPI_COMM_WORLD);
        MPI_Send(buffer, position, MPI_PACKED, 1, 666, MPI_COMM_WORLD);
    }


    if (rank == 1)
    {
        MPI_Recv(buffer, sizeof(buffer), MPI_PACKED, 0, 666, MPI_COMM_WORLD, &stat);
        MPI_Unpack(buffer, sizeof(buffer), &position, ad, sizeof(ad), MPI_CHAR, MPI_COMM_WORLD);
        MPI_Unpack(buffer, sizeof(buffer), &position, d_tarih, sizeof(d_tarih), MPI_CHAR, MPI_COMM_WORLD);
        MPI_Unpack(buffer, sizeof(buffer), &position, boy_kilo, sizeof(boy_kilo), MPI_CHAR, MPI_COMM_WORLD);

        printf("ad: %s  - d_tarih: %d %d %d   -  boy_kilo:  %f %f \n", ad, d_tarih[0], d_tarih[1], d_tarih[2], boy_kilo[0], boy_kilo[1]);
    }


    MPI_Finalize();

    return 0;
}
