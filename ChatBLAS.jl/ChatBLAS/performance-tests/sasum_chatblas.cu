#include <sys/time.h>
#include <cublas_v2.h>
#include "chatblas_cuda.h"

int main()
{

  struct timeval stop, start;    
  float *host_x;
  float *dev_x, *dev_result;
  float result_cublas, result_chatblas;
  cublasHandle_t h;

  int N = 500000000;
  host_x = (float *) malloc(N * sizeof(float));
  
  for (int i=0; i<N; ++i) {
    host_x[i] = 1.0;
  }
  
 
  cublasCreate(&h);
  cublasSetPointerMode(h, CUBLAS_POINTER_MODE_DEVICE);

  gettimeofday(&start, NULL);
  
  cudaMalloc( (void**)&dev_x, N*sizeof(float));
  cudaMalloc( (void**)&dev_result, sizeof(float));

  cublasSetVector(N, sizeof(host_x[0]), host_x, 1, dev_x, 1);
  cudaDeviceSynchronize();

  cublasSasum(h, N, dev_x, 1, dev_result);
  cudaDeviceSynchronize();

  cudaMemcpy(&result_cublas, dev_result, sizeof(float), cudaMemcpyDeviceToHost);

  gettimeofday(&stop, NULL);
  printf("cuBLAS took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

  if (h)
    cublasDestroy(h);
  cudaFree(dev_x);

  gettimeofday(&start, NULL);

  result_chatblas = chatblas_sasum( N, host_x );

  gettimeofday(&stop, NULL);
  printf("chatBLAS took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

  if(result_chatblas != result_cublas){
    printf("Error, cuBLAS = %2.f, chatBLAS = %2.f\n", result_cublas, result_chatblas);
  }
	  
  free(host_x);

  return 0;
}



