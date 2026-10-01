# Experiment 3 - MPI Distributed Matrix Multiplication

MPI (Message Passing Interface) uses multiple independent processes. In this experiment, one Master VM and three Worker VMs are connected through the same virtual network. Each process has its own memory space, so data is explicitly communicated between processes.

## 1 Objective

To implement distributed matrix multiplication using MPI and execute the computation across one Master VM and three Worker VMs.

## 2 Technologies Used

* VMware Workstation
* Ubuntu
* Open MPI
* OpenSSH
* C Programming
* MPI

## 3 Experimental Setup

* Matrix Size: 4000 × 4000
* Number of MPI Processes: 4
* Master VM: `master`
* Worker VMs: `worker1`, `worker2`, `worker3`
* MPI Network: Same virtual network
* Matrix Distribution: 1000 rows per MPI process
* Sequential Execution Time: 244.12 seconds

### VM Configuration

| Node     | Hostname | MPI Role |
| -------- | -------- | -------- |
| Master   | master   | Rank 0   |
| Worker 1 | worker1  | Rank 1   |
| Worker 2 | worker2  | Rank 2   |
| Worker 3 | worker3  | Rank 3   |

---

## 4 Execution Steps

### Step 1 - Create Four Ubuntu VMs

Create four Ubuntu virtual machines in VMware Workstation:

```text
master
worker1
worker2
worker3
```

Connect all four VMs to the same VMware virtual network.

MPI processes running on different VMs need network connectivity to communicate.

---

### Step 2 - Set Hostnames

On the Master VM:

```bash
sudo hostnamectl set-hostname master
```

On Worker 1:

```bash
sudo hostnamectl set-hostname worker1
```

On Worker 2:

```bash
sudo hostnamectl set-hostname worker2
```

On Worker 3:

```bash
sudo hostnamectl set-hostname worker3
```

Verify the hostname on each VM:

```bash
hostname
```

---

### Step 3 - Find IP Addresses

Run this command on each VM:

```bash
hostname -I
```

Record the IP address of every VM.

---

### Step 4 - Test Network Connectivity

Run these commands from the Master VM:

```bash
ping -c 4 <worker1-IP>
ping -c 4 <worker2-IP>
ping -c 4 <worker3-IP>
```

Expected result:

```text
4 packets transmitted, 4 received, 0% packet loss
```

This confirms that the Master can communicate with the Worker VMs.

---

### Step 5 - Install OpenSSH

Run on **all four VMs**:

```bash
sudo apt update
sudo apt install openssh-server -y
sudo systemctl enable --now ssh
```

Verify SSH service:

```bash
systemctl status ssh
```

---

### Step 6 - Install Open MPI

Run on **all four VMs**:

```bash
sudo apt update
sudo apt install openmpi-bin libopenmpi-dev -y
```

Verify MPI installation:

```bash
mpicc --version
mpirun --version
```

---

### Step 7 - Create SSH Key on Master

Run only on the Master VM:

```bash
ssh-keygen -t rsa
```

Press **Enter** to accept the default file location.

---

### Step 8 - Copy SSH Key to Workers

From the Master VM:

```bash
ssh-copy-id worker1
ssh-copy-id worker2
ssh-copy-id worker3
```

This allows the Master to connect to the Workers without repeatedly entering the password.

---

### Step 9 - Test Passwordless SSH

From the Master VM:

```bash
ssh worker1 hostname
ssh worker2 hostname
ssh worker3 hostname
```

Expected output:

```text
worker1
worker2
worker3
```

---

### Step 10 - Create MPI Working Directory

On the Master VM:

```bash
mkdir -p ~/parallel_lab/mpi
cd ~/parallel_lab/mpi
```

---

### Step 11 - Create MPI Hostfile

Create the hostfile:

```bash
nano hosts
```

Enter:

```text
master slots=1
worker1 slots=1
worker2 slots=1
worker3 slots=1
```

Save:

```text
Ctrl + O
Enter
Ctrl + X
```

The hostfile tells `mpirun` which machines participate in the MPI execution.

---

## 5 MPI Program

Create the MPI source file:

```bash
nano matrix_mpi.c
```

program:

```c
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>

#define N 4000

int main(int argc, char *argv[])
{
    int rank, size;
    int i, j, k;
    int rows_per_process;
    char hostname[256];

    double *A = NULL;
    double *B = NULL;
    double *C = NULL;
    double *local_A;
    double *local_C;
    double start, end;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    gethostname(hostname, sizeof(hostname));

    if (N % size != 0)
    {
        if (rank == 0)
            printf("Matrix size must be divisible by number of processes.\n");

        MPI_Finalize();
        return 0;
    }

    rows_per_process = N / size;

    local_A = (double *)malloc(
        rows_per_process * N * sizeof(double));

    local_C = (double *)malloc(
        rows_per_process * N * sizeof(double));

    B = (double *)malloc(
        N * N * sizeof(double));

    if (rank == 0)
    {
        A = (double *)malloc(N * N * sizeof(double));
        C = (double *)malloc(N * N * sizeof(double));

        printf("Initializing %d x %d matrices...\n", N, N);

        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                A[i * N + j] = 1.0;
                B[i * N + j] = 1.0;
                C[i * N + j] = 0.0;
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    start = MPI_Wtime();

    MPI_Scatter(
        A,
        rows_per_process * N,
        MPI_DOUBLE,
        local_A,
        rows_per_process * N,
        MPI_DOUBLE,
        0,
        MPI_COMM_WORLD);

    MPI_Bcast(
        B,
        N * N,
        MPI_DOUBLE,
        0,
        MPI_COMM_WORLD);

    printf("Rank %d on %s computing %d rows\n",
           rank, hostname, rows_per_process);

    for (i = 0; i < rows_per_process; i++)
    {
        for (j = 0; j < N; j++)
        {
            local_C[i * N + j] = 0.0;

            for (k = 0; k < N; k++)
            {
                local_C[i * N + j] +=
                    local_A[i * N + k] *
                    B[k * N + j];
            }
        }
    }

    MPI_Gather(
        local_C,
        rows_per_process * N,
        MPI_DOUBLE,
        C,
        rows_per_process * N,
        MPI_DOUBLE,
        0,
        MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    end = MPI_Wtime();

    if (rank == 0)
    {
        printf("\nMPI Matrix Multiplication Completed\n");
        printf("Matrix Size = %d x %d\n", N, N);
        printf("Number of MPI Processes = %d\n", size);
        printf("Execution Time = %f seconds\n", end - start);
        printf("Verification C[0][0] = %.2f\n", C[0]);

        free(A);
        free(C);
    }

    free(B);
    free(local_A);
    free(local_C);

    MPI_Finalize();

    return 0;
}
```

Save the file:

```text
Ctrl + O
Enter
Ctrl + X
```

---

## 6 Compile the MPI Program

On the Master VM:

```bash
mpicc -O2 matrix_mpi.c -o matrix_mpi
```

`mpicc` compiles the C program and links it with the MPI libraries.

---

## 7 Copy the Executable to Workers

From the Master VM:

```bash
scp matrix_mpi worker1:~/matrix_mpi
scp matrix_mpi worker2:~/matrix_mpi
scp matrix_mpi worker3:~/matrix_mpi
```

This ensures that every Worker has access to the MPI executable.

---

## 8 Run the MPI Program

From the Master VM:

```bash
mpirun -np 4 --hostfile hosts sh -c '$HOME/matrix_mpi'
```

The four MPI processes are distributed across:

```text
Rank 0 → Master
Rank 1 → Worker1
Rank 2 → Worker2
Rank 3 → Worker3
```

Each process computes:

```text
1000 rows
```

Expected output format:

```text
Rank 0 on master computing 1000 rows
Rank 1 on worker1 computing 1000 rows
Rank 2 on worker2 computing 1000 rows
Rank 3 on worker3 computing 1000 rows

MPI Matrix Multiplication Completed
Matrix Size = 4000 x 4000
Number of MPI Processes = 4
Execution Time = XX.XXXXXX seconds
Verification C[0][0] = 4000.00
```

The execution time may be different on different systems.

---

## 9 MPI Data Flow

```text
              Matrix A
           4000 × 4000
                 |
                 |
          MPI_Scatter
                 |
       +---------+---------+
       |         |         |
     Rank 0    Rank 1    Rank 2    Rank 3
    1000 rows 1000 rows 1000 rows 1000 rows
       |         |         |         |
       +---------+---------+---------+
                 |
              Computation
                 |
              MPI_Gather
                 |
                 ↓
          Complete Matrix C
             on Rank 0
```

Matrix B is distributed to all processes using:

```text
MPI_Bcast
```

---

## 10 Experimental Result

Reference result:

```text
MPI Matrix Multiplication Completed
Matrix Size = 4000 x 4000
Number of MPI Processes = 4
Execution Time = 92.979510 seconds
Verification C[0][0] = 4000.00
```

### Performance Comparison

| Method     |   Processes |    Execution Time |
| ---------- | ----------: | ----------------: |
| Sequential |           1 |    244.12 seconds |
| OpenMP     |   8 threads | 30.830434 seconds |
| MPI        | 4 processes | 92.979510 seconds |

### MPI Speedup

```text
Speedup = Sequential Time / MPI Time

Speedup = 244.12 / 92.979510

Speedup ≈ 2.63×
```

---

## 11 Verification

The output:

```text
Verification C[0][0] = 4000.00
```

confirms the expected matrix multiplication result because all elements of matrices A and B were initialized to `1.0`.

---

## 12 How MPI Works in This Experiment

MPI uses independent processes with separate memory spaces. The Master process distributes portions of Matrix A using `MPI_Scatter`.

Matrix B is sent to all processes using `MPI_Bcast`.

Each MPI process independently computes its assigned 1000 rows. The partial results are then collected by the Master process using `MPI_Gather`.

Because the processes communicate through network-connected virtual machines, communication and data transfer contribute to the total execution time.

---

## 13 Conclusion

The MPI implementation successfully performed distributed matrix multiplication using four MPI processes across one Master VM and three Worker VMs. The reference execution time was 92.979510 seconds compared with 244.12 seconds for the sequential implementation, giving a reference speedup of approximately 2.63×.

MPI demonstrates distributed-memory parallelism where separate processes communicate explicitly using operations such as `MPI_Scatter`, `MPI_Bcast`, and `MPI_Gather`.

## Author

Monika.M.Bhandari
