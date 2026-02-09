#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>

using namespace std;

const double epsilon = 1.0;
const double sigma = 1.0;

struct Particle {
    double x, y, z;
    double fx, fy, fz;
};

double run_simulation(int N, int threads) {

    vector<Particle> particles(N);

    for (int i = 0; i < N; i++) {
        particles[i].x = drand48() * 10.0;
        particles[i].y = drand48() * 10.0;
        particles[i].z = drand48() * 10.0;
        particles[i].fx = particles[i].fy = particles[i].fz = 0.0;
    }

    double total_energy = 0.0;

    double start = omp_get_wtime();

    #pragma omp parallel num_threads(threads)
    {
        double local_energy = 0.0;

        #pragma omp for schedule(dynamic)
        for (int i = 0; i < N; i++) {
            for (int j = i + 1; j < N; j++) {

                double dx = particles[i].x - particles[j].x;
                double dy = particles[i].y - particles[j].y;
                double dz = particles[i].z - particles[j].z;

                double r2 = dx*dx + dy*dy + dz*dz;
                double r = sqrt(r2);

                if (r == 0.0) continue;

                double sr = sigma / r;
                double sr6 = pow(sr, 6);
                double sr12 = sr6 * sr6;

                local_energy += 4 * epsilon * (sr12 - sr6);
            }
        }

        #pragma omp atomic
        total_energy += local_energy;
    }

    double end = omp_get_wtime();
    return end - start;
}

int main() {

    int N = 2000;

    double T1 = run_simulation(N, 1);

    cout << "Threads   Time(s)   Speedup   Efficiency\n";
    cout << "------------------------------------------\n";

    for (int t = 1; t <= 8; t *= 2) {
        double Tp = run_simulation(N, t);
        double speedup = T1 / Tp;
        double efficiency = speedup / t;

        cout << t << "         "
             << Tp << "      "
             << speedup << "      "
             << efficiency << endl;
    }

    return 0;
}

