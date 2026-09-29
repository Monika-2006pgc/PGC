# Experiment-2 - OpenMP Matrix Multiplication

## 1 Objective

To implement matrix multiplication using OpenMP and compare its execution performance with the sequential matrix multiplication program using multiple CPU threads.

## 2 Technologies Used

* WSL2 Ubuntu
* GCC Compiler
* OpenMP
* C Programming
* htop

## 3 Experimental Setup

* Matrix Size: 4000 × 4000
* OpenMP Threads: 8
* Execution Environment: WSL2 Ubuntu
* Parallelization: Outer loop of matrix multiplication
* Sequential Execution Time: 244.12 seconds

---

## 4 Execution Steps

### Step 1 - Start WSL Ubuntu

From Windows PowerShell:

```bash
wsl
```

---

### Step 2 - Check Available CPUs

```bash
nproc
```

Expected output:

```text
8
```

---

### Step 3 - Set OpenMP Threads

```bash
export OMP_NUM_THREADS=8
```

Verify the setting:

```bash
echo $OMP_NUM_THREADS
```

Expected output:

```text
8
```

---

### Step 4 - Create OpenMP Working Directory

```bash
mkdir -p ~/parallel_lab/openmp
cd ~/parallel_lab/openmp
```

---

### Step 5 - Create the Source File

```bash
nano matrix_openmp.c
```

The following OpenMP C program was used:

```c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 4000

int main()
{
    int i, j, k;
    double *A, *B, *C;
    double start, end;

    A = (double *)malloc(N * N * sizeof(double));
    B = (double *)malloc(N * N * sizeof(double));
    C = (double *)malloc(N * N * sizeof(double));

    if (A == NULL || B == NULL || C == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            A[i * N + j] = 1.0;
            B[i * N + j] = 1.0;
            C[i * N + j] = 0.0;
        }
    }

    start = omp_get_wtime();

    #pragma omp parallel for private(j, k)
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            for (k = 0; k < N; k++)
            {
                C[i * N + j] +=
                    A[i * N + k] *
                    B[k * N + j];
            }
        }
    }

    end = omp_get_wtime();

    printf("OpenMP Matrix Multiplication Completed\n");
    printf("Matrix Size = %d x %d\n", N, N);
    printf("Number of Threads Used = %d\n",
           omp_get_max_threads());
    printf("Execution Time = %f seconds\n",
           end - start);
    printf("Verification C[0][0] = %.2f\n", C[0]);

    free(A);
    free(B);
    free(C);

    return 0;
}
```

Save the file using:

```text
Ctrl + O
Enter
Ctrl + X
```

---

### Step 6 - Compile the Program

```bash
gcc -O2 -fopenmp matrix_openmp.c -o matrix_openmp
```

The `-fopenmp` option enables OpenMP support during compilation.

---

### Step 7 - Run the OpenMP Program

```bash
./matrix_openmp
```

Expected output format:

```text
OpenMP Matrix Multiplication Completed
Matrix Size = 4000 x 4000
Number of Threads Used = 8
Execution Time = XX.XXXXXX seconds
Verification C[0][0] = 4000.00
```

The execution time depends on the computer and system load.

---

### Step 8 - Monitor CPU Utilization

Open another Ubuntu terminal and run:

```bash
htop
```

This allows the CPU utilization of the OpenMP threads to be observed during execution.

If `htop` is not installed:

```bash
sudo apt update
sudo apt install htop
```

Then:

```bash
htop
```

---

## 5 Experimental Result

Reference execution result:

```text
OpenMP Matrix Multiplication Completed
Matrix Size = 4000 x 4000
Number of Threads Used = 8
Execution Time = 30.830434 seconds
Verification C[0][0] = 4000.00
```

### Performance Comparison

| Method     | Threads |    Execution Time |
| ---------- | ------: | ----------------: |
| Sequential |       1 |    244.12 seconds |
| OpenMP     |       8 | 30.830434 seconds |

### Speedup Calculation

```text
Speedup = Sequential Time / OpenMP Time

Speedup = 244.12 / 30.830434

Speedup ≈ 7.92×
```

---

## 6 Verification

The value of:

```text
C[0][0] = 4000.00
```

confirms that the matrix multiplication produced the expected result because every element of matrices A and B was initialized to `1.0`.

---

## 7 How OpenMP Works in This Experiment

OpenMP divides the outer `i` loop among multiple CPU threads using:

```c
#pragma omp parallel for
```

With 8 threads, different rows of the result matrix can be processed concurrently. All threads share the matrices stored in the common memory of the system.

Therefore, the computation is performed in parallel instead of using only one CPU thread.

---


## 8 Conclusion

The OpenMP implementation successfully performed matrix multiplication using 8 CPU threads. The reference execution time was 30.830434 seconds compared with 244.12 seconds for the sequential implementation, resulting in an approximately 7.92× speedup. This demonstrates how OpenMP can improve the performance of computationally intensive tasks by using multiple threads on a shared-memory system.

