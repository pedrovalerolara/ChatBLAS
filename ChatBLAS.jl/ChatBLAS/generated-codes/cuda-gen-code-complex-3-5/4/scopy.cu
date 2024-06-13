#include "chatblas_cuda.h"

__global__ void scopy_kernel(int n, float *x, float *y) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if(tid < n) {
        y[tid] = x[tid];
    }
}

void chatblas_scopy(int n, float *x, float *y) {
    float *d_x, *d_y;
    int size = sizeof(float) * n;

    cudaMalloc((void **)&d_x, size);
    cudaMalloc((void **)&d_y, size);

    cudaMemcpy(d_x, x, size, cudaMemcpyHostToDevice);

    int threadsPerBlock = 256;
    int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;

    scopy_kernel<<<blocksPerGrid, threadsPerBlock>>>(n, d_x, d_y);

    cudaMemcpy(y, d_y, size, cudaMemcpyDeviceToHost);

    cudaFree(d_x);
    cudaFree(d_y);
}
