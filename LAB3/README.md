# Parallel Correlation Matrix Computation (OpenMP)

## Introduction

This project computes the **Pearson correlation matrix** for a given data matrix using multiple execution strategies.  
Each row of the input matrix is treated as a signal, and correlations are calculated between all valid row pairs.

The assignment demonstrates **parallel programming concepts**, performance analysis, and optimization using **OpenMP**.

---

## Problem Description

Given an input matrix of size `ny × nx`:

- Compute correlation between all row pairs `(i, j)` such that `j ≤ i`
- Store results in **lower-triangular format**
- Indexing scheme used:

- All calculations use **double-precision arithmetic** for numerical stability

---

## Implemented Approaches

### 1. Sequential Version
- Single-threaded baseline
- Direct correlation computation
- Used as reference for speedup evaluation

### 2. OpenMP Parallel Version
- Parallelized using OpenMP directives
- Nested loops flattened using `collapse(2)`
- Improves CPU utilization across multiple cores

### 3. Optimized Parallel Version
- Row normalization (mean removal and magnitude scaling)
- Converts correlation into dot-product operation
- Uses:
- OpenMP parallel loops
- SIMD vectorization (`#pragma omp simd`)
- Compiler optimization flag `-O3`
- Improves cache efficiency and instruction throughput

---

## Build Instructions

### Requirements
- g++ with OpenMP support
- Linux / WSL environment recommended

### Compile
```bash
make clean
make

./correlate <ny> <nx>


Performance Evaluation
Test Environment

Platform: WSL2 (Ubuntu 22.04)

Compiler: g++ 11.x

Flags: -O3 -fopenmp

CPU cores: 8

Matrix size: 500 × 500

Execution Time Summary
Threads	Sequential (s)	OpenMP (s)	Optimized (s)
1	0.0698	0.0710	0.0699
2	0.0652	0.0342	0.0348
4	0.0650	0.0185	0.0187
8	0.0842	0.0348	0.0276

#SPEEDUP ANALYSIS

Speedup = T_serial(1 thread) / T_parallel(N threads)

Threads	Speedup	Efficiency
1	1.00×	100%
2	2.01×	100%
4	3.73×	93%
8	2.53×	32%

Observation:

Near-linear scaling up to 4 threads

Reduced efficiency at 8 threads due to memory bandwidth limitations

Optimization Techniques Used
Algorithm-Level

Row pre-normalization

Avoidance of redundant symmetric calculations

Reduced arithmetic in inner loops

Parallel-Level

OpenMP threading

Dynamic scheduling for better load balance

Loop collapsing

Low-Level

SIMD vectorization

Cache-friendly memory access patterns

Aggressive compiler optimizations (-O3)

Computational Complexity
Aspect	Complexity
Time Complexity	O(ny² × nx)
Memory Usage	O(ny × nx)
Total Correlations	ny × (ny + 1) / 2

For a 500 × 500 matrix:

Total correlations: 125,250

Approximate operations: 313 million FLOPs

Project Structure
lab_3/
├── app.cpp            # Driver and benchmarking logic
├── signal_ops.cpp     # Correlation implementations
├── signal_ops.h       # Function declarations
├── Makefile           # Build configuration
├── README.md          # Documentation

Notes

Thread count is set programmatically using OpenMP

Input data is generated using random values in range [0, 1)

High-resolution timing is used for benchmarking

Object files and binaries are excluded using .gitignore

Quick Start (WSL)
make clean && make
./correlate 500 500

Conclusion

This project highlights how parallelization and optimization techniques significantly improve performance.
The best performance is achieved at 4 threads, showing strong scalability before hardware limits dominate.