#include "chatblas_openmp.h"
#include <omp.h>

float chatblas_sdsdot(int n, float b, float *x, float *y) {
    double dot_product = 0.0;

    #pragma omp parallel for reduction (+:dot_product)
    for (int i = 0; i < n; i++) {
        dot_product += ((double)x[i]) * ((double)y[i]);
    }

    return (float)(dot_product + b);
}
