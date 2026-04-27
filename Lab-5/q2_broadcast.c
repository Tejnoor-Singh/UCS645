#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10000000

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    double *arr = (double*)malloc(N*sizeof(double));

    if(rank==0){
        for(int i=0;i<N;i++) arr[i]=i;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    double start = MPI_Wtime();

    // MyBcast
    if(rank==0){
        for(int i=1;i<size;i++){
            MPI_Send(arr,N,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(arr,N,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }

    double end = MPI_Wtime();

    if(rank==0) printf("MyBcast Time: %f\n", end-start);

    MPI_Barrier(MPI_COMM_WORLD);

    start = MPI_Wtime();

    MPI_Bcast(arr,N,MPI_DOUBLE,0,MPI_COMM_WORLD);

    end = MPI_Wtime();

    if(rank==0) printf("MPI_Bcast Time: %f\n", end-start);

    free(arr);
    MPI_Finalize();
    return 0;
}
