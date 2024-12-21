#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int filter(int n, int *dst, int *src) {
  int p = 0;
  for (int i = 0; i < n; i++) {
    if (src[i] >= 0) {
      dst[p] = src[i];
      p++;
    }
  }
  return p;
}

#include <omp.h>

int filter_openmp_limited(int n, int *dst, int *src) {
  int P = omp_get_max_threads();
  int* parts[P];
  int parts_n[P];

  for (int i = 0; i < P; i++) {
    parts[i] = NULL;
    parts_n[i] = 0;
  }

#pragma omp parallel
  {
    int t = omp_get_thread_num();
    int start = t * n/P;
    int end = (t+1) * n/P;
    if (t == omp_get_num_threads()-1) { end = n; }
    int chunk_size = end - start;

    parts[t] = malloc(sizeof(int) * chunk_size);
    parts_n[t] = filter(chunk_size, parts[t], src+start);
  }

  int p = 0;
  for (int i = 0; i < P; i++) {
    memcpy(dst+p, parts[i], parts_n[i]*sizeof(int));
    p += parts_n[i];
    free(parts[i]);
  }
  return p;
}

int filter_openmp(int n, int *dst, int *src) {
  int P = omp_get_max_threads();
  int* parts[P];
  int parts_n[P];

  for (int i = 0; i < P; i++) {
    parts[i] = NULL;
    parts_n[i] = 0;
  }

#pragma omp parallel
  {
    int t = omp_get_thread_num();
    int start = t * n/P;
    int end = (t+1) * n/P;
    if (t == omp_get_num_threads()-1) { end = n; }
    int chunk_size = end - start;

    parts[t] = malloc(sizeof(int) * chunk_size);
    parts_n[t] = filter(chunk_size, parts[t], src+start);
  }

  int offsets[P];
  offsets[0] = 0;
  for (int t = 1; t < P; t++) {
    offsets[t] = offsets[t-1] + parts_n[t-1];
  }

#pragma omp parallel for
  for (int t = 0; t < P; t++) {
    memcpy(dst+offsets[t], parts[t],
           parts_n[t]*sizeof(int));
    free(parts[t]);
  }

  return offsets[P-1]+parts_n[P-1];
}

int main(int argc, char** argv) {
  assert(argc == 2);
  int n = atoi(argv[1]);

  int* src = malloc(sizeof(int) * n);
  int* dst = malloc(sizeof(int) * n);

  for (int i = 0; i < n; i++) {
    src[i] = rand() % 10 - 5;
  }

  for (int i = 0; i < n; i++) {
    printf("%d\n", src[i]);
  }

  printf("===\n");

  int p = filter_openmp(n, dst, src);

  for (int i = 0; i < p; i++) {
    printf("%d\n", dst[i]);
  }
}
