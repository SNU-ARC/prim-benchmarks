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
#include <omp.h>
#include <time.h>
#include <sys/mman.h>

static uint32_t *A;
static uint32_t *B;
static uint32_t *C;
static uint32_t *C2; 

/**
* @brief creates a "test file" by filling a buffer of 64MB with pseudo-random values
* @param nr_elements how many 32-bit elements we want the file to be
* @return the buffer address
*/
void * create_test_file(unsigned int nr_elements, uint32_t ** C_PTR) {
    A = (uint32_t*) malloc(nr_elements * sizeof(uint32_t));
    B = (uint32_t*) malloc(nr_elements * sizeof(uint32_t));
    *C_PTR = (uint32_t*) malloc(nr_elements * sizeof(uint32_t));

    srand(100000);
    
    for (int i = 0; i < nr_elements; i++) {
        A[i] = (int) (rand());
        B[i] = (int) (rand());
    }
}

// Params ---------------------------------------------------------------------
typedef struct Params {
    int   input_size;
    int   n_warmup;
    int   n_reps;
    int   n_threads;
} Params;

void usage() {
    fprintf(stderr,
        "\nUsage:  ./program [options]"
        "\n"
        "\nGeneral options:"
        "\n    -h        help"
        "\n    -t <T>    # of threads (default=8)"
        "\n    -w <W>    # of untimed warmup iterations (default=1)"
        "\n    -e <E>    # of timed repetition iterations (default=3)"
        "\n"
        "\nBenchmark-specific options:"
        "\n    -i <I>    input size (default=8M elements)"
        "\n");
}

struct Params input_params(int argc, char **argv) {
    struct Params p;
    p.input_size    = 16777216;
    p.n_warmup      = 1;
    p.n_reps        = 3;
    p.n_threads     = 5;

    int opt;
    while((opt = getopt(argc, argv, "hi:w:e:t:")) >= 0) {
        switch(opt) {
        case 'h':
        usage();
        exit(0);
        break;
        case 'i': p.input_size    = atoi(optarg); break;
        case 'w': p.n_warmup      = atoi(optarg); break;
        case 'e': p.n_reps        = atoi(optarg); break;
        case 't': p.n_threads        = atoi(optarg); break;
        default:
            fprintf(stderr, "\nUnrecognized option!\n");
            usage();
            exit(0);
        }
    }
    assert(p.n_threads > 0 && "Invalid # of ranks!");

    return p;
}

static void vector_addition_host(unsigned int nr_elements, int t) {
    omp_set_num_threads(t);
    #pragma omp parallel for
    for (int i = 0; i < nr_elements; i++) {
        C2[i] = A[i] + B[i];
    }
}

static void vector_addition_host_tmp(unsigned int nr_elements, uint32_t * C, uint32_t * A, uint32_t * B) {
    for (int i = 0; i < nr_elements; i++) {
        C[i] = A[i] + B[i];
    }
}

/**
* @brief Main of the Host Application.
*/
int main(int argc, char **argv) {
    // Now lock all current and future pages from preventing of being paged
    struct Params p = input_params(argc, argv);
    const unsigned int file_size = p.input_size;

    if (mlockall(MCL_CURRENT | MCL_FUTURE)) {
        perror("mlockall failed:");
        return 0;
    }

    create_test_file(file_size, &C);
    struct timespec begin, end;
    clock_gettime(CLOCK_MONOTONIC, &begin);
    vec_vvadd_asm(file_size, C, A, B);
    clock_gettime(CLOCK_MONOTONIC, &end);
    long time_with_hwacha = (end.tv_sec - begin.tv_sec) * 1000000000 + (end.tv_nsec - begin.tv_nsec);

    free(A);
    free(B);
    free(C2);
    create_test_file(file_size, &C2);
    struct timespec begin2, end2;
    clock_gettime(CLOCK_MONOTONIC, &begin2);
    vector_addition_host(file_size, 1);
    clock_gettime(CLOCK_MONOTONIC, &end2);
    long time_without_hwacha = (end2.tv_sec - begin2.tv_sec) * 1000000000 + (end2.tv_nsec - begin2.tv_nsec);

    free(A);
    free(B);
    free(C2);
    create_test_file(file_size, &C2);
    struct timespec begin3, end3;
    clock_gettime(CLOCK_MONOTONIC, &begin3);
    vector_addition_host(file_size, 4);
    clock_gettime(CLOCK_MONOTONIC, &end3);
    long time_without_hwacha_t4 = (end3.tv_sec - begin3.tv_sec) * 1000000000 + (end3.tv_nsec - begin3.tv_nsec);

    bool correct = true;
    for (int i = 0; i < file_size; i++) {
        if (C[i] != C2[i]) {
            correct = false;
            break;
        }
    }

    printf("******************************\n");
    if (correct) {
        printf("Hwacha works correctly.\n");
    } else {
        printf("Hwacha outputs wrong result!\n");
    }
    printf("******************************\n");
    printf("With hwacha : %ld\n", time_with_hwacha);
    printf("Without hwacha : %ld\n", time_without_hwacha);
    printf("Without hwacha, 4 threads : %ld\n", time_without_hwacha_t4);

    free(A);
    free(B);
    free(C);
    free(C2);

   return 0;
}
