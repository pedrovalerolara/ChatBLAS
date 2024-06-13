#include "chatblas_hip.h"
__global__ void sscal_kernel( int n, float a , float *x ) {
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    if (i < n) {
        x[i] = a * x[i];
    }
}

void chatblas_sscal( int n, float a, float *x) {
    float *d_x;
    hipMalloc(&d_x, n * sizeof(float));

    hipMemcpy(d_x, x, n * sizeof(float), hipMemcpyHostToDevice);

    int blockSize = 256;
    int numBlocks = (n + blockSize - 1) / blockSize;

    sscal_kernel<<<numBlocks, blockSize>>>(n, a, d_x);

    hipMemcpy(x, d_x, n * sizeof(float), hipMemcpyDeviceToHost);

    hipFree(d_x);
}