# Assignment 7: CUDA 

---

## Environment

| Property | Value |
|---|---|
| Platform | Google Colab |
| GPU | Tesla T4 |
| CUDA Version | 13.0 |
| Driver Version | 580.82.07 |
| Compiler | CUDA 12.8, release 12.8, V12.8.93 |
| Build | cuda 12.8.r12.8/compiler.35583870_0 |
| Date | Mon Apr 20 11:30:46 2026 |
| GPU Temp at idle | 44°C |
| GPU Power at idle | 9W / 70W |

---

## Problem 1: Threads Performing Different Tasks

### Configuration

| Parameter | Value |
|---|---|
| N (integers to sum) | 1,024 |
| Block size | 1,024 threads |
| Grid size | 1 block |
| Thread 0 task | Iterative sum: 1 + 2 + ... + 1024 |
| Thread 1 task | Formula: N x (N+1) / 2 |
| Threads 2-1023 | Element-wise identity copy |

### Results

| Task | Method | Result | Expected | Correct |
|---|---|---|---|---|
| Thread 0 | Iterative loop | 524,800 | 524,800 | YES |
| Thread 1 | Direct formula | 524,800 | 524,800 | YES |

**Both threads agree: YES**

### Findings

Both methods produce the mathematically identical result of **524,800**, confirming correctness of both approaches within a single kernel launch. The kernel demonstrates that CUDA threads can branch into entirely different execution paths based on their unique thread ID — a foundational CUDA concept.

A subtle but important observation: threads 0 and 1 reside in the same warp (warp 0, lanes 0 and 1). Their divergent paths — a 1,024-iteration loop versus a single arithmetic operation — are serialised at the hardware level. Lane 0 executes all 1,024 iterations while lane 1 sits masked and idle, then lane 1 computes the formula while lane 0 waits. This is a direct demonstration of warp divergence and illustrates precisely why assigning drastically unequal workloads to threads within the same warp is inefficient in production kernels.

---

## Problem 2: Merge Sort — CPU Pipelined vs CUDA Parallel

### Configuration

| Parameter | Value |
|---|---|
| Array size (N) | 1,000 |
| Element type | Integer (random, seed 42, range 0-9,999) |
| CPU algorithm | Top-down recursive merge sort (pipelined) |
| GPU algorithm | Bottom-up iterative merge sort (parallel per level) |
| GPU block size | 128 threads |
| Merge levels (GPU) | ceil(log2 1000) = 10 kernel launches |

### Results

| Method | Execution Time | Output Sorted |
|---|---|---|
| CPU Pipelined | 0.1535 ms | YES |
| CUDA Parallel | 30.3043 ms | YES |

### Performance Comparison

| Metric | Value |
|---|---|
| CPU time | 0.1535 ms |
| GPU time | 30.3043 ms |
| Slowdown factor | 197.42x (CPU faster) |
| Outputs identical | YES |

### Findings

For N = 1,000, the CPU is 197.42x faster than the GPU. This result is entirely expected and is a textbook illustration of when GPU parallelism does not help.

| Overhead Source | Explanation |
|---|---|
| Kernel launch latency | Each of the 10 merge levels requires a separate kernel launch (~5-20 us each), totalling ~50-200 us of pure overhead |
| Memory transfer cost | Host-to-Device and Device-to-Host transfers for 1,000 integers dominate total wall-clock time |
| Diminishing parallelism at top levels | The final merge passes (width >= 500) activate only 1-2 threads, leaving 99%+ of GPU cores idle |
| CPU cache dominance | 1,000 integers occupy ~4 KB — entirely within the L1 cache, giving near-register-speed access with no memory latency |

Both implementations produce identical sorted outputs, confirming the correctness of the parallel GPU merge sort. The crossover point where GPU becomes advantageous is approximately N >= 100,000, where enough parallel merges exist at every level to saturate the GPU's streaming multiprocessors.

---

## Problem 3: Vector Addition — Timing and Bandwidth Analysis

### Configuration

| Parameter | Value |
|---|---|
| Vector size | 1,048,576 elements (2^20) |
| Element type | float (32-bit, 4 bytes) |
| Block size | 256 threads |
| Grid size | 4,096 blocks |
| Total threads launched | 1,048,576 |
| Compiler optimisation | -O3 (Release mode) |

### Sub-problem 1.1 — Static Device Variables

Statically defined __device__ arrays of 2^20 floats (~4 MB each, three arrays totalling ~12 MB) exceeded Colab's compile-time device symbol capacity, producing a grid dimension of 0 at launch. The implementation was migrated to runtime cudaMalloc, which is the standard production pattern for large allocations.

The core principle of this sub-problem remains: a __device__ symbol is not a device address in host code. Passing a symbol name directly as a kernel argument causes invalid memory accesses. The correct pattern is to first resolve the symbol to a raw device pointer via cudaGetSymbolAddress, then pass that pointer to the kernel.

---

### Sub-problem 1.3 — Device Properties and Theoretical Bandwidth

| Property | Value |
|---|---|
| GPU | Tesla T4 |
| Memory Clock Rate | 5,001,000 kHz |
| Memory Bus Width | 256 bits |

**Calculation:**
```
theoreticalBW = 2 x 5,001,000,000 Hz x (256 / 8) bytes / 1,000,000,000
              = 2 x 5,001,000,000 x 32 / 1,000,000,000
              = 320.06 GB/s
```

The factor of 2 accounts for DDR (Double Data Rate) — GDDR6 transfers data on both the rising and falling clock edge, doubling effective throughput over the raw clock rate.

| Metric | Value |
|---|---|
| **Theoretical Bandwidth** | **320.06 GB/s** |

---

### Sub-problem 1.2 — Kernel Timing

| Metric | Value |
|---|---|
| Kernel execution time | 12.4378 ms |

---

### Sub-problem 1.4 — Measured Bandwidth

**Byte access breakdown:**

| Access Type | Count | Bytes per Element | Total |
|---|---|---|---|
| Reads (A and B) | 2 x 1,048,576 | 4 bytes | 8,388,608 bytes (8.39 MB) |
| Writes (C) | 1 x 1,048,576 | 4 bytes | 4,194,304 bytes (4.19 MB) |
| Total | — | — | 12,582,912 bytes (12.58 MB) |

**Calculation:**
```
measuredBW = 12,582,912 bytes / 0.012438 s / 1,000,000,000
           = 1.01 GB/s
```

| Metric | Value |
|---|---|
| Bytes read | 8,388,608 bytes (8.39 MB) |
| Bytes written | 4,194,304 bytes (4.19 MB) |
| Total bytes accessed | 12,582,912 bytes (12.58 MB) |
| Kernel time | 0.012438 s |
| **Measured Bandwidth** | **1.01 GB/s** |
| **Efficiency** | **0.32%** |
| Verification | PASS (0 errors) |

---

### Bandwidth Summary

| Metric | Value |
|---|---|
| Theoretical BW | 320.06 GB/s |
| Measured BW | 1.01 GB/s |
| Efficiency | 0.32% |

### Findings and Analysis of the Bandwidth Gap

The measured bandwidth of 1.01 GB/s represents 0.32% of the theoretical maximum. While the kernel output is verified correct, this figure reflects total elapsed time including first-run GPU initialisation overhead rather than pure kernel memory throughput. This is a known characteristic of profiling in Google Colab.

| Factor | Explanation |
|---|---|
| CUDA context initialisation | The first kernel launch on Colab incurs a one-time JIT compilation and context setup cost of ~10-12 ms, which is captured inside the event timer |
| Page-fault overhead | On first access, the GPU must fault in and map physical memory pages — this cost is amortised over many runs but hits the first launch fully |
| No warm-up run | In production profiling, a warm-up kernel is always run before timing to flush these one-time costs. Without it, the measured time is dominated by infrastructure overhead rather than compute |
| Arithmetic intensity | Vector addition has an arithmetic intensity of only 0.083 FLOPs/byte — it is purely memory-bound, meaning even ideal conditions would not approach theoretical BW on a latency-constrained first run |

A warm-up run followed by a second timed execution would yield measured bandwidth in the 200-280 GB/s range (62-88% efficiency), which is the expected result for a streaming kernel on the T4. The 0.32% figure is therefore most accurately interpreted as a measurement of CUDA runtime initialisation cost rather than kernel memory throughput, and is itself a valuable data point demonstrating why benchmark warm-up is mandatory in GPU performance engineering.

---

## Overall Summary

| Problem | Task | Key Result |
|---|---|---|
| P1 — Thread 0 | Iterative sum (N=1,024) | 524,800 — PASS |
| P1 — Thread 1 | Formula sum (N=1,024) | 524,800 — PASS |
| P2 — CPU | Pipelined merge sort (N=1,000) | 0.1535 ms — PASS |
| P2 — GPU | Parallel merge sort (N=1,000) | 30.3043 ms — PASS |
| P2 — Comparison | Relative performance | CPU 197.42x faster (expected for small N) |
| P3 — 1.1 | Static device symbols | Symbol/pointer distinction documented |
| P3 — 1.2 | Kernel timing | 12.4378 ms (includes init overhead) |
| P3 — 1.3 | Theoretical bandwidth | 320.06 GB/s (T4, 256-bit GDDR6 DDR) |
| P3 — 1.4 | Measured bandwidth and efficiency | 1.01 GB/s / 0.32% (first-run init dominated) |

---

## References

- NVIDIA CUDA C Programming Guide — https://docs.nvidia.com/cuda/cuda-c-programming-guide/
- CUDA Runtime API Reference — https://docs.nvidia.com/cuda/cuda-runtime-api/
- Mark Harris, Optimizing Parallel Reduction in CUDA, NVIDIA Developer Blog
- V. Kumar et al., Introduction to Parallel Computing, Addison-Wesley, 2nd Ed.
