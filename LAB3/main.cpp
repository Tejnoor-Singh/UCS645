#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include <omp.h>

void run_serial(int rows, int cols, const float* input, float* output);
void run_parallel(int rows, int cols, const float* input, float* output);
void run_optimized(int rows, int cols, const float* input, float* output);

struct Metrics {
    double elapsed{};
    long long pairs{};
    long long ops{};
    double rate{};
    double gflops{};

    void compute(int r, int c) {
        pairs = (long long)r * (r + 1) / 2;
        ops = pairs * c * 5;
        rate = pairs / elapsed;
        gflops = (ops / 1e9) / elapsed;
    }

    void display(const std::string& label, int t) const {
        std::cout << std::fixed << std::setprecision(6);
        std::cout << label << " (" << t << " threads)\n";
        std::cout << "Time        : " << elapsed << " sec\n";
        std::cout << "Throughput  : " << std::setprecision(2) << rate << "\n";
        std::cout << "GFLOPS      : " << std::setprecision(3) << gflops << "\n\n";
    }
};

int main(int argc, char* argv[]) {

    if (argc < 3) {
        std::cout << "Usage: ./correlate <rows> <cols>\n";
        return 1;
    }

    int rows = std::atoi(argv[1]);
    int cols = std::atoi(argv[2]);

    float* matrix = new float[rows * cols];
    float* result = new float[rows * rows];

    srand(42);
    for (int i = 0; i < rows * cols; i++)
        matrix[i] = static_cast<float>(rand()) / RAND_MAX;

    int thread_set[] = {1, 2, 4, 8};
    Metrics s[4], p[4], f[4];

    for (int i = 0; i < 4; i++) {
        omp_set_num_threads(thread_set[i]);

        auto t1 = std::chrono::high_resolution_clock::now();
        run_serial(rows, cols, matrix, result);
        auto t2 = std::chrono::high_resolution_clock::now();
        s[i].elapsed = std::chrono::duration<double>(t2 - t1).count();
        s[i].compute(rows, cols);
        s[i].display("Serial", thread_set[i]);

        t1 = std::chrono::high_resolution_clock::now();
        run_parallel(rows, cols, matrix, result);
        t2 = std::chrono::high_resolution_clock::now();
        p[i].elapsed = std::chrono::duration<double>(t2 - t1).count();
        p[i].compute(rows, cols);
        p[i].display("OpenMP", thread_set[i]);

        t1 = std::chrono::high_resolution_clock::now();
        run_optimized(rows, cols, matrix, result);
        t2 = std::chrono::high_resolution_clock::now();
        f[i].elapsed = std::chrono::duration<double>(t2 - t1).count();
        f[i].compute(rows, cols);
        f[i].display("Optimized", thread_set[i]);
    }

    delete[] matrix;
    delete[] result;
    return 0;
}
