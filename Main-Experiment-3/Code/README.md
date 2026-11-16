
##  Implementation – Source Code

This experiment consists of six C programs to demonstrate different synchronization mechanisms and their performance.

### 1 Race Condition (race.c)

Demonstrates multiple threads accessing and modifying a shared counter without synchronization.

```c
#include <stdio.h>
#include <omp.h>

int main() {
    long counter = 0;

    #pragma omp parallel for
    for (long i = 0; i < 10000000; i++) {
        counter++;
    }

    printf("Counter = %ld\n", counter);
    return 0;
}
```

### 2 Critical Section (critical.c)

Uses the OpenMP critical directive to ensure that only one thread accesses the shared counter at a time.

```c
#include <stdio.h>
#include <omp.h>

int main() {
    long counter = 0;

    #pragma omp parallel for
    for (long i = 0; i < 10000000; i++) {
        #pragma omp critical
        {
            counter++;
        }
    }

    printf("Counter = %ld\n", counter);
    return 0;
}
```

### 3 Atomic Operation (atomic.c)

Uses the OpenMP atomic directive to perform safe and indivisible updates to a shared counter.

```c
#include <stdio.h>
#include <omp.h>

int main() {
    long counter = 0;

    #pragma omp parallel for
    for (long i = 0; i < 10000000; i++) {
        #pragma omp atomic
        counter++;
    }

    printf("Counter = %ld\n", counter);
    return 0;
}
```

### 4 Barrier Synchronization (barrier.c)

Demonstrates synchronization between threads using an OpenMP barrier.

```c
#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        printf("Thread %d before barrier\n",
               omp_get_thread_num());

        #pragma omp barrier

        printf("Thread %d after barrier\n",
               omp_get_thread_num());
    }

    return 0;
}
```

### 5 Semaphore Synchronization (semaphore.c)

Uses POSIX semaphores to control access to shared resources and prevent simultaneous modification of shared data.

```c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaphore;
int counter = 0;

void* work(void* arg) {
    for (int i = 0; i < 100000; i++) {
        sem_wait(&semaphore);
        counter++;
        sem_post(&semaphore);
    }

    return NULL;
}

int main() {
    pthread_t t1, t2;

    sem_init(&semaphore, 0, 1);

    pthread_create(&t1, NULL, work, NULL);
    pthread_create(&t2, NULL, work, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Counter = %d\n", counter);

    sem_destroy(&semaphore);
    return 0;
}
```

### 6 Performance Comparison (compare_sync.c)

Compares four synchronization strategies: No Synchronization, Critical Section, Atomic Operation, and OpenMP Lock. Execution time and correctness are measured using different thread counts.

```c
#include <stdio.h>
#include <omp.h>

#define N 10000000L

long run_none(int nthreads) {
    long counter = 0;
    omp_set_num_threads(nthreads);

    #pragma omp parallel for
    for (long i = 0; i < N; i++) {
        counter++;
    }

    return counter;
}

long run_critical(int nthreads) {
    long counter = 0;
    omp_set_num_threads(nthreads);

    #pragma omp parallel for
    for (long i = 0; i < N; i++) {
        #pragma omp critical
        {
            counter++;
        }
    }

    return counter;
}

long run_atomic(int nthreads) {
    long counter = 0;
    omp_set_num_threads(nthreads);

    #pragma omp parallel for
    for (long i = 0; i < N; i++) {
        #pragma omp atomic
        counter++;
    }

    return counter;
}

long run_lock(int nthreads) {
    long counter = 0;
    omp_lock_t lock;

    omp_init_lock(&lock);
    omp_set_num_threads(nthreads);

    #pragma omp parallel for
    for (long i = 0; i < N; i++) {
        omp_set_lock(&lock);
        counter++;
        omp_unset_lock(&lock);
    }

    omp_destroy_lock(&lock);
    return counter;
}

void run_and_report(const char *name,
                    long (*fn)(int), int nthreads) {
    double start = omp_get_wtime();
    long result = fn(nthreads);
    double elapsed = omp_get_wtime() - start;

    printf("%-10s | %d | %ld | %s | %.4f seconds\n",
           name, nthreads, result,
           result == N ? "Correct" : "INCORRECT",
           elapsed);
}

int main() {
    int thread_counts[] = {1, 2, 4, 8};

    printf("Synchronization Performance Comparison\n");

    for (int i = 0; i < 4; i++) {
        int t = thread_counts[i];

        printf("\nThreads: %d\n", t);

        run_and_report("None", run_none, t);
        run_and_report("Critical", run_critical, t);
        run_and_report("Atomic", run_atomic, t);
        run_and_report("Lock", run_lock, t);
    }

    printf("\nExpected Count = %ld\n", N);

    return 0;
}
```

