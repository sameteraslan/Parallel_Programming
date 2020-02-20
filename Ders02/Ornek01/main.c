#include <stdio.h>
#include "mpi.h"


/***************************************************
* derleme:    mpicc main.c -o main.x
* calistirma: mpirun -np 4 ./a.out 345 22
****************************************************/
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_size(MPI_COMM_WORLD, &size); // toplam process sayisi
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 

    if (rank == 0)
    {
        printf("arg count: %d\n", argc);
        printf("argv[0]: %s\n", argv[0]);
        printf("argv[1]: %s\n", argv[1]);
        printf("argv[2]: %s\n", argv[2]);
    }
    printf("%s size: %d  rank: %d\n", "Merhaba Dunya", size, rank);
    printf("\n");
    //Herkes en son processin gelmesini beklesin
    //sonra hep beraber devam etsin
    /*
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 1 || rank == 2)
    {
        printf("%s size: %d  rank: %d\n", "Merhaba Dunya", size, rank);
    }
    */
    MPI_Finalize();
    return 0;
}