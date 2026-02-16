#include <cmath>
#include <omp.h>
#include "signal_ops.h"

static void normalize(int rows, int cols, const float* in, double* out) {
    #pragma omp parallel for
    for (int r = 0; r < rows; r++) {
        double mean = 0.0;
        for (int c = 0; c < cols; c++)
            mean += in[c + r * cols];
        mean /= cols;

        double mag = 0.0;
        for (int c = 0; c < cols; c++) {
            out[c + r * cols] = in[c + r * cols] - mean;
            mag += out[c + r * cols] * out[c + r * cols];
        }

        double scale = 1.0 / std::sqrt(mag);
        for (int c = 0; c < cols; c++)
            out[c + r * cols] *= scale;
    }
}

void run_serial(int rows, int cols, const float* input, float* output) {
    double* buf = new double[rows * cols];
    normalize(rows, cols, input, buf);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j <= i; j++) {
            double sum = 0.0;
            for (int c = 0; c < cols; c++)
                sum += buf[c + i * cols] * buf[c + j * cols];
            output[i + j * rows] = (float)sum;
        }

    delete[] buf;
}

void run_parallel(int rows, int cols, const float* input, float* output) {
    double* buf = new double[rows * cols];
    normalize(rows, cols, input, buf);

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; i++)
        for (int j = 0; j <= i; j++) {
            double sum = 0.0;
            for (int c = 0; c < cols; c++)
                sum += buf[c + i * cols] * buf[c + j * cols];
            output[i + j * rows] = (float)sum;
        }

    delete[] buf;
}

void run_optimized(int rows, int cols, const float* input, float* output) {
    double* buf = new double[rows * cols];
    normalize(rows, cols, input, buf);

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < rows; i++)
        for (int j = 0; j <= i; j++) {
            double sum = 0.0;
            #pragma omp simd reduction(+:sum)
            for (int c = 0; c < cols; c++)
                sum += buf[c + i * cols] * buf[c + j * cols];
            output[i + j * rows] = (float)sum;
        }

    delete[] buf;
}
