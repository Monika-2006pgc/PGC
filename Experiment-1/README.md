# Experiment 1 – Sequential Matrix Multiplication

## 1. Objective

To implement sequential matrix multiplication using C and measure its execution time. This execution time is used as the baseline for comparing the performance of OpenMP, MPI, and CUDA implementations.

## 2. Environment

- Operating System: Windows 11
- Linux Environment: Ubuntu using WSL2
- Programming Language: C
- Compiler: GCC
- Matrix Size: 4000 × 4000
- Compiler Optimization: `-O2`

## 3. Prerequisites

The following are required:

- Windows PowerShell
- WSL2
- Ubuntu distribution
- Internet connection for package installation
- Sudo permission in Ubuntu
- GCC compiler

## 4. Verify WSL

Open Windows PowerShell and run:

```bash
wsl --status
