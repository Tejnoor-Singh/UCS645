# Adaptive Parallel Sorting System

An advanced sorting system that dynamically chooses between serial and parallel execution strategies based on the input size to minimize overhead and maximize multi-core processor utilization. The system features a high-performance C++ backend utilizing OpenMP and a modern web dashboard built with Python (Flask) and HTML/CSS/JS to visualize performance metrics.

## Features

- **High-Performance Backend**: Core sorting logic is written in C++ for maximum execution speed.
- **Dynamic Algorithm Selection**: Automatically uses `serialMergeSort` for small array subsets to avoid task creation overhead, and switches to `parallelMergeSort` via OpenMP for large datasets.
- **Beautiful Web Interface**: A sleek, dark-mode web dashboard featuring glassmorphism design, vibrant gradients, and micro-animations.
- **Real-time Performance Metrics**: Visualizes execution times of Serial, Naive Parallel, and Adaptive Sort using interactive charts (Chart.js).

## Project Structure

```text
PARALLEL-COMPUTING/
├── cpp_backend/          # C++ OpenMP Source code
│   ├── main.cpp          # Entry point and benchmark logic
│   ├── sorting.cpp       # Merge sort implementations
│   ├── sorting.h         # Headers
│   └── Makefile          # Build instructions
├── api/                  # Python Flask Web Server
│   ├── app.py            # API & Server logic
│   ├── static/           # CSS and JS for frontend
│   │   ├── style.css
│   │   └── app.js
│   └── templates/        # HTML layout
│       └── index.html
└── README.md             # Project documentation
```

## Prerequisites

Before running the application, ensure you have the following installed:
- **C++ Compiler**: GCC, Clang, or MSVC with OpenMP support.
  - *macOS*: `brew install llvm libomp` (The Makefile handles macOS libomp linking automatically).
  - *Linux*: `sudo apt install g++ libomp-dev`
- **Python 3**: Ensure `python3` and `pip3` are installed.
- **Flask**: Installed via `pip3 install flask` (already provided in the setup instructions).

## Setup & Installation

### 1. Compile the C++ Backend

Navigate to the `cpp_backend` directory and run `make`:

```bash
cd cpp_backend
make clean
make
```
This will compile the `adaptive_sort` binary.

### 2. Run the Web Server

Navigate to the `api` directory and run the Flask server:

```bash
cd ../api
python3 app.py
```
The server will start on `http://127.0.0.1:5000/`.

## Usage

1. Open your web browser and navigate to `http://127.0.0.1:5000/`.
2. You will be greeted by the **Adaptive Parallel Sorting System** dashboard.
3. Enter an array size in the input field (e.g., `5000000` for 5 million elements).
4. Click **"Run Benchmark"**.
5. Wait for the backend to process the array. Once complete, the dashboard will display the execution time for:
   - Serial Sort
   - Naive Parallel Sort
   - Adaptive Sort
6. The interactive bar chart will populate with visual comparisons, and you can see your calculated speedup multiplier and the number of threads utilized by your CPU.

## Methodology

- **Serial Sort**: A standard recursive merge sort implementation. Efficient for small datasets but doesn't utilize multiple CPU cores.
- **Naive Parallel Sort**: Spawns OpenMP tasks for every recursive subdivision, down to subsets of size 1. This generates excessive overhead for very small chunks and often runs slower than serial sort.
- **Adaptive Sort**: Combines the best of both approaches. It utilizes a `threshold` variable. If the current subarray size is greater than the threshold, it spawns OpenMP tasks for parallel execution. If the subarray size is smaller than the threshold, it switches to standard serial sort to avoid the overhead of task creation.

## Objectives Met

- Designed and implemented an adaptive sorting system.
- Analyzed and visualized the performance of serial vs parallel sorting algorithms.
- Dynamically selected the appropriate sorting method based on input size.
- Demonstrated performance improvement via a clear speedup metric.
- Delivered a premium user interface to compare execution time and resource utilization.
