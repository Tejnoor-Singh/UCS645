#ifndef SIGNAL_OPS_H
#define SIGNAL_OPS_H

void run_serial(int rows, int cols, const float* input, float* output);
void run_parallel(int rows, int cols, const float* input, float* output);
void run_optimized(int rows, int cols, const float* input, float* output);

#endif
