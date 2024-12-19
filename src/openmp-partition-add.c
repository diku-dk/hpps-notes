#include <omp.h>
#include <stdio.h>

int sum(int n, int* A) {
  int P = omp_get_max_threads();
  int sums[P];

  for (int i = 0; i < P; i++) {
    sums[i] = 0;
  }

#pragma omp parallel
  {
    int t = omp_get_thread_num();

    int chunk_size = n / P;
    int start = t * chunk_size;
    int end = start + chunk_size;
    if (t == omp_get_num_threads()-1) {
      end = n;
    }

    int local_sum = 0;

    for (int i = start; i < end; i++) {
      local_sum += A[i];
    }

    sums[t] = local_sum;
  }

  int sum = 0;
  for (int i = 0; i < P; i++) {
    sum += sums[i];
  }

  return sum;
}

#include "timing.h"
#include <assert.h>

int main(int argc, char** argv) {
  assert(argc==2);
  int n = atoi(argv[1]);
  int* arr = calloc(n, sizeof(int));
  for (int i = 0; i < n; i++) {
    arr[i] = i % 2;
  }

  double bef = seconds();
  int x = sum(n, arr);
  double aft = seconds();
  printf("sum: %d\ntime: %f\n", x, aft-bef);
}
