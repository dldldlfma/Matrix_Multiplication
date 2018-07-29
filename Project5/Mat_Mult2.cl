__kernel void mat_mult(const __global double* A,
					   const __global double* B,
							 __global double* C,
					   const int M,
					   const int N,
					   const int K)
{

	// Thread identifiers
	const int m = get_global_id(0);
	const int k = get_global_id(1);


	double acc = 0.0;
	for (int n = 0; n<N; n++) {
		acc += A[m*N + n] * B[n*K + k];
	}

	// Store the result
	C[m*K + k] = acc;
}
