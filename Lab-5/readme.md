# MPI Assignment 5 Report

# Overview

This assignment investigates key concepts in parallel and distributed computing using MPI. The focus is on understanding how communication patterns, workload distribution, and collective operations influence performance and scalability.

The experiments include implementation and analysis of DAXPY computation, comparison between manual and optimized broadcast mechanisms, distributed dot product computation using collective operations, and master worker based task scheduling for prime and perfect number detection.

---

# System Configuration

The experiments were performed on a Linux based system using the MPI compiler mpicc. The number of processes used for execution varied between 1, 2, 4, and 8 in order to evaluate scalability and performance improvements.

---

# Question 1 DAXPY Operation

## Description

The DAXPY operation is defined as
X[i] = a multiplied by X[i] plus Y[i]

This is a fundamental vector operation widely used in scientific computing. The goal was to parallelize this computation using MPI and measure the improvement in execution time compared to a single process implementation.

## Execution Time

| Number of Processes | Time in seconds |
| ------------------- | --------------- |
| 1                   | 0.000068        |
| 2                   | 0.000035        |
| 4                   | 0.000022        |
| 8                   | 0.000018        |

## Speedup Calculation

Speedup is defined as the ratio of execution time on a single process to execution time on multiple processes.

| Number of Processes | Speedup |
| ------------------- | ------- |
| 2                   | 1.94    |
| 4                   | 3.09    |
| 8                   | 3.78    |

## Detailed Analysis

The results demonstrate that parallelization reduces execution time as the number of processes increases. Up to four processes, the speedup is close to linear, indicating efficient utilization of computational resources.

However, when the number of processes increases to eight, the improvement in speedup begins to diminish. This is due to overheads associated with process management, memory access contention, and communication costs. Since the computation per process becomes very small, the overhead dominates the benefits of parallel execution.

This experiment highlights that parallelism is most effective when the computation to communication ratio is sufficiently high.

---

# Question 2 Broadcast Race

## Description

This experiment compares two approaches for broadcasting a large array from one process to all other processes.

The first approach uses a manual implementation where the root process sends data sequentially to each process using point to point communication.

The second approach uses the MPI provided broadcast function which internally implements an optimized communication strategy.

## Execution Time

| Number of Processes | Manual Broadcast Time | MPI Broadcast Time |
| ------------------- | --------------------- | ------------------ |
| 2                   | 0.052571              | 0.017737           |
| 4                   | 0.171679              | 0.025523           |
| 8                   | 0.432988              | 0.032641           |

## Detailed Analysis

In the manual broadcast implementation, the root process sends data individually to each process. This results in a linear communication pattern. As the number of processes increases, the total communication time increases significantly because the root process becomes a bottleneck.

In contrast, MPI broadcast uses a tree based communication pattern. In this approach, the data is propagated in stages, where multiple processes forward the data simultaneously. This reduces the total number of communication steps required.

The observed results clearly show that manual broadcast scales poorly with increasing processes, while MPI broadcast scales efficiently. The theoretical complexity of manual broadcast is proportional to the number of processes, whereas MPI broadcast achieves logarithmic complexity.

This experiment demonstrates the importance of using optimized collective communication routines provided by MPI instead of manually implementing communication patterns.

---

# Question 3 Distributed Dot Product and Amdahl Law

## Description

The objective of this experiment is to compute the dot product of two very large vectors using parallel processing. The workload is divided among multiple processes, each computing a partial result. These partial results are then combined using a reduction operation.

## Execution Time

| Number of Processes | Time in seconds |
| ------------------- | --------------- |
| 1                   | 0.549298        |
| 2                   | 0.297956        |
| 4                   | 0.210852        |
| 8                   | 0.106226        |

## Speedup

| Number of Processes | Speedup |
| ------------------- | ------- |
| 2                   | 1.84    |
| 4                   | 2.60    |
| 8                   | 5.17    |

## Efficiency

Efficiency is defined as the ratio of speedup to the number of processes.

| Number of Processes | Efficiency |
| ------------------- | ---------- |
| 2                   | 0.92       |
| 4                   | 0.65       |
| 8                   | 0.65       |

## Detailed Analysis

The results show a clear reduction in execution time as the number of processes increases. The speedup is significant, especially when moving from one to eight processes.

However, the efficiency decreases as more processes are added. This is a direct consequence of Amdahl Law, which states that the speedup of a parallel program is limited by the portion of the program that cannot be parallelized.

In this case, the overhead comes from communication operations such as broadcasting the multiplier and reducing partial results. Synchronization between processes also contributes to delays.

Although the computation part is highly parallelizable, these overheads prevent perfect linear scaling. This demonstrates a key limitation of parallel systems and emphasizes the importance of minimizing communication overhead.

---

# Question 4 Prime Number Computation

## Description

This program uses a master worker model to find all prime numbers up to a given limit. The master process assigns numbers to worker processes dynamically. Each worker tests whether a number is prime and returns the result.

## Output

The program correctly identifies all prime numbers up to 100.

## Detailed Analysis

The use of dynamic task allocation ensures that all worker processes remain busy, leading to better load balancing. Instead of assigning a fixed chunk of numbers to each worker, tasks are distributed based on availability.

This approach is particularly useful when the computation time for each task varies, as it prevents some processes from being idle while others are still working.

The implementation demonstrates effective use of point to point communication and dynamic scheduling.

---

# Question 5 Perfect Number Computation

## Description

This program follows a similar master worker approach to identify perfect numbers. A perfect number is defined as a number that is equal to the sum of its proper divisors.

## Output

The program identifies the following perfect numbers up to 10000

6, 28, 496, 8128

## Detailed Analysis

Perfect number computation is more computationally intensive compared to prime number checking. The master worker model helps distribute this workload efficiently.

Since perfect numbers are rare, the output is sparse. However, the computation required to verify each number is significant. Parallel processing reduces the overall execution time by distributing these checks among multiple processes.

This experiment reinforces the importance of parallelism in handling computationally intensive tasks.

---

# Final Conclusion

The experiments conducted in this assignment clearly demonstrate the advantages and limitations of parallel computing using MPI.

Parallelization significantly reduces execution time for computational tasks. However, the benefits are constrained by communication overhead and synchronization costs.

Optimized collective operations such as MPI broadcast provide substantial performance improvements over manual implementations.

The master worker model proves to be effective for dynamic task distribution and load balancing.

Overall, this assignment highlights the importance of choosing appropriate communication strategies and understanding the trade offs involved in parallel system design.

---

# Execution Instructions

To compile the programs
make

To run the programs

mpirun -np 1 ./q1
mpirun -np 2 ./q1
mpirun -np 4 ./q1
mpirun -np 8 ./q1

mpirun -np 2 ./q2
mpirun -np 4 ./q2
mpirun -np 8 ./q2

mpirun -np 1 ./q3
mpirun -np 2 ./q3
mpirun -np 4 ./q3
mpirun -np 8 ./q3

mpirun -np 4 ./q4
mpirun -np 4 ./q5

---

# Key Observations

Parallel programs do not always achieve perfect speedup due to overhead.
Communication patterns significantly impact performance.
Tree based communication is more efficient than linear communication.
Dynamic task scheduling improves load balancing.
Amdahl Law limits scalability in real world systems.
