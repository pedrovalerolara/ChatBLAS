#include <math.h>
#include "hip/hip_runtime.h"

//BLAS Level 1
void chatblas_sscal(int n, float a, float *x);
void chatblas_sscal(int n, float a, float *x);
void chatblas_saxpy(int n, float a, float *x, float *y);
int chatblas_isamax(int n, float *x); 
float chatblas_snrm2(int n, float *x);
float chatblas_sdot(int n, float *x, float *y);
float chatblas_sdsdot(int n, float b, float *x, float *y);
float chatblas_sasum(int n, float *x);
void chatblas_sswap(int n, float *x, float *y);

/*
extern "C" void chatblas_saxpy(int n, float a, float *x, float *y);
extern "C" int chatblas_isamax(int n, float *x);
extern "C" float chatblas_snrm2(int n, float *x);
extern "C" float chatblas_sdot(int n, float *x, float *y);
extern "C" float chatblas_sdsdot(int n, float b, float *x, float *y);
extern "C" float chatblas_sasum(int n, float *x);
extern "C" void chatblas_scopy( int n, float *x, float *y);
extern "C" void chatblas_sswap( int n, float *x, float *y);
*/
