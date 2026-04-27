# Assignment 6: Introduction to CUDA

## Overview
- This assignment covers basic CUDA programming
- Focus on GPU properties and parallel computation
- Implemented:
  - Device Query
  - Array Sum using CUDA
  - Matrix Addition using CUDA

---

## Part A: Device Query

### GPU Details
- GPU Name: Tesla T4
- Compute Capability: 7.5

### Block and Grid Limits
- Max Threads per Block: 1024
- Max Block Dimensions: 1024 x 1024 x 64
- Max Grid Dimensions: 2147483647 x 65535 x 65535

### Memory Details
- Global Memory: 14.56 GB
- Shared Memory per Block: 48 KB
- Constant Memory: 64 KB

### Other Properties
- Warp Size: 32
- Multiprocessors: 40
- Max Threads per Multiprocessor: 1024
- Double Precision Support: Yes

---

### Answers

**Maximum Threads**
- Max grid size = 65535  
- Max block size = 512  
- Maximum threads = 65535 × 512 = 33,553,920  

**When not to use max threads**
- Can cause:
  - Resource waste
  - Memory conflicts
  - Lower performance

**What limits threads**
- Shared memory
- Registers
- Hardware limits
- Kernel design

---

### Memory Types

- Shared Memory  
  - Fast, inside GPU  
  - Used within a block  

- Global Memory  
  - Large but slower  
  - Used by all threads  

- Constant Memory  
  - Read-only  
  - Fast for repeated reads  

---

### Warp Size
- Warp = group of threads running together  
- Warp size = 32  

---

## Part B: Array Sum

### Problem
- Compute sum of large array using GPU

### Input
- Array Size: 1,048,576 elements  

---

### Results

- CPU Result: 1048576.000000  
- GPU Result: 1048576.000000  

### Performance

- CPU Time: 3.0350 ms  
- GPU Time: 16.4526 ms  
- Speedup: 0.18x  

---

### Observations
- GPU is slower here because:
  - Problem size is small
  - Memory transfer overhead is high
- CPU is faster for simple operations
- GPU benefits appear for larger workloads

---

## Part C: Matrix Addition

### Problem
- Add two matrices using CUDA

### Input
- Matrix Size: 1024 x 1024  

---

### Configuration
- Block Size: 32 x 32  
- Threads per Block: 1024  
- Grid Size: 32 x 32  

---

### Kernel Metrics

- Total Elements: 1,048,576  

**FLOPs**
- 1 addition per element  
- Total = 1,048,576  

**Global Memory Reads**
- 2 reads per element  
- Total = 2,097,152  

**Global Memory Writes**
- 1 write per element  
- Total = 1,048,576  

---

### Performance

- CPU Time: 4.7190 ms  
- GPU Time: 14.8788 ms  
- Speedup: 0.32x  

---

### Verification
- Status: PASS  
- Errors: 0  

---

### Observations
- Matrix addition is memory bound
- GPU slower due to:
  - Memory access overhead
  - Small problem size
- GPU advantage appears in heavier computations

---

## How to Run

### Requirements
- CUDA Toolkit installed  
- GPU available (Tesla T4 used here)  
- Jupyter Notebook or terminal  

---

### Steps
1. Compile code  

