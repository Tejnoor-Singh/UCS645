#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int A[8] = {1,2,3,4,5,6,7,8};
    int B[8] = {8,7,6,5,4,3,2,1};

    int chunk = 8 / size;
    int localA[chunk], localB[chunk];

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    MPI_Scatter(A, chunk, MPI_INT, localA, chunk, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, chunk, MPI_INT, localB, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    int local_dot = 0;
    for (int i = 0; i < chunk; i++)
        local_dot += localA[i] * localB[i];

    int global_dot = 0;
    MPI_Reduce(&local_dot, &global_dot, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0) {
        printf("Dot Product = %d (Expected = 120)\n", global_dot);
        printf("Time: %f seconds\n", end - start);
    }

    MPI_Finalize();
    return 0;
}
