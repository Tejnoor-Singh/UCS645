#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 100;
    int data[N];
    int chunk = N / size;
    int local[chunk];

    if (rank == 0) {
        for (int i = 0; i < N; i++)
            data[i] = i + 1;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    MPI_Scatter(data, chunk, MPI_INT, local, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    int local_sum = 0;
    for (int i = 0; i < chunk; i++)
        local_sum += local[i];

    int global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0) {
        printf("Global Sum = %d  Expected = 5050\n", global_sum);
        printf("Time: %f seconds\n", end - start);
    }

    MPI_Finalize();
    return 0;
}
