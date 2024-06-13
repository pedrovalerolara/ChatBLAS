#include "chatblas_hip.h" 

__global__ void isamax_kernel(int n, float *x, int *ind) { int id = threadIdx.x + blockIdx.x * blockDim.x; __shared__ float maxvals[256]; __shared__ int maxinds[256]; if (id < n) { maxinds[threadIdx.x] = id; maxvals[threadIdx.x] = fabsf(x[id]); } __syncthreads(); for (int i = blockDim.x/2; i>0; i >>= 1) { if (threadIdx.x < i && id + i < n) { if (maxvals[threadIdx.x] < maxvals[threadIdx.x + i]) { maxvals[threadIdx.x] = maxvals[threadIdx.x + i]; maxinds[threadIdx.x] = maxinds[threadIdx.x + i]; } } __syncthreads(); } if (threadIdx.x == 0) ind[blockIdx.x] = maxinds[0]; } 

int chatblas_isamax(int n, float *x) { float *d_x; int *d_ind, *ind; hipMalloc((void**)&d_x, n*sizeof(float)); hipMalloc((void**)&d_ind, n*sizeof(int)); hipMemcpy(d_x, x, n*sizeof(float), hipMemcpyHostToDevice); dim3 blockSize(256); dim3 gridSize(n/256 + (n%256 > 0)); isamax_kernel<<<gridSize, blockSize>>>(n, d_x, d_ind); ind = (int*)malloc(n*sizeof(int)); hipMemcpy(ind, d_ind, n*sizeof(int), hipMemcpyDeviceToHost); int result = ind[0]; for (int i = 1; i < n; i++) { if (x[ind[i]] > x[result]) result = ind[i]; } free(ind); hipFree(d_x); hipFree(d_ind); return result; }