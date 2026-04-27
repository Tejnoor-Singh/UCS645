#include <mpi.h>
#include <stdio.h>

int isPerfect(int n){
    int sum = 0;
    for(int i=1;i<=n/2;i++){
        if(n%i==0) sum+=i;
    }
    return sum==n;
}

int main(int argc, char *argv[]){
    int rank, size;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    int max = 10000;

    if(rank==0){
        int next = 2;
        int active = size-1;

        while(active>0){
            int msg;
            MPI_Status status;

            MPI_Recv(&msg,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);

            if(next<=max){
                MPI_Send(&next,1,MPI_INT,status.MPI_SOURCE,0,MPI_COMM_WORLD);
                next++;
            } else {
                int stop = -1;
                MPI_Send(&stop,1,MPI_INT,status.MPI_SOURCE,0,MPI_COMM_WORLD);
                active--;
            }
        }
    } else {
        int num=0;

        while(1){
            MPI_Send(&num,1,MPI_INT,0,0,MPI_COMM_WORLD);
            MPI_Recv(&num,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

            if(num==-1) break;

            if(isPerfect(num)){
                printf("Perfect: %d\n", num);
            }
        }
    }

    MPI_Finalize();
    return 0;
}
