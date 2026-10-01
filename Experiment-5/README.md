# Experiment-5 : Develop Multithreaded Programs Using Parallel Programming Libraries to Understand Thread Creation, Management, and Coordination

## Experiment Overview

This experiment demonstrates the development of multithreaded programs using **Pthreads and OpenMP**. The experiment focuses on thread creation, thread management, work distribution, race conditions, synchronization, coordination, and performance analysis.

The experiment was performed using **WSL Ubuntu on a Windows laptop** with GCC, Pthreads, and OpenMP.

---

## 1. Objectives

The main objectives of this experiment are:

- To understand the concept of threads and multithreading.
- To create and manage threads using Pthreads.
- To create parallel regions using OpenMP.
- To understand how work can be distributed among multiple threads.
- To demonstrate race conditions in multithreaded programs.
- To use mutexes and critical sections for synchronization.
- To understand thread coordination using barriers.
- To compare sequential, Pthreads, and OpenMP execution.
- To calculate speedup and efficiency.
- To analyze the effect of increasing the number of threads on execution time.

---

## 2. Configuration

### Hardware

- **System:** Dell Inspiron 15 3520
- **RAM:** 8 GB
- **Processor:** Intel Core i5-1235U
- **Logical CPUs:** 12

### Software

- **Operating System:** Windows 11
- **Linux Environment:** WSL Ubuntu
- **Compiler:** GCC
- **Programming Language:** C
- **Parallel Programming Libraries:**
  - POSIX Threads (Pthreads)
  - OpenMP
- **Editor:** Nano
- **Terminal:** WSL Ubuntu Terminal

### Compilation

Pthreads programs were compiled using:

```bash
gcc program.c -o program -pthread
```

## 3. Architecture

The experiment follows a progressive multithreading architecture:

                    Multithreaded Program
                            |
             +--------------+--------------+
             |                             |
          Pthreads                       OpenMP
             |                             |
      Thread Creation               Parallel Region
             |                             |
      Multiple Threads              Work Sharing
             |                             |
       Work Distribution            Race Condition
             |                             |
      Race Condition                  Critical
             |                             |
          Mutex                       Barrier
             |                             |
             +-------------+-----------+
                           |
                    Performance Analysis
                           |
             +-------------+-------------+
             |             |             |
        Sequential      Pthreads      OpenMP
             |             |             |
             +-------------+-------------+
                           |
                  Execution Time
                           |
                       Speedup
                           |
                      Efficiency
                      
Thread Coordination Concept
Thread 1 ──┐
Thread 2 ──┤
Thread 3 ──┤── Synchronization ──> Continue
Thread 4 ──┘

Shared data is protected using synchronization mechanisms such as Pthread mutexes and OpenMP critical sections.

## 4. Execution
Part A — Pthreads

The following programs were implemented:

Program	Purpose
thread1.c	Create and execute one thread
thread2.c	Create multiple threads
thread_sum.c	Divide array-sum work among threads
race.c	Demonstrate a race condition
mutex.c	Fix the race condition using a mutex
pthread_perf.c	Measure Pthreads performance
Part B — OpenMP
Program	Purpose
omp1.c	Create an OpenMP parallel region
omp_sum.c	Work sharing and reduction
omp_race.c	Demonstrate an OpenMP race condition
omp_critical.c	Synchronize using a critical section
omp_barrier.c	Coordinate threads using a barrier
omp_perf.c	Measure OpenMP performance
Part C — Performance Analysis

The same computational workload was executed using:

Sequential execution
Pthreads
OpenMP

The parallel programs were tested using:

1, 2, 4, 6, 8, and 12 threads

The following measurements were collected:

Execution time
Speedup
Efficiency

## 5. Results
Sequential Baseline

The measured sequential execution time was:

1.987856 seconds
Execution Time Comparison
Threads	Pthreads (s)	OpenMP (s)
1	1.959201	2.015492
2	1.225233	1.101542
4	0.670307	0.645589
6	0.588161	0.621729
8	0.489643	0.558282
12	0.482019	0.407102
Speedup

Speedup was calculated using:

Speedup = Sequential Execution Time / Parallel Execution Time
Threads	Pthreads Speedup	OpenMP Speedup
1	1.015x	0.986x
2	1.622x	1.805x
4	2.966x	3.079x
6	3.380x	3.197x
8	4.060x	3.561x
12	4.124x	4.883x
Efficiency

Efficiency was calculated using:

Efficiency = (Speedup / Number of Threads) × 100
Threads	Pthreads Efficiency	OpenMP Efficiency
1	101.46%	98.63%
2	81.12%	90.23%
4	74.14%	76.98%
6	56.33%	53.29%
8	50.75%	44.51%
12	34.37%	40.69%
Performance Graphs
Execution Time vs Number of Threads

Speedup vs Number of Threads

Efficiency vs Number of Threads

Result Observation

The measured execution time generally decreased as the number of threads increased. The lowest measured execution time was obtained with 12 OpenMP threads, at 0.407102 seconds.

The results also show that increasing the number of threads does not produce perfectly proportional speedup. Thread management, scheduling, memory access, synchronization, and other system overheads affect parallel performance.

## 6. Conclusion

This experiment successfully demonstrated multithreaded programming using Pthreads and OpenMP.

Pthreads provided explicit control over thread creation, joining, work distribution, and mutex-based synchronization. OpenMP provided a higher-level approach using parallel regions, work-sharing constructs, critical sections, barriers, and reduction.

The race-condition experiments demonstrated that multiple threads accessing shared data without proper synchronization can produce incorrect results. Mutexes and critical sections were used to protect shared data and obtain the expected result.

The performance analysis showed that parallel execution can significantly reduce execution time for a suitable computational workload. Both Pthreads and OpenMP showed improved performance as the number of threads increased, although the speedup was not perfectly linear due to parallel execution overhead.

Overall, the experiment provided practical understanding of:

Thread Creation → Work Distribution → Race Conditions → Synchronization → Coordination → Performance Analysis

## 7. Author

Monika M. Bhandari
