#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 500000000

int main(int argc, char *argv[]) {
    int rank, size;
    double multiplier;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(rank==0){
        multiplier = 2.0;
    }

    MPI_Bcast(&multiplier,1,MPI_DOUBLE,0,MPI_COMM_WORLD);

    long chunk = N / size;

    double local_sum = 0;

    double start = MPI_Wtime();

    for(long i=0;i<chunk;i++){
        double A = 1.0;
        double B = 2.0 * multiplier;
        local_sum += A * B;
    }

    double global_sum;

    MPI_Reduce(&local_sum,&global_sum,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if(rank==0){
        printf("Dot Product: %f\n", global_sum);
        printf("Time: %f\n", end-start);
    }

    MPI_Finalize();
    return 0;
}
