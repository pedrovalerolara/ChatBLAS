#include "chatblas_hip.h" 

__global__ void sswap_kernel(int n, float *x, float *y) { int i = blockIdx.x * blockDim.x + threadIdx.x; if (i < n) { float temp = x[i]; x[i] = y[i]; y[i] = temp; } } 

void chatblas_sswap(int n, float *x, float *y) { const int blockSize = 256; const int numBlocks = (n + blockSize - 1) / blockSize; float *d_x, *d_y; hipMalloc((void**)&d_x, n * sizeof(float)); hipMalloc((void**)&d_y, n * sizeof(float)); hipMemcpy(d_x, x, n * sizeof(float), hipMemcpyHostToDevice); hipMemcpy(d_y, y, n * sizeof(float), hipMemcpyHostToDevice); sswap_kernel<<<numBlocks, blockSize>>>(n, d_x, d_y); hipMemcpy(x, d_x, n * sizeof(float), hipMemcpyDeviceToHost); hipMemcpy(y, d_y, n * sizeof(float), hipMemcpyDeviceToHost); hipFree(d_x); hipFree(d_y); }
