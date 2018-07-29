#pragma once

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <CL/cl.h>



void Matrix_Mult(double* in_A, double* in_B, double* out_C, int M, int N, int K, int num_of_core);
void print_Matrix(double* Matrix, int M, int N);
const char *getErrorString(cl_int error);
void error_checker(char* func_name, cl_int status);
char* readKernelFile(const char* filename, long* _size);
void CL_Matrix_Mult(double* A, double* B, double* C, int M, int N, int K);