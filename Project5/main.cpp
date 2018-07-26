#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#include <CL/cl.h>


using namespace std;

void Matrix_Mult(double* in_A, double* in_B, double* out_C, int M, int N, int K, int num_of_core);
void print_Matrix(double* Matrix, int M, int N);

const char *getErrorString(cl_int error);
void error_checker(char* func_name, cl_int status);
char* readKernelFile(const char* filename, long* _size);
void CL_Matrix_Mult(double* A, double* B, double* C, int M, int N, int K);


int main()
{
	int M = 2048;
	int N = 2048;
	int K = 2048;
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
	
	for (i = 1; i < 5; i++) {
		printf("\n\n MM with %d core \n", i);
		t = clock();
		Matrix_Mult(A, B, C, M, N, K, i);
		t = clock() - t;
		printf("time : %d ms\n", t);
	}
	

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

/* 
M = length of Matrix's row 
N = length of Matrix's col 
*/
void print_Matrix(double* Matrix, int M, int N) {

	int i, j;
	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			cout << Matrix[i*N + j] << "	";
		}
		cout << endl;
	}
	cout << endl;
}

void Matrix_Mult(double* in_A, double* in_B, double* out_C, int M, int N, int K, int num_of_core) {

	omp_set_num_threads(num_of_core);
	int m, n, k;
	double val=0;

#pragma omp parallel for private(m)
	for (m = 0; m < M; m++) {
		for (k = 0; k < K; k++) {
			for (n = 0; n< N; n++) {
				val += in_A[m*N + n] * in_B[n * K + k];
			}
			out_C[m*K + k] = val;
			val = 0;
		}
	}


}



const char *getErrorString(cl_int error)
{
	switch (error) {
		// run-time and JIT compiler errors
	case 0: return "CL_SUCCESS";
	case -1: return "CL_DEVICE_NOT_FOUND";
	case -2: return "CL_DEVICE_NOT_AVAILABLE";
	case -3: return "CL_COMPILER_NOT_AVAILABLE";
	case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
	case -5: return "CL_OUT_OF_RESOURCES";
	case -6: return "CL_OUT_OF_HOST_MEMORY";
	case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
	case -8: return "CL_MEM_COPY_OVERLAP";
	case -9: return "CL_IMAGE_FORMAT_MISMATCH";
	case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
	case -11: return "CL_BUILD_PROGRAM_FAILURE";
	case -12: return "CL_MAP_FAILURE";
	case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
	case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
	case -15: return "CL_COMPILE_PROGRAM_FAILURE";
	case -16: return "CL_LINKER_NOT_AVAILABLE";
	case -17: return "CL_LINK_PROGRAM_FAILURE";
	case -18: return "CL_DEVICE_PARTITION_FAILED";
	case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

		// compile-time errors
	case -30: return "CL_INVALID_VALUE";
	case -31: return "CL_INVALID_DEVICE_TYPE";
	case -32: return "CL_INVALID_PLATFORM";
	case -33: return "CL_INVALID_DEVICE";
	case -34: return "CL_INVALID_CONTEXT";
	case -35: return "CL_INVALID_QUEUE_PROPERTIES";
	case -36: return "CL_INVALID_COMMAND_QUEUE";
	case -37: return "CL_INVALID_HOST_PTR";
	case -38: return "CL_INVALID_MEM_OBJECT";
	case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
	case -40: return "CL_INVALID_IMAGE_SIZE";
	case -41: return "CL_INVALID_SAMPLER";
	case -42: return "CL_INVALID_BINARY";
	case -43: return "CL_INVALID_BUILD_OPTIONS";
	case -44: return "CL_INVALID_PROGRAM";
	case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
	case -46: return "CL_INVALID_KERNEL_NAME";
	case -47: return "CL_INVALID_KERNEL_DEFINITION";
	case -48: return "CL_INVALID_KERNEL";
	case -49: return "CL_INVALID_ARG_INDEX";
	case -50: return "CL_INVALID_ARG_VALUE";
	case -51: return "CL_INVALID_ARG_SIZE";
	case -52: return "CL_INVALID_KERNEL_ARGS";
	case -53: return "CL_INVALID_WORK_DIMENSION";
	case -54: return "CL_INVALID_WORK_GROUP_SIZE";
	case -55: return "CL_INVALID_WORK_ITEM_SIZE";
	case -56: return "CL_INVALID_GLOBAL_OFFSET";
	case -57: return "CL_INVALID_EVENT_WAIT_LIST";
	case -58: return "CL_INVALID_EVENT";
	case -59: return "CL_INVALID_OPERATION";
	case -60: return "CL_INVALID_GL_OBJECT";
	case -61: return "CL_INVALID_BUFFER_SIZE";
	case -62: return "CL_INVALID_MIP_LEVEL";
	case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
	case -64: return "CL_INVALID_PROPERTY";
	case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
	case -66: return "CL_INVALID_COMPILER_OPTIONS";
	case -67: return "CL_INVALID_LINKER_OPTIONS";
	case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

		// extension errors
	case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
	case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
	case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
	case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
	case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
	case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
	default: return "Unknown OpenCL error";
	}
}

void error_checker(const char* func_name, cl_int status) {
	printf("%s error is : %s \n", func_name, getErrorString(status));
}

char* readKernelFile(const char* filename, long* _size) {

	// Open the file
	FILE* file = fopen(filename, "r");
	if (!file) {
		printf("-- Error opening file %s\n", filename);
		exit(1);
	}

	// Get its size
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	rewind(file);

	// Read the kernel code as a string
	char* source = (char *)malloc((size + 1) * sizeof(char));
	fread(source, 1, size * sizeof(char), file);
	source[size] = '\0';
	//printf("\n\n %s \n\n", source);
	fclose(file);

	// Save the size and return the source string
	*_size = (size + 1);
	return source;
}



void CL_Matrix_Mult(double* A, double* B, double* C, int M, int N, int K) {

	/*
	printf("\n\n");
	print_Matrix(A, M, N);
	printf("\n\n");
	print_Matrix(B, N, K);
	printf("\n\n");
	print_Matrix(C, M, K);
	printf("\n\n");
	*/
	
	//각 API 호출의 출력체크를 위해 사용함
	cl_int status;

	// 플랫폼 수를 가져옴
	cl_uint numPlatforms = 0;
	status = clGetPlatformIDs(0, NULL, &numPlatforms);
	//error_checker("clGetPlatformIDs", status);

	//각 플랫폼을 위한 충분한 공간을 할당
	cl_platform_id* platforms = NULL;
	platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id)*numPlatforms);

	//플랫폼 정보를 가져다가 platforms에 담음
	status = clGetPlatformIDs(numPlatforms, platforms, NULL);
	//error_checker("clGetPlatformIDs", status);

	//각 디바이스의 수를 가져옴
	cl_uint numDevices = 0;
	status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
	//error_checker("clGeDeviceIDs", status);

	//각 디바이스를 위한 충분한 공간을 할당
	cl_device_id* devices = NULL;
	devices = (cl_device_id*)malloc(sizeof(cl_device_id) * numDevices);

	//디바이스 정보를 가져옴
	status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, numDevices, devices, NULL);
	//error_checker("clGeDeviceIDs", status);

	//컨택스트를 생성하고 디바이스와 연결시킴
	cl_context context;
	context = clCreateContext(NULL, numDevices, devices, NULL, NULL, &status);
	//error_checker("clCreateContext", status);

	//명령어 큐를 생성하고 디바이스와 연결시킴
	cl_command_queue cmdQueue;
	cmdQueue = clCreateCommandQueue(context, devices[0], 0, &status);
	//error_checker("clCreateCommandQueue", status);

	//호스트 배열 A로 부터 데이터를 포함하는 버퍼 객체를 생성
	cl_mem bufA;
	bufA = clCreateBuffer(context, CL_MEM_READ_ONLY, (M*N)*(sizeof(double)), NULL, &status);
	//error_checker("clCreateBuffer for A", status);

	//호스트 배열 B로부터 데이터를 포함하는 버퍼 객체를 생성
	cl_mem bufB;
	bufB = clCreateBuffer(context, CL_MEM_READ_ONLY, (N*K)*(sizeof(double)), NULL, &status);
	//error_checker("clCreateBuffer for B", status);

	//호스트 배열 C로부터 데이터를 포함하는 버퍼 객체 생성

	cl_mem bufC;
	bufC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, (M*K)*(sizeof(double)), NULL, &status);
	//error_checker("clCreateBuffer for C", status);

	//입력 배열 A,B를 디바이스 버퍼 bufA,bufB에 작성
	status = clEnqueueWriteBuffer(cmdQueue, bufA, CL_FALSE, 0,(M*N)*(sizeof(double)), A, 0, NULL, NULL);
	//error_checker("clEnqueueWriteBuffer for A", status);
	status = clEnqueueWriteBuffer(cmdQueue, bufB, CL_FALSE, 0,(N*K)*(sizeof(double)), B, 0, NULL, NULL);
	//error_checker("clEnqueueWriteBuffer for B", status);

	// Mat_Mult.cl 을 읽어서 source에 저장함 
	//readKernelFile이라는 함수를 만들어서 사용
	long sizeSource;
	const char* source = readKernelFile("Mat_Mult.cl", &sizeSource);

	//program을 Source에서 받아서 생성
	cl_program program = clCreateProgramWithSource(context, 1, &source, NULL, &status);
	//error_checker("clCreateProgramWithSource", status);

	//생성한 프로그램을 빌드함
	status = clBuildProgram(program, numDevices, devices, NULL, NULL, NULL);

	//벡터 덧셈 커널을 생성
	cl_kernel kernel;
	kernel = clCreateKernel(program, "mat_mult", &status);
	//error_checker("clCreateKernel", status);

	//입력 및 출력 버퍼와 커널을 연결
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufA);
	//error_checker("clSetKernelArg A", status);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufB);
	//error_checker("clSetKernelArg B", status);
	status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufC);
	//error_checker("clSetKernelArg C", status);
	status = clSetKernelArg(kernel, 3, sizeof(int), &M);
	//error_checker("clSetKernelArg K", status);
	status = clSetKernelArg(kernel, 4, sizeof(int), &N);
	//error_checker("clSetKernelArg N", status);
	status = clSetKernelArg(kernel, 5, sizeof(int), &K);
	//error_checker("clSetKernelArg N", status);

	//실행을 위해 워크아이템의 인덱스 공간(글로벌 워크사이즈)를 정의함
	//워크그룹 크기(로컬 워크 사이즈)가 필요하지는 않지만 사용될 수 있다. 
	size_t globalWorkSize[2] = {M, K};

	//커널을 실행함
	cl_event event;
	status = clEnqueueNDRangeKernel(cmdQueue, kernel, 2, NULL, globalWorkSize, NULL, 0, NULL, &event);
	//error_checker("clEnqueueNDRangeKernel", status);

	status = clWaitForEvents(1, &event);
	//error_checker("clWaitForEvents", status);

	//디바이스 출력 버퍼에서 호스트 출력 버퍼로 읽어옴
	clEnqueueReadBuffer(cmdQueue, bufC, CL_TRUE, 0, (M*K)*(sizeof(double)), C, 0, NULL,NULL);
	//error_checker("clEnqueueReadBuffer", status);

	//print_Matrix(C, M, K);

	//OpenCL 리소스 해제
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(cmdQueue);
	clReleaseMemObject(bufA);
	clReleaseMemObject(bufB);
	clReleaseMemObject(bufC);
	clReleaseContext(context);

	//호스트 리소스 해제
	//free(A);
	//free(B);
	//free(C);
	free(platforms);
	free(devices);

}