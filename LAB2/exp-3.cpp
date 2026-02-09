#include <iostream>
#include <vector>
#include <omp.h>
#include <iomanip>

using namespace std;

// Run heat diffusion with given threads
double run_heat_simulation(int N, int steps, int threads, double &total_heat) {

    vector<vector<double>> grid(N, vector<double>(N, 0.0));
    vector<vector<double>> new_grid(N, vector<double>(N, 0.0));

    // Initialize heat source in center
    grid[N/2][N/2] = 1000.0;

    double start = omp_get_wtime();

    for (int t = 0; t < steps; t++) {

        #pragma omp parallel for num_threads(threads) schedule(static)
        for (int i = 1; i < N-1; i++) {
            for (int j = 1; j < N-1; j++) {

                new_grid[i][j] =
                    0.25 * (grid[i+1][j] +
                            grid[i-1][j] +
                            grid[i][j+1] +
                            grid[i][j-1]);
            }
        }

        // Swap grids
        swap(grid, new_grid);
    }

    // Compute total heat (reduction)
    total_heat = 0.0;

    #pragma omp parallel for num_threads(threads) reduction(+:total_heat)
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            total_heat += grid[i][j];

    double end = omp_get_wtime();

    return end - start;
}

int main() {

    int N = 2000;      // Grid size (2000x2000)
    int steps = 500;   // Time steps

    cout << "2D Heat Diffusion Parallel Scaling\n";
    cout << "Grid: " << N << " x " << N << ", Steps: " << steps << "\n\n";

    double total_heat;

    double T1 = run_heat_simulation(N, steps, 1, total_heat);

    cout << "Threads   Time(s)   Speedup   Efficiency\n";
    cout << "------------------------------------------\n";

    cout << fixed << setprecision(6);

    cout << 1 << "         "
         << T1 << "      "
         << 1.0 << "      "
         << 1.0 << endl;

    for (int t = 2; t <= 8; t *= 2) {

        double Tp = run_heat_simulation(N, steps, t, total_heat);

        double speedup = T1 / Tp;
        double efficiency = speedup / t;

        cout << t << "         "
             << Tp << "      "
             << speedup << "      "
             << efficiency << endl;
    }

    cout << "\nFinal Total Heat: " << total_heat << endl;

    return 0;
}

