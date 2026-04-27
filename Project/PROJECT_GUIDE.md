# Adaptive Parallel Sorting System: Complete Project Guide

This document provides a comprehensive, end-to-end overview of the project. It is designed to act as a study guide, project presentation reference, and FAQ to help you thoroughly understand every aspect of what we have built, why we built it, and how it works.

---

## 1. What is this project about?
This project is an **Adaptive Parallel Sorting System**. It focuses on solving a specific problem in the field of parallel computing: the **overhead** created by parallelizing tasks that are too small. 

While splitting a computational task (like sorting) across multiple CPU cores (parallel computing) significantly speeds up processing for massive datasets, it actually slows down processing for very small datasets. This happens because the CPU spends more time and resources creating, managing, and synchronizing threads than it does actually sorting the data. 

## 2. What is our Goal?
The primary goal is to **optimize sorting performance** by introducing an *adaptive decision-making mechanism*.
Instead of blindly using a parallel sorting algorithm for all data sizes, the system checks the size of the array segment at runtime.
- If the array segment is smaller than a specific **threshold** (e.g., 100 or 10,000 elements), it sorts it using a single core (Serial Sort).
- If the array segment is large, it divides the work across multiple cores (Parallel Sort).

By dynamically choosing the best algorithm, we aim to achieve the lowest possible execution time and maximum hardware utilization.

## 3. What technologies did we use?
We built a modern, full-stack application to execute the complex math and visualize the results intuitively:
- **C++**: The core programming language used for the backend due to its extreme performance and low-level memory management capabilities.
- **OpenMP (Open Multi-Processing)**: A powerful C++ library used to parallelize the recursive merge sort operations. It allows us to easily split sorting tasks across multiple CPU threads.
- **Python (Flask)**: Used to create a lightweight web server that acts as a bridge (API) between the C++ backend and the frontend User Interface.
- **HTML / CSS / Vanilla JavaScript**: Used to build a premium, interactive web dashboard. CSS was used to implement a modern "glassmorphism" aesthetic with dark-mode styling.
- **Chart.js**: A JavaScript visualization library used to dynamically plot the performance differences on an interactive bar chart.

## 4. How is it implemented? (Step-by-Step)

### Phase 1: The Core Algorithm (C++)
We implemented the classic **Merge Sort** algorithm in three different ways inside `sorting.cpp`:
1. **Serial Merge Sort**: The standard algorithm running on a single thread. It recursively splits the array in half and merges the sorted halves.
2. **Naive Parallel Merge Sort**: Uses `#pragma omp task` to spawn a new thread for *every* recursive split, all the way down to an array size of 1. This is considered "naive" because it creates thousands of tiny tasks for small arrays, overwhelming the CPU with task management overhead.
3. **Adaptive Merge Sort**: The optimized version. It includes an intelligent `if/else` condition:
   ```cpp
   if (subarray_size <= threshold) {
       serialMergeSort(...); // Avoid thread overhead, use a single core
   } else {
       // Use OpenMP tasks to split the work across multiple cores
       #pragma omp task
       adaptiveMergeSortImpl(...);
       // ...
   }
   ```

### Phase 2: The Command-Line Interface (C++)
The C++ program (`main.cpp`) acts as our high-speed calculator. It accepts an array size as an input argument, generates a random array of that size, runs all three sorting algorithms, measures their execution times in milliseconds, and prints the results in a structured **JSON** format.

### Phase 3: The Web API (Python/Flask)
The Flask server (`app.py`) listens for HTTP requests from the browser. When a user requests a benchmark from the UI, the Python script uses the `subprocess` module to execute the compiled C++ binary in the background. It captures the JSON output from the C++, calculates the "Speedup" multiplier (Serial Time ÷ Adaptive Time), and sends the finalized data back to the browser.

### Phase 4: The User Interface (Frontend)
The web dashboard (`index.html` & `app.js`) allows the user to input any array size. When the user clicks "Run Benchmark", JavaScript makes an asynchronous `fetch` request to the Python API. While waiting, a loading animation plays. Once the data is returned, the UI dynamically updates the Metric Cards and plots the execution times on the Chart.js graph.

---

## 5. Potential Questions and Answers (FAQ / Presentation Prep)

**Q: Why does the "Naive Parallel Sort" sometimes run slower than the standard "Serial Sort"?**
A: Because creating, scheduling, and destroying threads in OpenMP takes a small amount of computational time. If the array you are sorting is very small, the time spent managing the threads is actually greater than the time it would take a single thread to just sort the array sequentially. This is known as **"Parallel Overhead"**.

**Q: What is the "Threshold" and how does it work?**
A: The threshold is the cutoff size where the algorithm switches from parallel execution to serial execution. For example, if the threshold is 10,000, any subarray with fewer than 10,000 elements is passed to the Serial Merge Sort to prevent thread overhead. This number is usually determined through experimental benchmarking—finding the "sweet spot" where serial sorting becomes faster than parallel task creation.

**Q: What is "Speedup"?**
A: Speedup is a standard metric used in parallel computing to measure how much faster a parallel algorithm is compared to the best serial algorithm. It is calculated as: `Speedup = Time(Serial) / Time(Adaptive)`. A speedup of 2.0x means the adaptive parallel version is twice as fast as the serial version.

**Q: How exactly does OpenMP work in this code?**
A: OpenMP uses compiler directives (like `#pragma omp parallel` and `#pragma omp task`). It creates a pool of worker threads. When a `#pragma omp task` is encountered (during the recursive split of the array), OpenMP assigns that chunk of work to an available thread in the pool. A `#pragma omp taskwait` is used to ensure that the program waits for both halves of the array to be completely sorted before it attempts to merge them together.

**Q: Why did we build a Web Interface instead of just using the command line?**
A: To provide a clear, visual, and user-friendly demonstration of the performance differences. A graphical chart instantly conveys the impact of Parallel Overhead and the efficiency of the Adaptive approach in a way that raw terminal numbers cannot.

---

## 6. Conclusion
This project successfully demonstrates a fundamental concept in High-Performance Computing (HPC): **Parallelism is powerful, but it must be applied intelligently.** By combining the strengths of serial execution (low overhead) and parallel execution (multi-core processing), the Adaptive Parallel Sorting System provides an optimal, real-world solution to data sorting.
