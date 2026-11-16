# Experiment: Analyze Synchronization Mechanisms in Parallel Programs

## Experiment Overview

This experiment demonstrates the implementation and analysis of synchronization mechanisms used in shared-memory parallel programs. It focuses on understanding race conditions, critical sections, atomic operations, barriers, semaphores, and locks.

The experiment compares different synchronization techniques based on correctness, execution time, and scalability as the number of threads increases.

The experiment was performed using Ubuntu 22.04 LTS on VMware Workstation Pro with GCC, OpenMP, and POSIX Threads (Pthreads).

## 1. Objectives

The main objectives of this experiment are:

- To understand the need for synchronization in parallel programming.
- To demonstrate race conditions in shared-memory programs.
- To implement critical sections using OpenMP.
- To understand atomic operations and their advantages.
- To implement barriers for thread coordination.
- To implement semaphores using POSIX Threads.
- To understand mutual exclusion using locks.
- To compare different synchronization mechanisms.
- To analyze execution time and scalability with increasing thread counts.
- To understand the trade-off between correctness and performance.

## 2. Configuration

### Hardware
- System: Dell Inspiron 15 3520
- RAM: 8 GB
- Processor: Intel Core i5-1235U
- Logical CPUs: 12

### Software
- Host Operating System: Windows 11
- Virtualization Software: VMware Workstation Pro
- Guest Operating System: Ubuntu 22.04 LTS
- Compiler: GCC
- Programming Language: C

### Parallel Programming Libraries
- OpenMP
- POSIX Threads (Pthreads)
- POSIX Semaphore Library

### Editor and Terminal
- Editor: Nano
- Terminal: Ubuntu Terminal

### Compilation Commands

OpenMP programs:

```bash
gcc program.c -fopenmp -o program
```

Pthreads and Semaphore programs:

```bash
gcc program.c -pthread -o program
```

## 3. Architecture

The experiment follows a synchronization-based architecture:

```text
             Parallel Program
                    |
          Shared Memory Access
                    |
          +---------+---------+
          |                   |
     Multiple Threads     Shared Counter
          |                   |
          +---------+---------+
                    |
           Synchronization
                    |
    +-------+-------+-------+-------+
    |       |       |       |       |
   Race  Critical Atomic Barrier Semaphore
   Condition Section Operation        |
    |       |       |       |       |
    |       |       |       |      Lock
    |       |       |       |
    +-------+-------+-------+
                    |
           Correctness Analysis
                    |
           Performance Analysis
                    |
          Scalability Comparison
```

### Thread Coordination Concept

```text
Thread 1 ----+
Thread 2 ----+
Thread 3 ----+----> Synchronization ----> Continue
Thread 4 ----+
```

Synchronization mechanisms coordinate access to shared resources and help prevent incorrect results caused by simultaneous thread execution.

### Repository Structure

```text
Synchronization-Mechanisms/
│
├── README.md
│
├── Programs/
│   ├── race.c
│   ├── critical.c
│   ├── atomic.c
│   ├── barrier.c
│   ├── semaphore.c
│   └── compare_sync.c
│
├── Screenshots/
│   ├── race_output.png
│   ├── critical_output.png
│   ├── atomic_output.png
│   ├── barrier_output.png
│   ├── semaphore_output.png
│   └── comparison_output.png
│
└── Graphs/
    └── performance_comparison.png
```

## 4. Execution

### Part A — Race Condition

**Program:** `race.c`

- Demonstrates simultaneous access to a shared counter.
- Multiple threads increment the same variable without synchronization.
- Lost updates may occur because of concurrent read-modify-write operations.

Compilation:

```bash
gcc race.c -fopenmp -o race
```

Execution:

```bash
./race
```

### Part B — Critical Section

**Program:** `critical.c`

- Uses `#pragma omp critical`.
- Allows only one thread at a time to execute the protected section.
- Prevents simultaneous modification of shared data.

Compilation:

```bash
gcc critical.c -fopenmp -o critical
```

### Part C — Atomic Operation

**Program:** `atomic.c`

- Uses `#pragma omp atomic`.
- Performs shared counter updates as indivisible operations.
- Provides synchronization with relatively low overhead for simple updates.

Compilation:

```bash
gcc atomic.c -fopenmp -o atomic
```

### Part D — Barrier Synchronization

**Program:** `barrier.c`

- Uses `#pragma omp barrier`.
- Ensures all threads reach a synchronization point before proceeding.
- Demonstrates coordination between different execution phases.

Compilation:

```bash
gcc barrier.c -fopenmp -o barrier
```

### Part E — Semaphore

**Program:** `semaphore.c`

- Uses POSIX semaphore functions.
- Implements mutual exclusion using a binary semaphore.
- Uses `sem_wait()` and `sem_post()` to control access to shared data.

Compilation:

```bash
gcc semaphore.c -pthread -o semaphore
```

### Part F — Performance Comparison

**Program:** `compare_sync.c`

This program compares four synchronization strategies:

1. No Synchronization
2. Critical Section
3. Atomic Operation
4. OpenMP Lock

The same workload of 10,000,000 shared counter increments is executed using different thread counts.

Thread configurations:

- 1 Thread
- 2 Threads
- 4 Threads
- 8 Threads

The following parameters are measured:

- Final Counter Value
- Correctness
- Execution Time
- Scalability

Compilation:

```bash
gcc compare_sync.c -fopenmp -o compare_sync
```

Execution:

```bash
./compare_sync
```

## 5. Results

### Sample Output

The comparison program produced the following representative results.

| Mechanism | Threads | Final Count | Correctness | Time (s) |
|---|---:|---:|---|---:|
| None | 1 | 10000000 | Correct | 0.0486 |
| Critical | 1 | 10000000 | Correct | 0.1571 |
| Atomic | 1 | 10000000 | Correct | 0.0567 |
| Lock | 1 | 10000000 | Correct | 0.1586 |
| None | 2 | 10000000 | Correct | 0.0249 |
| Critical | 2 | 10000000 | Correct | 0.1584 |
| Atomic | 2 | 10000000 | Correct | 0.0574 |
| Lock | 2 | 10000000 | Correct | 0.1590 |
| None | 4 | 7545062 | Incorrect | 0.0250 |
| Critical | 4 | 10000000 | Correct | 0.1583 |
| Atomic | 4 | 10000000 | Correct | 0.0585 |
| Lock | 4 | 10000000 | Correct | 0.1615 |
| None | 8 | 4394535 | Incorrect | 0.0250 |
| Critical | 8 | 10000000 | Correct | 0.1579 |
| Atomic | 8 | 10000000 | Correct | 0.0565 |
| Lock | 8 | 10000000 | Correct | 0.1603 |

*Note: These are representative sample measurements from the experiment document. Actual execution times and race-condition results may vary depending on the system and thread scheduling.*

### Observation / Comparison Table

| Mechanism | Correctness | Relative Performance | Scalability |
|---|---|---|---|
| No Synchronization | Incorrect with race conditions | Fastest | Poor |
| Critical Section | Correct | Slow | Poor |
| Atomic Operation | Correct | Fast | Good |
| OpenMP Lock | Correct | Slow | Poor to Moderate |
| Barrier | Synchronization only | Waiting overhead | Moderate |
| Semaphore | Correct | Moderate overhead | Moderate |

### Result Observation

- Without synchronization, multiple threads may update shared data simultaneously, resulting in incorrect counter values.
- Critical sections guarantee correctness but introduce waiting overhead.
- Atomic operations provide correct updates with relatively low overhead.
- Locks provide mutual exclusion but may reduce scalability.
- Barriers coordinate threads by ensuring all threads reach a common synchronization point.
- Semaphores control access to shared resources using counter-based synchronization.

The comparison demonstrates that atomic operations provide a good balance between correctness and performance for simple shared-variable updates.

## 6. Conclusion

This experiment successfully demonstrated different synchronization mechanisms used in parallel programming.

The race-condition program showed how simultaneous access to shared data can produce incorrect results. Critical sections, atomic operations, locks, and semaphores demonstrated different approaches to protecting shared resources and ensuring correctness.

The barrier experiment demonstrated how threads can be coordinated during different phases of parallel execution.

The performance comparison showed that synchronization mechanisms have different execution overheads and scalability characteristics. Atomic operations provided relatively better performance for simple shared counter updates, while critical sections and locks introduced additional waiting overhead.

Overall, the experiment provided practical understanding of:

**Race Conditions → Critical Sections → Atomic Operations → Barriers → Semaphores → Locks → Performance Analysis**

## 7. Author

**Monika M. Bhandari**  
B.E. Computer Science and Artificial Intelligence Engineering  
KLE Technological University, Hubballi

---

*Experiment: Analyze Synchronization Mechanisms in Parallel Programs*  
*Course: Parallel Programming / Cloud Computing Lab*
