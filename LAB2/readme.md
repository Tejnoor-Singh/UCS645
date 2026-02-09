# UCS645 – Parallel and Distributed Computing  
## Lab Assignment 2: OpenMP Performance Evaluation

---

## Introduction

This experiment analyzes the performance of OpenMP programs by measuring execution time, speedup, and efficiency across multiple threads. Three computational problems are used to study scalability and synchronization overhead.

---

## Experiment 1: Molecular Dynamics Simulation (Lennard-Jones Potential)

### Aim
To compute pairwise Lennard-Jones forces and total potential energy for a system of particles in three-dimensional space.

### Parallelization Approach
- Outer iteration loop parallelized using OpenMP
- Atomic operations used for force updates
- Reduction clause applied for total energy calculation
- Dynamic scheduling used to balance workload

### Performance Results

| Threads | Time (s) | Speedup | Efficiency |
|--------|----------|---------|------------|
| 1 | 0.103977 | 1.00184 | 1.00184 |
| 2 | 0.0539918 | 1.92933 | 0.964667 |
| 4 | 0.0271062 | 3.84297 | 0.960742 |
| 8 | 0.0276139 | 3.77231 | 0.471538 |

### Analysis
- Near-linear speedup achieved up to 4 threads
- Maximum efficiency observed at 4 threads
- Performance saturates at 8 threads due to limited physical cores

---

## Experiment 2: Smith–Waterman DNA Sequence Alignment

### Problem Description
Local sequence alignment is performed using dynamic programming. Each cell value is computed as:

H(i, j) = max(0, diagonal + match/mismatch, up + gap, left + gap)

### Parallel Strategy
- Wavefront (anti-diagonal) parallelization
- Synchronization after each diagonal computation
- Reduction used to compute maximum alignment score

Matrix Size: 10000 × 10000

### Performance Results

| Threads | Time (s) | Speedup | Efficiency |
|--------|----------|---------|------------|
| 1 | 6.924757 | 1.000000 | 1.000000 |
| 2 | 4.037569 | 1.715081 | 0.857540 |
| 4 | 2.748181 | 2.519761 | 0.629940 |
| 8 | 7.644054 | 0.905901 | 0.113238 |

### Analysis
- Moderate scalability up to 4 threads
- Efficiency decreases due to synchronization overhead
- Performance degrades at 8 threads due to oversubscription
- Wavefront dependency limits available parallelism

---

## Experiment 3: Two-Dimensional Heat Diffusion

### Objective
To simulate heat diffusion using a finite difference method.

T_new(i, j) = 0.25 × [T(i+1, j) + T(i-1, j) + T(i, j+1) + T(i, j-1)]

### Parallel Implementation
- Grid points distributed across threads
- No race conditions as each thread writes to separate cells
- Single barrier per time step
- Reduction used to calculate total heat

Grid Size: 2000 × 2000  
Time Steps: 500

### Performance Results

| Threads | Time (s) | Speedup | Efficiency |
|--------|----------|---------|------------|
| 1 | 38.351733 | 1.000000 | 1.000000 |
| 2 | 16.969854 | 2.259992 | 1.129996 |
| 4 | 9.276156 | 4.134442 | 1.033611 |
| 8 | 9.438288 | 4.063420 | 0.507928 |

Final Total Heat: 1000.000000

### Analysis
- Near-ideal scalability up to 4 threads
- Superlinear speedup due to improved cache utilization
- Performance saturates beyond physical core limit

---

## Conclusion

Among the three applications, Molecular Dynamics and Heat Diffusion demonstrate better scalability due to fewer synchronization requirements. Smith–Waterman exhibits limited scalability because of strict data dependencies and frequent synchronization barriers.
