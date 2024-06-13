#include "chatblas_openmp.h"

float chatblas_sasum(int n, float *x)
{
    float sum = 0;
    int i;

    #pragma omp parallel for reduction(+:sum) shared(n, x)
    for (i = 0; i < n; i++)
    {
        sum += fabs(x[i]);
    }

    return sum;
}