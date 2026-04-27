# 💻 Parallel and Distributed Computing (UCS645)
**Thapar Institute of Engineering and Technology**

This repository documents high-performance implementations, scalability evaluations, and accelerator programming using **OpenMP, MPI, and CUDA**. The project progresses from shared-memory parallelism to distributed systems and GPU computing.

---

## 📂 Repository Structure

### 🧪 [Lab 1: Fundamentals of OpenMP](./Lab1/)
Basic implementations to understand thread management and data sharing.

- **DAXPY Loop:** Measuring speedup in vector operations  
- **Matrix Multiplication:** Comparing 1D vs 2D threading strategies  
- **PI Calculation:** Using `reduction` clause for numerical integration  

---

### 🚀 [Lab 2: Performance Evaluation](./Lab2/)
Scalability, speedup, and efficiency analysis across multiple threads.

- **Molecular Dynamics:** Lennard-Jones pairwise force calculations  
- **Smith–Waterman Algorithm:** Wavefront parallelization bottleneck study  
- **2D Heat Diffusion:** Superlinear speedup via cache locality  

---

### ⚡ [Lab 3: Optimized Correlation Matrix](./Lab3/)
High-performance Pearson correlation for large matrices.

- **Pre-normalization:** Converts correlation into dot product  
- **SIMD Optimization:** `collapse(2)` + `omp simd`  
- **Peak Throughput:** **16.97 GFLOPS**  

---

### 🌐 [Lab 4: MPI Programming](./Lab4/)
Introduction to distributed-memory parallelism using MPI.

- **Ring Communication:** Token passing across ranks  
- **Global Array Sum:** Reduction and workload distribution  
- **Global Max/Min:** Collective reduction operations  
- **Parallel Dot Product:** Scatter + Reduce performance analysis  

#### Key Findings
- MPI overhead dominates for small workloads  
- Communication cost > computation for tiny arrays  
- Demonstrates **Amdahl’s Law** clearly  
- Ring communication performed correctly  

---

### 🛰️ [Lab 5: Advanced MPI Programming](./Lab5/)
Scalable distributed algorithms and communication optimization.

- **DAXPY with MPI**
- **Manual Broadcast vs MPI_Bcast**
- **Large Vector Dot Product**
- **Prime Number Search (Master-Worker)**
- **Perfect Number Detection**

#### Key Findings
- `MPI_Bcast` significantly outperforms manual broadcast  
- Dynamic scheduling improves load balancing  
- Dot product reached **5.17× speedup** at 8 processes  
- Master-worker model effective for irregular workloads :contentReference[oaicite:0]{index=0}

---

### 🔥 [Lab 6: Introduction to CUDA](./Lab6/)
Basic GPU programming using CUDA.

- **Device Query:** Tesla T4 architecture inspection  
- **Array Sum:** CPU vs GPU comparison  
- **Matrix Addition:** CUDA kernel launch + correctness testing  

#### Key Findings
- GPU slower for small workloads due to transfer overhead  
- Matrix addition is memory-bound  
- GPU benefits appear for larger workloads  

---

### ⚙️ [Lab 7: CUDA Performance Engineering](./Lab7/)
Advanced CUDA optimization and benchmarking.

- **Thread Divergence Demo**
- **CPU vs GPU Merge Sort**
- **Vector Addition Bandwidth Analysis**
- **Theoretical vs Measured GPU Memory Throughput**

#### Key Results
- Tesla T4 theoretical BW: **320.06 GB/s**  
- Demonstrated warp divergence behavior  
- CPU faster than GPU for small `N=1000` merge sort  
- Showed why warm-up runs matter in GPU benchmarking :contentReference[oaicite:1]{index=1}

---

# 📊 Unified Performance Summary

| Application | Peak Speedup | Peak GFLOPS | Best Threads/Processes | Scaling Characteristic |
|---|---:|---:|---:|---|
| Heat Diffusion | 4.13× | - | 4 Threads | Superlinear (Cache) |
| Molecular Dynamics | 3.84× | - | 4 Threads | Near-Ideal |
| Correlation Matrix | 3.73× | **16.97** | 4 Threads | Compute Bound |
| MPI Dot Product | **5.17×** | - | 8 Processes | Communication Limited |
| CUDA Vector Add | - | - | Tesla T4 | Memory Bound |

---

# 🏁 Key Engineering Insights

## 1. The 4-Core Sweet Spot
Across OpenMP workloads, scaling saturates beyond 4 threads due to bandwidth contention and SMT overhead.

## 2. Communication Matters
MPI performance heavily depends on collective communication efficiency.

## 3. Cache Can Beat Theory
Heat diffusion achieved superlinear speedup due to improved cache locality.

## 4. GPUs Need Large Workloads
Small problems underutilize GPUs because kernel launch + memory transfer dominate runtime.

## 5. Algorithm > Hardware
Pre-normalization and optimized communication often matter more than raw compute power.

---

# 🧠 Skills Demonstrated

- OpenMP Parallel Programming  
- MPI Distributed Computing  
- CUDA GPU Programming  
- SIMD Vectorization  
- Performance Benchmarking  
- Scalability Analysis  
- Memory Hierarchy Optimization  
- C/C++ High Performance Computing  

---

# 🛠️ Requirements & Execution

## OpenMP
```bash
g++ -O3 -fopenmp file.cpp -o run
./run
