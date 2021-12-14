/**
* @file app.c
* @brief Template for a Host Application Source File.
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "../../support/timer.h"
#include "../../support/common.h"
#include <sys/mman.h>
#include <omp.h>

T** A;
T* B;
T* B2;
T* B3;
T* C;

// Create input arrays
static void init_data(T** A, T* B, unsigned int m_size, unsigned int n_size){
    for (unsigned int l = 0; l < NUM_LAYERS; l++)
		for (unsigned int i = 0; i < m_size * n_size; i++){
			if(i % 100 < 98){
				A[l][i] = 0;
			}else{
				A[l][i] = (l+i) % 2;
			}
		}
	for (unsigned int i = 0; i < n_size; i++){
		if(i % 50 < 48){
			B[i] = 0;
		}
		else{
			B[i] = i % 2;
		}
	}
}

// Compute output in the host
static void mlp_host(T* C, T** A, T* B, unsigned int m_size, unsigned int n_size, int t) {
	for (unsigned int nl = 0; nl < NUM_LAYERS; nl++){
		for (unsigned int m = 0; m < m_size; m++){
			C[m] = 0;
		}

    omp_set_num_threads(t);
    #pragma omp parallel for
		for (unsigned int m = 0; m < m_size; m++){
			for (unsigned int n = 0; n < n_size; n++){
				C[m] += A[nl][m * n_size + n] * B[n];
			}
			C[m] = max(0, C[m]);
		}

		for (unsigned int n = 0; n < n_size; n++){
			B[n] = C[n];
		}
	}
}

// Compute output in the hwacha
static void mlp_hwacha(T* C, T** A, T* B, unsigned int m_size, unsigned int n_size) {
	for (unsigned int nl = 0; nl < NUM_LAYERS; nl++){
		for (unsigned int m = 0; m < m_size; m++){
			C[m] = 0;
		}
		
		for (unsigned int m = 0; m < m_size; m++) {
      T * result = (T *)calloc(n_size, sizeof(T));
      int size = vec_vvmlp_asm(n_size, result, &A[nl][m * n_size], B);

      T count_hwacha = 0;
      for (int j = 0; j < size; j++) {
        count_hwacha += result[j];
      }
			C[m] = max(0, count_hwacha);
      free(result);
		}
    vec_vvcopy_asm(n_size, B, C);
	}
}

static uint64_t mlp_host_sum(uint64_t n_size, uint64_t m_size) {
  uint64_t sum = 0;
  for (uint64_t m = 0; m < n_size; m++){
    sum += B[m];
  }
  return sum;
}

// Params ---------------------------------------------------------------------
typedef struct Params {
  char* dpu_type;
  int   nr_of_ranks;
  int   input_size_n;
  int   input_size_m;
  int   n_warmup;
  int   n_reps;
}Params;

void usage() {
  fprintf(stderr,
    "\nUsage:  ./program [options]"
    "\n"
    "\nGeneral options:"
    "\n    -h        help"
    "\n    -d <D>    DPU type (default=fsim)"
    "\n    -r <R>    # of ranks (default=2)"
    "\n"
    "\nBenchmark-specific options:"
    "\n    -i <I>    input size (default=8M elements)"
    "\n");
  }

  struct Params input_params(int argc, char **argv) {
    struct Params p;
    p.dpu_type      = "fsim";
    p.nr_of_ranks   = 1;
    p.input_size_n  = 1 << 9;
    p.input_size_m  = 1 << 9;
    p.n_warmup      = 2;
    p.n_reps        = 3;

    int opt;
    while((opt = getopt(argc, argv, "hd:r:i:")) >= 0) {
      switch(opt) {
        case 'h':printf("\n");
        usage();
        exit(0);
        break;
        case 'd': p.dpu_type        = optarg; break;
        case 'r': p.nr_of_ranks     = atoi(optarg); break;
        case 'n': p.input_size_n    = atoi(optarg); break;
        case 'm': p.input_size_m    = atoi(optarg); break;
        default:
        fprintf(stderr, "\nUnrecognized option!\n");
        usage();
        exit(0);
      }
    }
    assert(p.nr_of_ranks > 0 && "Invalid # of ranks!");

    return p;
  }

  /**
  * @brief Main of the Host Application.
  */
  int main(int argc, char **argv) {
    if (mlockall(MCL_CURRENT | MCL_FUTURE)) {
      perror("mlockall failed:");
      return 0;
    }

    struct Params p = input_params(argc, argv);
    uint64_t n_size = 8192;
    uint64_t m_size = 20480;
    //uint64_t n_size = 20000;
    //uint64_t m_size = 20000;

    Timer timer;
    A = malloc(NUM_LAYERS * sizeof(T*));
    for(int l = 0; l < NUM_LAYERS; l++)
        A[l] = malloc(n_size*m_size*sizeof(unsigned int));
    B = malloc(m_size*sizeof(unsigned int));
    C = malloc(m_size*sizeof(unsigned int));

    // Create an input file with arbitrary data.
    init_data(A, B, m_size, n_size);

    start(&timer, 0, 0);
    mlp_host(C, A, B, n_size, m_size, 1);
    stop(&timer, 0);

    free(A);
    free(C);
    A = malloc(NUM_LAYERS * sizeof(T*));
    for(int l = 0; l < NUM_LAYERS; l++)
        A[l] = malloc(n_size*m_size*sizeof(unsigned int));
    B2 = malloc(m_size*sizeof(unsigned int));
    C = malloc(m_size*sizeof(unsigned int));

    // Create an input file with arbitrary data.
    init_data(A, B2, m_size, n_size);

    start(&timer, 1, 0);
    mlp_hwacha(C, A, B2, n_size, m_size);
    stop(&timer, 1);

    free(A);
    free(C);
    A = malloc(NUM_LAYERS * sizeof(T*));
    for(int l = 0; l < NUM_LAYERS; l++)
        A[l] = malloc(n_size*m_size*sizeof(unsigned int));
    B3 = malloc(m_size*sizeof(unsigned int));
    C = malloc(m_size*sizeof(unsigned int));

    // Create an input file with arbitrary data.
    init_data(A, B3, m_size, n_size);

    start(&timer, 2, 0);
    mlp_host(C, A, B3, n_size, m_size, 4);
    stop(&timer, 2);

    bool correct = true;
    bool hwacha_wrong = false;
    for (int i = 0; i < n_size; i++) {
        if (B[i] != B2[i] || B[i] != B3[i]) {
            correct = false;
	    if (B[i] != B2[i]) hwacha_wrong = true;
            break;
        }
    }

    printf("******************************\n");
    if (correct) {
        printf("Hwacha works correctly.\n");
    } else if (hwacha_wrong) {
        printf("\n*** Hwacha outputs wrong result! ***\n");

        printf("<<< Results >>>\n");
        for (int i = 0; i < n_size; i++) {
          printf("%d : %d %d\n", i, B[i], B2[i]);
        }
        printf("\n");
    } else {
	    printf("Hwacha works well, but multi-threading doesn't.\n");
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

    for(int l = 0; l < NUM_LAYERS; l++)
        free(A[l]);
    free(A);
    free(B);
    free(B2);
    free(B3);
    free(C);

    return 0;
}
