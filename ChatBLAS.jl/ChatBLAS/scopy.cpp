#include "chatblas_hip.h" __global__ void scopy_kernel( int n, float *x, float *y ) { int index = threadIdx.x + blockIdx.x * blockDim.x; if (index < n) { y[index] = x[index]; } } void chatblas_scopy(int n, float *x, float *y) { float *d_x, *d_y; hipMalloc((void**)&d_x, n*sizeof(float)); hipMalloc((void**)&d_y, n*sizeof(float)); int threadsPerBlock = 256; int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock; hipMemcpy(d_x, x, n*sizeof(float), hipMemcpyHostToDevice); scopy_kernel<<<blocksPerGrid, threadsPerBlock>>>(n, d_x, d_y); hipMemcpy(y, d_y, n*sizeof(float), hipMemcpyDeviceToHost); hipFree(d_x); hipFree(d_y); }