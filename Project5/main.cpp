#include "basic_function.h"

#define COMPUTE_PROFILE = 1



void Matrix_Mult(double* in_A, double* in_B, double* out_C, int M, int N, int K, int num_of_core);
void print_Matrix(double* Matrix, int M, int N);

const char *getErrorString(cl_int error);
void error_checker(char* func_name, cl_int status);
char* readKernelFile(const char* filename, long* _size);
void CL_Matrix_Mult(double* A, double* B, double* C, int M, int N, int K);


int main()
{
	int M = 512;
	int N = 512;
	int K = 512;
	time_t t;

	double* A;
	double* B;
	double* C;

	A = (double*)malloc(sizeof(double)*M *N);
	B = (double*)malloc(sizeof(double)*N *K);
	C = (double*)malloc(sizeof(double)*M *K);

	int i, j, k;

	for (i = 0; i < M*N; i++) {
		A[i] = i;
	}
	for (i = 0; i < N*K; i++) {
		B[i] = i;
	}
	for (i = 0; i < M*K; i++) {
		C[i] = 0.0;
	}
	

	
	printf("\nOpenMP Acceleration\n");
	/*
	for (i = 1; i < 5; i++) {
		printf("\n\n MM with %d core \n", i);
		t = clock();
		Matrix_Mult(A, B, C, M, N, K, i);
		t = clock() - t;
		printf("time : %d ms\n", t);
	}
	*/

	printf("OpenCL Acceleration step 1");

	printf("\n\n clMM step 1 \n");
	t = clock();
	CL_Matrix_Mult(A, B, C, M, N, K);
	t = clock() - t;
	printf("time : %d ms\n", t);




	free(A);
	free(B);
	free(C);

	return 0;
}
