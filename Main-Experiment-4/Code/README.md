## Code Implementation

## Program 1: Cache Coherence Protocol Simulator

**File: coherence_sim.py**

```python
class Cache:
    def __init__(self):
        self.state = "I"


def simulate(protocol, layout, iterations):
    cache0 = Cache()
    cache1 = Cache()

    transactions = 0
    invalidations = 0
    writebacks = 0
    transfers = 0

    for i in range(iterations):
        # Core 0 writes to its variable
        if cache0.state == "I":
            transactions += 1
            cache0.state = "M"

        # Core 1 accesses memory
        if layout == "adjacent":
            if cache1.state == "I":
                transactions += 1

                if cache0.state == "M":
                    if protocol == "MOESI":
                        cache0.state = "O"
                        cache1.state = "S"
                        transfers += 1
                    else:
                        cache0.state = "S"
                        cache1.state = "S"
                        writebacks += 1

                else:
                    cache1.state = "S"

        else:
            if cache1.state == "I":
                cache1.state = "M"
                transactions += 1

    print("\nProtocol:", protocol)
    print("Layout:", layout)
    print("Bus Transactions:", transactions)
    print("Invalidations:", invalidations)
    print("Write-backs:", writebacks)
    print("Cache-to-Cache Transfers:", transfers)


protocols = ["MSI", "MESI", "MOESI"]
layouts = ["adjacent", "padded"]

for protocol in protocols:
    for layout in layouts:
        simulate(protocol, layout, 1000)
```

**Execution:**

```bash
python3 coherence_sim.py
```

## Program 2: False Sharing (Adjacent vs Padded)

**File: false_sharing.c**

```c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100000000

typedef struct {
    long counter;
} Adjacent;

typedef struct {
    long counter;
    char padding[64];
} Padded;

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: ./false_sharing adjacent/padded\n");
        return 1;
    }

    double start, end;

    if (argv[1][0] == 'a') {

        Adjacent counters[2] = {{0}, {0}};

        start = omp_get_wtime();

        #pragma omp parallel num_threads(2)
        {
            int id = omp_get_thread_num();

            for (long i = 0; i < N / 2; i++) {
                counters[id].counter++;
            }
        }

        end = omp_get_wtime();

        printf("Layout: Adjacent\n");
        printf("Final Count: %ld\n",
               counters[0].counter + counters[1].counter);
        printf("Execution Time: %f seconds\n", end - start);
    }

    else {

        Padded counters[2] = {0};

        start = omp_get_wtime();

        #pragma omp parallel num_threads(2)
        {
            int id = omp_get_thread_num();

            for (long i = 0; i < N / 2; i++) {
                counters[id].counter++;
            }
        }

        end = omp_get_wtime();

        printf("Layout: Padded\n");
        printf("Final Count: %ld\n",
               counters[0].counter + counters[1].counter);
        printf("Execution Time: %f seconds\n", end - start);
    }

    return 0;
}
```

**Compilation:**

```bash
gcc -O2 false_sharing.c -fopenmp -o false_sharing
```

**Execution:**

```bash
OMP_NUM_THREADS=2 ./false_sharing adjacent
OMP_NUM_THREADS=2 ./false_sharing padded
```

## Program 3: NUMA Memory Sweep

**File: numa_sweep.c**

```c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE (512 * 1024 * 1024)
#define PASSES 4

int main() {

    char *memory = (char *)malloc(SIZE);

    if (memory == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    double start, end;

    printf("NUMA Memory Sweep Experiment\n");
    printf("Memory Size: 512 MB\n");
    printf("Number of Passes: %d\n", PASSES);

    #pragma omp parallel for
    for (long i = 0; i < SIZE; i++) {
        memory[i] = 1;
    }

    start = omp_get_wtime();

    for (int pass = 0; pass < PASSES; pass++) {

        #pragma omp parallel for
        for (long i = 0; i < SIZE; i += 4096) {
            memory[i]++;
        }
    }

    end = omp_get_wtime();

    printf("Execution Time: %f seconds\n", end - start);

    free(memory);

    return 0;
}
```

**Compilation:**

```bash
gcc -O2 numa_sweep.c -fopenmp -o numa_sweep
```

**Execution:**

```bash
OMP_NUM_THREADS=2 numactl --cpunodebind=0 --membind=0 ./numa_sweep

OMP_NUM_THREADS=2 numactl --cpunodebind=0 --membind=1 ./numa_sweep
```

## Program 4: Comparison of Scalability and Execution Time

**File: compare_numa.c**

```c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10000000

int main() {

    int thread_counts[] = {1, 2, 4};
    int total_threads = 3;

    printf("\nCACHE COHERENCE PERFORMANCE COMPARISON\n");

    printf("-----------------------------------------------\n");
    printf("Layout\tThreads\tFinal Count\tTime\n");
    printf("-----------------------------------------------\n");

    for (int t = 0; t < total_threads; t++) {

        int threads = thread_counts[t];

        long adjacent[8] = {0};
        long padded[8][8] = {{0}};

        double start, end;

        // Adjacent counter experiment
        start = omp_get_wtime();

        #pragma omp parallel num_threads(threads)
        {
            int id = omp_get_thread_num();

            for (long i = 0; i < N / threads; i++) {
                adjacent[id]++;
            }
        }

        end = omp_get_wtime();

        long adjacent_total = 0;

        for (int i = 0; i < threads; i++) {
            adjacent_total += adjacent[i];
        }

        printf("Adjacent\t%d\t%ld\t%f\n",
               threads, adjacent_total, end - start);

        // Padded counter experiment
        start = omp_get_wtime();

        #pragma omp parallel num_threads(threads)
        {
            int id = omp_get_thread_num();

            for (long i = 0; i < N / threads; i++) {
                padded[id][0]++;
            }
        }

        end = omp_get_wtime();

        long padded_total = 0;

        for (int i = 0; i < threads; i++) {
            padded_total += padded[i][0];
        }

        printf("Padded\t\t%d\t%ld\t%f\n",
               threads, padded_total, end - start);

        printf("-----------------------------------------------\n");
    }

    return 0;
}
```

**Compilation:**

```bash
gcc -O2 compare_numa.c -fopenmp -o compare_numa
```

**Execution:**

```bash
./compare_numa
```

## Complete Compilation Commands

```bash
gcc -O2 false_sharing.c -fopenmp -o false_sharing

gcc -O2 numa_sweep.c -fopenmp -o numa_sweep

gcc -O2 compare_numa.c -fopenmp -o compare_numa
```

## Conclusion

The programs demonstrate cache coherence protocols, false sharing, and NUMA memory access performance. The experiments compare adjacent and padded memory layouts and measure execution time for different thread counts.


