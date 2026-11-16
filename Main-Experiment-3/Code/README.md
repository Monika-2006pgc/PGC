
/* ============================================================
   EXPERIMENT: ANALYZE SYNCHRONIZATION MECHANISMS
   Author: Monika M. Bhandari
   ============================================================ */


/* ===================== 1. race.c ===================== */

#include <stdio.h>
#include <omp.h>

int main() {
    long counter = 0;

    #pragma omp parallel for
    for (long i = 0; i < 10000000; i++) {
        counter++;
    }

    printf("Race Condition Result: %ld\n", counter);

    return 0;
}


/* ===================== 2. critical.c ===================== */

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

    printf("Critical Section Result: %ld\n", counter);

    return 0;
}


/* ===================== 3. atomic.c ===================== */

#include <stdio.h>
#include <omp.h>

int main() {
    long counter = 0;

    #pragma omp parallel for
    for (long i = 0; i < 10000000; i++) {
        #pragma omp atomic
        counter++;
    }

    printf("Atomic Operation Result: %ld\n", counter);

    return 0;
}


/* ===================== 4. barrier.c ===================== */

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


/* ===================== 5. semaphore.c ===================== */

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

    printf("Semaphore Result: %d\n", counter);

    sem_destroy(&semaphore);

    return 0;
}


/* ===================== 6. compare_sync.c ===================== */

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
                    long (*fn)(int),
                    int nthreads) {

    double start = omp_get_wtime();
    long result = fn(nthreads);
    double elapsed = omp_get_wtime() - start;

    const char *correctness =
        (result == N) ? "Correct" : "INCORRECT";

    printf("%-10s | %7d | %12ld | %10s | %10.4f\n",
           name, nthreads, result, correctness, elapsed);
}

int main() {

    int thread_counts[] = {1, 2, 4, 8};

    int num_configs =
        sizeof(thread_counts) / sizeof(thread_counts[0]);

    printf("Synchronization Mechanism Comparison\n\n");

    printf("Mechanism | Threads | Final Count | Correctness | Time(s)\n");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < num_configs; i++) {

        int t = thread_counts[i];

        run_and_report("None", run_none, t);
        run_and_report("Critical", run_critical, t);
        run_and_report("Atomic", run_atomic, t);
        run_and_report("Lock", run_lock, t);

        printf("---------------------------------------------------------------\n");
    }

    printf("\nExpected correct final count = %ld\n", N);

    return 0;
}
