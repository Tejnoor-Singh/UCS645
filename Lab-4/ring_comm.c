#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int value;

    if (rank == 0)
        value = 100;

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    for (int step = 0; step < size; step++) {
        if (rank == step) {
            int next = (rank + 1) % size;
            printf("Process %d sending %d to process %d\n", rank, value, next);
            MPI_Send(&value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        }
        int prev = (rank - 1 + size) % size;
        if (rank == (step + 1) % size) {
            MPI_Recv(&value, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            value += rank;
            printf("Process %d received value %d\n", rank, value);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0)
        printf("Time: %f seconds\n", end - start);

    MPI_Finalize();
    return 0;
}
