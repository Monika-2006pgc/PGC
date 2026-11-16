# Experiment: Analyze Cache Coherence (MSI, MESI, MOESI) and NUMA Memory Access Performance

## 1. Objective

To study how cache coherence protocols (MSI, MESI, and MOESI) maintain consistency between the caches of a multi-core processor and to evaluate the performance effects of false sharing and NUMA memory placement.

The experiment uses two threads, each incrementing its own counter. The counters are placed either on the same cache line (adjacent layout) or on separate cache lines (padded layout).

The experiment measures correctness, execution time, and scalability as the number of threads increases.

## 2. Software Requirements

| Sl. No. | Software / Tool | Purpose |
|---|---|---|
| 1 | VMware Workstation Pro | Virtualization software |
| 2 | Ubuntu 22.04 LTS | Operating system |
| 3 | GCC Compiler | Compiling C programs |
| 4 | OpenMP | Parallel programming and timing |
| 5 | numactl | NUMA memory and CPU binding |
| 6 | hwloc | Displaying NUMA topology |
| 7 | Python 3 | Running the cache coherence simulator |

## 3. Theory

### 3.1 Cache Coherence

In a multi-core processor, each core has its own private cache. When multiple cores access the same memory location, they may have separate copies of the data.

Cache coherence ensures that all cores see a consistent value for shared memory locations.

### 3.2 False Sharing

False sharing occurs when two threads modify different variables that are located on the same cache line.

Even though the threads access different variables, the entire cache line moves between cores, causing unnecessary coherence traffic and reducing performance.

**Solution:** Padding or alignment can place each thread's counter on a separate cache line.

### 3.3 MSI Protocol

MSI has three cache states:

- **M – Modified:** The cache contains the only valid copy, and it is dirty.
- **S – Shared:** One or more caches contain a clean copy.
- **I – Invalid:** The cache copy is not valid.

### 3.4 MESI Protocol

MESI adds the Exclusive (E) state to MSI.

- **E – Exclusive:** The cache has the only clean copy.
- A write can change the state from E to M without requiring a bus transaction.

### 3.5 MOESI Protocol

MOESI adds the Owned (O) state.

- **O – Owned:** A cache holds a dirty line that can also be shared with other caches.
- The owner can supply data directly to another cache without first writing it back to memory.

### 3.6 NUMA

NUMA stands for Non-Uniform Memory Access.

In NUMA systems, local memory access is generally faster than remote memory access because remote access must travel through an interconnect.

The `numactl` command is used to control CPU and memory placement.

### 3.7 Correctness, Performance and Scalability

- **Correctness:** Whether the program produces the expected result.
- **Performance:** How much time the program takes to execute.
- **Scalability:** How performance changes when the number of threads increases.

## 4. Programs Implemented

The experiment consists of four main programs.

### Program 1: Cache Coherence Protocol Simulator

**File:** `coherence_sim.py`

- Simulates two processor cores.
- Compares MSI, MESI and MOESI protocols.
- Displays cache states after memory operations.
- Counts bus transactions, invalidations, write-backs and cache-to-cache transfers.
- Compares adjacent and padded memory layouts.

**Execution:**

```bash
python3 coherence_sim.py 1000
```

### Program 2: False Sharing (Adjacent vs Padded)

**File:** `false_sharing.c`

- Uses OpenMP to execute parallel counter increments.
- Compares adjacent and padded counter layouts.
- Measures execution time.
- Demonstrates the performance impact of false sharing.

**Compilation:**

```bash
gcc -O2 false_sharing.c -fopenmp -o false_sharing
```

**Execution:**

```bash
OMP_NUM_THREADS=2 ./false_sharing adjacent
OMP_NUM_THREADS=2 ./false_sharing padded
```

### Program 3: NUMA Memory Sweep

**File:** `numa_sweep.c`

- Allocates a large memory buffer.
- Performs memory access across multiple passes.
- Measures memory access execution time.
- Compares local and remote NUMA memory placement.

**Compilation:**

```bash
gcc -O2 numa_sweep.c -fopenmp -o numa_sweep
```

**Execution:**

```bash
OMP_NUM_THREADS=2 numactl --cpunodebind=0 --membind=0 ./numa_sweep
OMP_NUM_THREADS=2 numactl --cpunodebind=0 --membind=1 ./numa_sweep
```

### Program 4: Comparison of Scalability and Execution Time

**File:** `compare_numa.c`

- Compares adjacent and padded counter layouts.
- Tests 1, 2 and 4 threads.
- Checks the final counter value.
- Measures execution time.
- Performs a 512 MB memory sweep.
- Generates comparison tables for performance analysis.

**Compilation:**

```bash
gcc -O2 compare_numa.c -fopenmp -o compare_numa
```

**Execution:**

```bash
./compare_numa
```

## 5. Execution Steps

1. Install GCC, OpenMP, numactl, hwloc and Python 3.
2. Check CPU and NUMA node information.
3. Create the Python and C source files.
4. Run the cache coherence simulator.
5. Compile and execute the false sharing program.
6. Compile and execute the NUMA memory sweep program.
7. Compile and execute the comparison program.
8. Record execution times for different configurations.
9. Compare the results and analyze performance.

## 6. Sample Output and Results

### 6.1 Cache Coherence Protocol Simulation

The simulator compares the cache states and memory transactions for MSI, MESI and MOESI.

| Layout | Protocol | Bus Transactions | Write-backs | Cache-to-Cache Transfers |
|---|---|---:|---:|---:|
| Adjacent | MSI | 4000 | 1999 | 0 |
| Adjacent | MESI | 3999 | 1999 | 0 |
| Adjacent | MOESI | 3999 | 0 | 1999 |
| Padded | MSI | 4 | 0 | 0 |
| Padded | MESI | 2 | 0 | 0 |
| Padded | MOESI | 2 | 0 | 0 |

*Simulator results for 1000 iterations, as shown in the experiment.*

### 6.2 False Sharing Comparison

The program checks the final counter value for adjacent and padded layouts using 1, 2 and 4 threads.

Expected total count: `100000000`

| Layout | Threads | Expected Result |
|---|---:|---|
| Adjacent | 1 | Correct |
| Padded | 1 | Correct |
| Adjacent | 2 | Correct |
| Padded | 2 | Correct |
| Adjacent | 4 | Correct |
| Padded | 4 | Correct |

### 6.3 NUMA Memory Sweep

The memory sweep uses a 512 MB buffer and performs four passes.

It compares execution time using 1 and 2 threads under different memory placement configurations.

Actual execution times depend on the hardware and NUMA configuration.

## 7. Observation and Comparison

| Case | Correctness | Performance | Scalability |
|---|---|---|---|
| MSI | Correct | More bus transactions | Moderate |
| MESI | Correct | Reduces transactions for private data | Good |
| MOESI | Correct | Avoids forced write-back in certain cases | Good |
| Adjacent Counters | Correct | Usually slower due to false sharing | Poor |
| Padded Counters | Correct | Usually faster | Good |
| Local Memory | Correct | Reference memory access time | Reference |
| Remote Memory | Correct | Usually slower on multi-socket systems | May reduce scalability |

*Typical performance observations from the experiment; actual timings depend on the machine.*

## 8. Important Commands

```bash
# Check GCC version
gcc --version

# Check OpenMP support
echo '#include <omp.h>' | gcc -E -fopenmp -xc - > /dev/null && echo OpenMP OK

# Check CPU and NUMA information
lscpu
numactl --hardware

# Set the number of OpenMP threads
export OMP_NUM_THREADS=4

# Run the simulator
python3 coherence_sim.py 1000
```

## 9. Conclusion

The experiment demonstrates how cache coherence protocols maintain consistent data across multiple processor cores.

MSI, MESI and MOESI use different cache states and communication mechanisms. MESI reduces unnecessary bus transactions for private data, while MOESI reduces memory write-back traffic by allowing cache-to-cache data transfers.

The false sharing experiment shows that placing multiple counters on the same cache line can reduce performance. Padding the counters onto separate cache lines helps improve scalability.

The NUMA experiment demonstrates that local memory access is generally faster than remote memory access on multi-socket systems.

Therefore, proper cache-line alignment, synchronization and NUMA memory placement are important for improving the performance of parallel programs.

## 10. Author

**Name:** Monika M. Bhandari  
**Branch:** Computer Science and Artificial Intelligence Engineering  
**Institution:** KLE Technological University, Hubballi

