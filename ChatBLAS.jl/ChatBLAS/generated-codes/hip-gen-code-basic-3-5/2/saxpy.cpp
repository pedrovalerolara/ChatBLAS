#include "chatblas_hip.h"

__global__ void saxpy_kernel(int n, float a, float *x, float *y) {
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i < n) {
        y[i] += a * x[i];
    }
}

void chatblas_saxpy(int n, float a, float *x, float *y) {
    float *d_x, *d_y;
    hipMalloc(&d_x, n * sizeof(float));
    hipMalloc(&d_y, n * sizeof(float));

    hipMemcpy(d_x, x, n * sizeof(float), hipMemcpyHostToDevice);
    hipMemcpy(d_y, y, n * sizeof(float), hipMemcpyHostToDevice);

    int blockSize = 256;
    int numBlocks = (n + blockSize - 1) / blockSize;

    saxpy_kernel<<<numBlocks, blockSize>>>(n, a, d_x, d_y);

    hipMemcpy(y, d_y, n * sizeof(float), hipMemcpyDeviceToHost);

    hipFree(d_x);
    hipFree(d_y);
}