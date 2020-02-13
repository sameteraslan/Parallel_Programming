#include <stdio.h>
#include "mpi.h"

int main(void)
{
    MPI_Init(NULL, NULL);

    int rank, size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("%s size: %d  rank: %d\n", "Merhaba Dunya", size, rank);
    printf("\n");
    //Herkes en son processin gelmesini beklesin
    //sonra hep beraber devam etsin
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 1 || rank == 2)
    {
        printf("%s size: %d  rank: %d\n", "Merhaba Dunya", size, rank);
    }
    MPI_Finalize();
    return 0;
}