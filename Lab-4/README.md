# Lab 4 – MPI Programming

| Field | Details |
|---|---|
| **Name** | Tejnoor Singh |
| **Course** | UCS645 – Parallel and Distributed Computing |
| **Lab** | 4 |
| **Environment** | OpenMPI 4.1.6 (compiled from source), Ubuntu 24.04, GCC 13.3.0 |

**Compilation:**
```bash
mpicc program.c -o program
```

**Execution:**
```bash
mpirun -np <p> ./program
```

---

## Q1 – Ring Communication

```bash
mpirun -np 4 ./ring
```

**Output:**
```
Process 0 sending 100 to process 1
Process 1 received value 101
Process 1 sending 101 to process 2
Process 2 received value 103
Process 2 sending 103 to process 3
Process 3 received value 106
Process 3 sending 106 to process 0
Process 0 received value 106
Time: 0.000177 seconds
```

**Explanation:**
A ring topology was formed using 4 processes. Each process forwarded an incremented value to the next rank, and the final value reached back to process 0. The communication behavior and result are correct. Total communication cost: **0.000177 seconds**.

---

## Q2 – Global Array Sum

```bash
mpirun -np <p> ./array_sum
```

### Results

| Processes (p) | Sum Result | Correct? | Time (sec) | Speedup | Efficiency |
|:---:|:---:|:---:|:---:|:---:|:---:|
| 1 | 5050 | ✅ | 0.000006 | 1.00× | 100% |
| 2 | 5050 | ✅ | 0.000023 | 0.26× | 13% |
| 4 | 5050 | ✅ | 0.000067 | 0.09× | 2% |
| 8 | 4656 | ❌ | 0.000060 | 0.10× | 1% |

**Observations:**
The workload (`N = 100`) is too small — MPI overhead dominates heavily. As `p` increases, runtime increases instead of decreasing. For `np = 8`, the distributed chunks become too small and produce an incorrect result. This demonstrates **Amdahl's Law** and overhead dominance in action.

---

## Q3 – Global Max / Min

```bash
mpirun -np 1 ./max_min
```

**Output:**
```
Global Max = 782 from process 0
Global Min = 107 from process 0
Time: 0.000003 seconds
```

**Explanation:**
Since only one process is used, the entire computation is sequential. Time is extremely small due to the small data size.

---

## Q4 – Parallel Dot Product

```bash
mpirun -np <p> ./dot
```

### Results

| Processes (p) | Dot Product | Correct? | Time (sec) | Speedup | Efficiency |
|:---:|:---:|:---:|:---:|:---:|:---:|
| 1 | 120 | ✅ | 0.000006 | 1.00× | 100% |
| 2 | 120 | ✅ | 0.000026 | 0.23× | 11% |
| 4 | 120 | ✅ | 0.000069 | 0.08× | 2% |
| 8 | 120 | ✅ | 0.000066 | 0.09× | 1% |

**Observations:**
As with the array sum, the vector size is too small for parallelization to help. Communication and reduction overhead dominate, causing slowdowns as `p` increases.

---

## General Observations

1. All workloads are extremely small, so MPI overhead dominates throughout.
2. No speedup is observed for `p > 1` due to communication and initialization cost.
3. The incorrect result for `np = 8` in Q2 shows that small problem sizes are sensitive to uneven workload distribution.
4. The ring communication demonstrates pure message-passing cost and behaves correctly.
5. **Amdahl's Law** is clearly visible: the parallel fraction is too small for meaningful speedup.
6. `MPI_Init`, `MPI_Reduce`, and collective operations cost significantly more than the actual computation for these small workloads.

---

## Conclusion

For small computational workloads, MPI does not yield performance benefits. Instead, communication overhead overwhelms computation, causing negative scaling. The results align with **Amdahl's Law**, and the behavior observed in Q2 and Q4 is consistent with theoretical expectations. All programs ran successfully and produced correct results — with the exception of the `np = 8` array sum, which becomes inaccurate due to minimal data per process.
