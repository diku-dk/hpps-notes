void vecadd_implicit(int n,
                     const double *x, const double *y,
                     double *out) {
#pragma omp parallel for
  for (int i = 0; i < n; i++) {
    out[i] = x[i] + y[i];
  }
}

#include <omp.h>

void vecadd_explicit(int n,
                     const double *x, const double *y,
                     double *out) {
#pragma omp parallel
  {
    int t = omp_get_thread_num();
    int P = omp_get_num_threads();
    int chunk_size = n / P;
    int start = t * chunk_size;
    int end = start + chunk_size;
    if (t == omp_get_num_threads()-1) {
      end = n;
    }

    for (int i = start; i < end; i++) {
      out[i] = x[i] + y[i];
    }
  }
}

int main() {
}
