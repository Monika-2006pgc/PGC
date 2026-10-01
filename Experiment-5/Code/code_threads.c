## 6. Codes

### Pthreads Programs

The following programs demonstrate thread creation, thread management, work distribution, race conditions, and synchronization using Pthreads.

- `thread1.c` – Creates and executes a single thread.
- `thread2.c` – Creates multiple threads.
- `thread_sum.c` – Demonstrates work distribution among threads.
- `race.c` – Demonstrates a race condition.
- `mutex.c` – Demonstrates synchronization using a mutex.

### OpenMP Programs

The following programs demonstrate parallel execution, work sharing, race conditions, critical sections, and barrier synchronization using OpenMP.

- `omp1.c` – Demonstrates an OpenMP parallel region.
- `omp_sum.c` – Demonstrates work sharing and reduction.
- `omp_race.c` – Demonstrates a race condition in OpenMP.
- `omp_critical.c` – Demonstrates the use of the OpenMP critical section.
- `omp_barrier.c` – Demonstrates barrier synchronization.

### Performance Programs

The following programs are used to compare sequential, Pthreads, and OpenMP execution performance.

- `sequential.c` – Sequential execution baseline.
- `pthread_perf.c` – Measures Pthreads execution time for different thread counts.
- `omp_perf.c` – Measures OpenMP execution time for different thread counts.
