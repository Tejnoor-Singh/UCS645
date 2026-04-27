#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 65536

int main(int argc, char *argv[]) {
    int rank, size;
    double a = 2.5;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunk = N / size;

    double *X = (double*)malloc(chunk * sizeof(double));
    double *Y = (double*)malloc(chunk * sizeof(double));

    for(int i=0;i<chunk;i++){
        X[i] = 1.0;
        Y[i] = 2.0;
    }

    double start = MPI_Wtime();

    for(int i=0;i<chunk;i++){
        X[i] = a * X[i] + Y[i];
    }

    double end = MPI_Wtime();

    if(rank==0){
        printf("MPI Time: %f\n", end-start);
    }

    free(X);
    free(Y);
    MPI_Finalize();
    return 0;
}
