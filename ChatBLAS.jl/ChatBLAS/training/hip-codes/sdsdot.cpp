#include "chatblas_hip.h"

__global__ void sdsdot_kernel( int n, float b, float *x, float *y, float *res ) {
    extern __shared__ double shared_mem[];
    int tid = threadIdx.x;
    shared_mem[tid] = 0.0;
    for (int i = tid; i < n; i += blockDim.x) {
        shared_mem[tid] += (double)x[i] * (double)y[i];
    }
    __syncthreads();

    if (tid == 0) {
        double sum = 0.0;
        for(int i = 0; i < blockDim.x; ++i) {
            sum += shared_mem[i];
        }
        res[0] = (float)(sum + b); 
    }
}

float chatblas_sdsdot( int n, float b, float *x, float *y) {
    float *res, *d_x, *d_y, *d_res;
    hipMalloc((void**)&d_x, n*sizeof(float));
    hipMemcpy(d_x, x, n*sizeof(float), hipMemcpyHostToDevice);
    hipMalloc((void**)&d_y, n*sizeof(float));
    hipMemcpy(d_y, y, n*sizeof(float), hipMemcpyHostToDevice);

    int blockSize = 32;
    int gridSize = n / blockSize + (n % blockSize > 0);
  
    hipMalloc((void**)&d_res, 1 * sizeof(float));
    sdsdot_kernel<<<gridSize, blockSize, blockSize * sizeof(double)>>>(n, b, d_x, d_y, d_res);

    res = (float*)malloc(1 * sizeof(float));
    hipMemcpy(res, d_res, 1 * sizeof(float), hipMemcpyDeviceToHost);
  
    float dotProductResult = res[0];
    free(res);
    hipFree(d_x);
    hipFree(d_y);
    hipFree(d_res);
    
    return dotProductResult;
}
