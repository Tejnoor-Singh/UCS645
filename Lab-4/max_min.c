#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) + rank);

    int local_values[10];
    for (int i = 0; i < 10; i++)
        local_values[i] = rand() % 1000;

    int local_max = -1, local_min = 10000;
    for (int i = 0; i < 10; i++) {
        if (local_values[i] > local_max) local_max = local_values[i];
        if (local_values[i] < local_min) local_min = local_values[i];
    }

    struct { int value; int rank; } in_max, out_max, in_min, out_min;

    in_max.value = local_max;
    in_max.rank  = rank;

    in_min.value = local_min;
    in_min.rank  = rank;

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    MPI_Reduce(&in_max, &out_max, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
    MPI_Reduce(&in_min, &out_min, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0) {
        printf("Global Max = %d from process %d\n", out_max.value, out_max.rank);
        printf("Global Min = %d from process %d\n", out_min.value, out_min.rank);
        printf("Time: %f seconds\n", end - start);
    }

    MPI_Finalize();
    return 0;
}
