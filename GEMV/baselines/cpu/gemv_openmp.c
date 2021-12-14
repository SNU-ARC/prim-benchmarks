#include <stdlib.h>
#include <stdio.h>
#include "../../support/timer.h"
#include "gemv_utils.h"
#include <sys/mman.h>
#include <omp.h>
#include <stdbool.h>
#include <math.h>

size_t m, m_; // m_ : rows
size_t n, n_; // n_ : cols
size_t k, k_;

int main(int argc, char *argv[])
{
  if (mlockall(MCL_CURRENT | MCL_FUTURE)) {
      perror("mlockall failed:");
      return 0;
  }

  //m_ = 20480;
  //n_ = 8192;
  //k_ = 8001;

  m_ = 1001;
  n_ = 1000;
  k_ = 999;
  
  m = m_ + 4 - m_%4;
  n = n_ + 4 - n_%4;
  k = k_ + 4 - k_%4;

  double *A, *b, *b2, *b3, *x;
  
  A = (double*) malloc(sizeof(double)*m*n);
  b = (double*) malloc(sizeof(double)*m*k);
  b2 = (double*) malloc(sizeof(double)*m*k);
  b3 = (double*) malloc(sizeof(double)*m*k);
  x = (double*) malloc(sizeof(double)*n*k);

  make_hilbert_mat(m, n, m_, n_, A);
  make_hilbert_mat(n, k, n_, k_, x);

  #pragma omp for
  for (size_t p = 0; p < m; p++) {
    for (size_t q = 0; q < k; q++) {
      b[p*k+q] = (double) 0.0;
      b2[p*k+q] = (double) 0.0;
      b3[p*k+q] = (double) 0.0;
    }
  }

  Timer timer;
  start(&timer, 0, 0);
  gemv(A, x, m, n, k, b, 1); 
  stop(&timer, 0);

  start(&timer, 1, 0);
  vec_dgemm_opt_c(m, n, k, b2, A, x);
  stop(&timer, 1);

  start(&timer, 2, 0);
  gemv(A, x, m, n, k, b3, 4); 
  stop(&timer, 2);

  double sum_1 = sum_vec(b, m, k);
  double sum_2 = sum_vec(b2, m, k);
  double sum_3 = sum_vec(b3, m, k);

  bool hwacha_correct = sum_1 == sum_2;
  bool multi_correct = sum_1 == sum_3;

  for (int p = 0; p < m; p++) {
    for (int q = 0; q < k; q++) {
      if (b[p * k + q] != b2[p * k + q]) {
        printf("%d %d : %30.25lf %30.25lf\n", p, q, b[p * k + q], b2[p * k + q]);
      }
    }
  }

  printf("%30.25lf %30.25lf\n", sum_1, sum_2);

  if (hwacha_correct) {
      printf("Both works correctly.\n");  
  } else if (multi_correct) {
      printf("Hwacha outputs wrong result!\n");      
  } else if (hwacha_correct) {
  	printf("Hwacha works correctly, but not multi-threading\n");
  } else {
    printf("Both wrong\n");
  }

  printf("******************************\n");
  printf("CPU ");
  print(&timer, 0, 1);
  printf("\n");
  printf("Hwacha ");
  print(&timer, 1, 1);
  printf("\n");
  printf("4 threads ");
  print(&timer, 2, 1);
  printf("\n");

#if 0
  print_vec(x, m_);
  print_mat(A, m_, n_);
  print_vec(b, m_);
#endif

  free(A);
  free(b);
  free(b2);
  free(b3);
  free(x); 

  munlockall();

  return 0;
}

void gemv(double* A, double* x, size_t m, size_t n, size_t k, double* b, int t) {
  omp_set_num_threads(t);
  #pragma omp parallel for
  for (size_t p = 0; p < m; p++) {
    for (size_t q = 0; q < k; q++) {
      for (size_t r = 0; r < n; r++) {
        b[p*k+q] += A[p*n+r]*x[r*k+q];
      }
    }
  }
}

void make_hilbert_mat(size_t r, size_t c, size_t r_, size_t c_, double* m) {
  #pragma omp parallel for
  for (size_t p = 0; p < r; p++) {
    for (size_t q = 0; q < c; q++) {
      if (p < r_ && q < c_) {
        m[p*c+q] = 1 / ((double) p + (double) q + 1.0);
      } else {
        m[p*c+q] = (double) 0.0;
      }
    }
  }
}

double sum_vec(double* vec, size_t m, size_t k) {
  double sum = 0.0;
  #pragma omp parallel for reduction(+:sum)
  for (int p = 0; p < m; p++) 
    for (int q = 0; q < k; q++)
      sum = sum + vec[p*k+q];
  return sum;
}
