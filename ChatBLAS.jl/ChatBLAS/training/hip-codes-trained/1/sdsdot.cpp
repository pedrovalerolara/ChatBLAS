#include "chatblas_hip.h" 

__global__ void sdsdot_kernel( int n, float b, float *x, float *y, float *res ) { res[threadIdx.x] = 0.0; __shared__ double cache[256]; int tid = threadIdx.x + blockIdx.x * blockDim.x; int cacheIndex = threadIdx.x; double temp = 0; while (tid < n) { temp += (double)x[tid] * (double)y[tid]; tid += blockDim.x * gridDim.x; } cache[cacheIndex] = temp; __syncthreads(); int i = blockDim.x / 2; while (i != 0) { if (cacheIndex < i) cache[cacheIndex] += cache[cacheIndex + i]; __syncthreads(); i /= 2; } if (cacheIndex == 0) res[blockIdx.x] = cache[0] + b; } 

float chatblas_sdsdot( int n, float b, float *x, float *y) { float *x_device, *y_device, *res_device, result = 0; float *partial_res = (float*) malloc(256 * sizeof(float)); hipMalloc((void**)&x_device, n * sizeof(float)); hipMemcpy(x_device, x, n * sizeof(float), hipMemcpyHostToDevice); hipMalloc((void**)&y_device, n * sizeof(float)); hipMemcpy(y_device, y, n * sizeof(float), hipMemcpyHostToDevice); hipMalloc((void**)&res_device, 256 * sizeof(float)); int threadsPerBlock = 256; int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock; sdsdot_kernel<<<blocksPerGrid, threadsPerBlock>>>(n, b, x_device, y_device, res_device); hipMemcpy(partial_res, res_device, 256 * sizeof(float), hipMemcpyDeviceToHost); for (int i = 0; i < blocksPerGrid; i++) { result += partial_res[i]; } hipMemcpy(&result, res_device, sizeof(float), hipMemcpyDeviceToHost); hipFree(x_device); hipFree(y_device); hipFree(res_device); free(partial_res); return result; }
