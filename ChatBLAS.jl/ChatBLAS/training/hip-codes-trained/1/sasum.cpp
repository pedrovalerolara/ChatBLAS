#include "chatblas_hip.h" 

__global__ void sasum_kernel(int n, float *x, float *sum) { __shared__ float cache[512]; int tid = threadIdx.x + blockIdx.x * blockDim.x; int cacheIndex = threadIdx.x; float temp = 0; while (tid < n) { temp += fabs(x[tid]); tid += blockDim.x * gridDim.x; } cache[cacheIndex] = temp; __syncthreads(); int i = blockDim.x/2; while (i != 0) { if (cacheIndex < i) cache[cacheIndex] += cache[cacheIndex + i]; __syncthreads(); i /= 2; } if (cacheIndex == 0) sum[blockIdx.x] = cache[0]; } 

float chatblas_sasum(int n, float *x) { float *sum, *d_x, *d_sum; float result = 0.0; sum = (float*)malloc(n*sizeof(float)); hipMalloc(&d_x, n*sizeof(float)); hipMalloc(&d_sum, 512 * sizeof(float)); hipMemcpy(d_x, x, n*sizeof(float), hipMemcpyHostToDevice); sasum_kernel<<<512, 512>>>(n, d_x, d_sum); hipMemcpy(sum, d_sum, 512 * sizeof(float), hipMemcpyDeviceToHost); for (int i = 0; i < 512; i++) { result += sum[i]; } result = fabs(result); hipFree(d_x); hipFree(d_sum); free(sum); return result; }