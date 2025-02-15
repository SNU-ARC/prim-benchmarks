#define LIMIT -999
#define TRACE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>
#include <sys/mman.h>
#include "../../support/timer.h"
#define OPENMP
//#define NUM_THREAD 4

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define BLOCK_SIZE 16

////////////////////////////////////////////////////////////////////////////////
// declaration, forward
void runTest( int argc, char** argv);

extern "C" void vec_vvcopy_asm(int, int* dest, int* source);

#ifdef OMP_OFFLOAD
#pragma omp declare target
#endif
int maximum( int a,
        int b,
        int c){

    int k;
    if( a <= b )
        k = b;
    else 
        k = a;

    if( k <=c )
        return(c);
    else
        return(k);
}

#ifdef OMP_OFFLOAD
#pragma omp end declare target
#endif

int blosum62[24][24] = {
    { 4, -1, -2, -2,  0, -1, -1,  0, -2, -1, -1, -1, -1, -2, -1,  1,  0, -3, -2,  0, -2, -1,  0, -4},
    {-1,  5,  0, -2, -3,  1,  0, -2,  0, -3, -2,  2, -1, -3, -2, -1, -1, -3, -2, -3, -1,  0, -1, -4},
    {-2,  0,  6,  1, -3,  0,  0,  0,  1, -3, -3,  0, -2, -3, -2,  1,  0, -4, -2, -3,  3,  0, -1, -4},
    {-2, -2,  1,  6, -3,  0,  2, -1, -1, -3, -4, -1, -3, -3, -1,  0, -1, -4, -3, -3,  4,  1, -1, -4},
    { 0, -3, -3, -3,  9, -3, -4, -3, -3, -1, -1, -3, -1, -2, -3, -1, -1, -2, -2, -1, -3, -3, -2, -4},
    {-1,  1,  0,  0, -3,  5,  2, -2,  0, -3, -2,  1,  0, -3, -1,  0, -1, -2, -1, -2,  0,  3, -1, -4},
    {-1,  0,  0,  2, -4,  2,  5, -2,  0, -3, -3,  1, -2, -3, -1,  0, -1, -3, -2, -2,  1,  4, -1, -4},
    { 0, -2,  0, -1, -3, -2, -2,  6, -2, -4, -4, -2, -3, -3, -2,  0, -2, -2, -3, -3, -1, -2, -1, -4},
    {-2,  0,  1, -1, -3,  0,  0, -2,  8, -3, -3, -1, -2, -1, -2, -1, -2, -2,  2, -3,  0,  0, -1, -4},
    {-1, -3, -3, -3, -1, -3, -3, -4, -3,  4,  2, -3,  1,  0, -3, -2, -1, -3, -1,  3, -3, -3, -1, -4},
    {-1, -2, -3, -4, -1, -2, -3, -4, -3,  2,  4, -2,  2,  0, -3, -2, -1, -2, -1,  1, -4, -3, -1, -4},
    {-1,  2,  0, -1, -3,  1,  1, -2, -1, -3, -2,  5, -1, -3, -1,  0, -1, -3, -2, -2,  0,  1, -1, -4},
    {-1, -1, -2, -3, -1,  0, -2, -3, -2,  1,  2, -1,  5,  0, -2, -1, -1, -1, -1,  1, -3, -1, -1, -4},
    {-2, -3, -3, -3, -2, -3, -3, -3, -1,  0,  0, -3,  0,  6, -4, -2, -2,  1,  3, -1, -3, -3, -1, -4},
    {-1, -2, -2, -1, -3, -1, -1, -2, -2, -3, -3, -1, -2, -4,  7, -1, -1, -4, -3, -2, -2, -1, -2, -4},
    { 1, -1,  1,  0, -1,  0,  0,  0, -1, -2, -2,  0, -1, -2, -1,  4,  1, -3, -2, -2,  0,  0,  0, -4},
    { 0, -1,  0, -1, -1, -1, -1, -2, -2, -1, -1, -1, -1, -2, -1,  1,  5, -2, -2,  0, -1, -1,  0, -4},
    {-3, -3, -4, -4, -2, -2, -3, -2, -2, -3, -2, -3, -1,  1, -4, -3, -2, 11,  2, -3, -4, -3, -2, -4},
    {-2, -2, -2, -3, -2, -1, -2, -3,  2, -1, -1, -2, -1,  3, -3, -2, -2,  2,  7, -1, -3, -2, -1, -4},
    { 0, -3, -3, -3, -1, -2, -2, -3, -3,  3,  1, -2,  1, -1, -2, -2,  0, -3, -1,  4, -3, -2, -1, -4},
    {-2, -1,  3,  4, -3,  0,  1, -1,  0, -3, -4,  0, -3, -3, -2,  0, -1, -4, -3, -3,  4,  1, -1, -4},
    {-1,  0,  0,  1, -3,  3,  4, -2,  0, -3, -3,  1, -1, -3, -1,  0, -1, -3, -2, -2,  1,  4, -1, -4},
    { 0, -1, -1, -1, -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2,  0,  0, -2, -1, -1, -1, -1, -1, -4},
    {-4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,  1}
};

////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
    int
main( int argc, char** argv) 
{
    runTest( argc, argv);

    return EXIT_SUCCESS;
}

void usage(int argc, char **argv)
{
    fprintf(stderr, "Usage: %s <max_rows/max_cols> <penalty> <num_threads>\n", argv[0]);
    fprintf(stderr, "\t<dimension>      - x and y dimensions\n");
    fprintf(stderr, "\t<penalty>        - penalty(positive integer)\n");
    fprintf(stderr, "\t<num_threads>    - no. of threads\n");
    exit(1);
}

void nw_optimized(int *input_itemsets, int *output_itemsets, int *referrence,
        int max_rows, int max_cols, int penalty)
{
#ifdef OMP_OFFLOAD
    int transfer_size = max_rows * max_cols;
#pragma omp target data map(to: max_cols, penalty, referrence[0:transfer_size]) map(input_itemsets[0:transfer_size])

    {

#pragma omp target 
#endif
        for( int blk = 1; blk <= (max_cols-1)/BLOCK_SIZE; blk++ )
        {
/*
#ifdef OPENMP
#pragma omp parallel for schedule(static) shared(input_itemsets, referrence) firstprivate(blk, max_rows, max_cols, penalty)
#endif
*/

            for( int b_index_x = 0; b_index_x < blk; ++b_index_x)
            {
                int b_index_y = blk - 1 - b_index_x;
                int input_itemsets_l[(BLOCK_SIZE + 1) *(BLOCK_SIZE+1)] __attribute__ ((aligned (64)));
                int reference_l[BLOCK_SIZE * BLOCK_SIZE] __attribute__ ((aligned (64)));

                // Copy referrence to local memory
                for ( int i = 0; i < BLOCK_SIZE; ++i )
                {
                    for ( int j = 0; j < BLOCK_SIZE; ++j)
                    {
                        reference_l[i*BLOCK_SIZE + j] = referrence[max_cols*(b_index_y*BLOCK_SIZE + i + 1) + b_index_x*BLOCK_SIZE +  j + 1];
                    }
                }

                // Copy input_itemsets to local memory
                for ( int i = 0; i < BLOCK_SIZE + 1; ++i )
                {
                    for ( int j = 0; j < BLOCK_SIZE + 1; ++j)
                    {
                        input_itemsets_l[i*(BLOCK_SIZE + 1) + j] = input_itemsets[max_cols*(b_index_y*BLOCK_SIZE + i) + b_index_x*BLOCK_SIZE +  j];
                    }
                }

                // Compute
                for ( int i = 1; i < BLOCK_SIZE + 1; ++i )
                {
                    for ( int j = 1; j < BLOCK_SIZE + 1; ++j)
                    {
                        input_itemsets_l[i*(BLOCK_SIZE + 1) + j] = maximum( input_itemsets_l[(i - 1)*(BLOCK_SIZE + 1) + j - 1] + reference_l[(i - 1)*BLOCK_SIZE + j - 1],
                                input_itemsets_l[i*(BLOCK_SIZE + 1) + j - 1] - penalty,
                                input_itemsets_l[(i - 1)*(BLOCK_SIZE + 1) + j] - penalty);
                    }
                }

                // Copy results to global memory
                for ( int i = 0; i < BLOCK_SIZE; ++i )
                {
                    for ( int j = 0; j < BLOCK_SIZE; ++j)
                    {
                        input_itemsets[max_cols*(b_index_y*BLOCK_SIZE + i + 1) + b_index_x*BLOCK_SIZE +  j + 1] = input_itemsets_l[(i + 1)*(BLOCK_SIZE+1) + j + 1];
                    }
                }

            }
        }    

        printf("Processing bottom-right matrix\n");

#ifdef OMP_OFFLOAD
#pragma omp target
#endif 

        for ( int blk = 2; blk <= (max_cols-1)/BLOCK_SIZE; blk++ )
        {
/*
#ifdef OPENMP
#pragma omp parallel for schedule(static) shared(input_itemsets, referrence) firstprivate(blk, max_rows, max_cols, penalty)
#endif
*/

            for( int b_index_x = blk - 1; b_index_x < (max_cols-1)/BLOCK_SIZE; ++b_index_x)
            {
                int b_index_y = (max_cols-1)/BLOCK_SIZE + blk - 2 - b_index_x;

                int input_itemsets_l[(BLOCK_SIZE + 1) *(BLOCK_SIZE+1)] __attribute__ ((aligned (64)));
                int reference_l[BLOCK_SIZE * BLOCK_SIZE] __attribute__ ((aligned (64)));

                // Copy referrence to local memory
                for ( int i = 0; i < BLOCK_SIZE; ++i )
                {
                    for ( int j = 0; j < BLOCK_SIZE; ++j)
                    {
                        reference_l[i*BLOCK_SIZE + j] = referrence[max_cols*(b_index_y*BLOCK_SIZE + i + 1) + b_index_x*BLOCK_SIZE +  j + 1];
                    }
                }

                // Copy input_itemsets to local memory
                for ( int i = 0; i < BLOCK_SIZE + 1; ++i )
                {
                    for ( int j = 0; j < BLOCK_SIZE + 1; ++j)
                    {
                        input_itemsets_l[i*(BLOCK_SIZE + 1) + j] = input_itemsets[max_cols*(b_index_y*BLOCK_SIZE + i) + b_index_x*BLOCK_SIZE +  j];
                    }
                }

                // Compute
                for ( int i = 1; i < BLOCK_SIZE + 1; ++i )
                {
                    for ( int j = 1; j < BLOCK_SIZE + 1; ++j)
                    {
                        input_itemsets_l[i*(BLOCK_SIZE + 1) + j] = maximum( input_itemsets_l[(i - 1)*(BLOCK_SIZE + 1) + j - 1] + reference_l[(i - 1)*BLOCK_SIZE + j - 1],
                                input_itemsets_l[i*(BLOCK_SIZE + 1) + j - 1] - penalty,
                                input_itemsets_l[(i - 1)*(BLOCK_SIZE + 1) + j] - penalty);
                    }
                }

                // Copy results to global memory
                for ( int i = 0; i < BLOCK_SIZE; ++i )
                {
                    for ( int j = 0; j < BLOCK_SIZE; ++j)
                    {
                        input_itemsets[max_cols*(b_index_y*BLOCK_SIZE + i + 1) + b_index_x*BLOCK_SIZE +  j + 1] = input_itemsets_l[(i + 1)*(BLOCK_SIZE+1) + j +1];
                    }
                }
            }
        }

#ifdef OMP_OFFLOAD
    }
#endif
}

void nw_optimized_hwacha(int *input_itemsets, int *output_itemsets, int *referrence,
        int max_rows, int max_cols, int penalty)
{
    for( int blk = 1; blk <= (max_cols-1)/BLOCK_SIZE; blk++ )
    {
        for( int b_index_x = 0; b_index_x < blk; ++b_index_x)
        {
            int b_index_y = blk - 1 - b_index_x;
            int input_itemsets_l[(BLOCK_SIZE + 1) *(BLOCK_SIZE+1)] __attribute__ ((aligned (64)));
            int reference_l[BLOCK_SIZE * BLOCK_SIZE] __attribute__ ((aligned (64)));

            // Copy referrence to local memory
            for ( int i = 0; i < BLOCK_SIZE; ++i )
            {
                // reference_l[i*BLOCK_SIZE + j] = referrence[max_cols*(b_index_y*BLOCK_SIZE + i + 1) + b_index_x*BLOCK_SIZE +  j + 1];
                //vec_vvcopy_asm(BLOCK_SIZE, &reference_l[i*BLOCK_SIZE], &referrence[max_cols*(b_index_y*BLOCK_SIZE + i + 1) + b_index_x*BLOCK_SIZE + 1]);
            }

            // Copy input_itemsets to local memory
            for ( int i = 0; i < BLOCK_SIZE + 1; ++i )
            {
                // input_itemsets_l[i*(BLOCK_SIZE + 1) + j] = input_itemsets[max_cols*(b_index_y*BLOCK_SIZE + i) + b_index_x*BLOCK_SIZE +  j];
                //vec_vvcopy_asm(BLOCK_SIZE + 1, &input_itemsets_l[i*(BLOCK_SIZE + 1)], &input_itemsets[max_cols*(b_index_y*BLOCK_SIZE + i) + b_index_x*BLOCK_SIZE]);
            }

            // Compute
            for ( int i = 1; i < BLOCK_SIZE + 1; ++i )
            {    
                for ( int j = 1; j < BLOCK_SIZE + 1; ++j)
                {
                    input_itemsets_l[i*(BLOCK_SIZE + 1) + j] = maximum( 
                        input_itemsets_l[(i - 1)*(BLOCK_SIZE + 1) + j - 1] + reference_l[(i - 1)*BLOCK_SIZE + j - 1],
                        input_itemsets_l[i*(BLOCK_SIZE + 1) + j - 1] - penalty,
                        input_itemsets_l[(i - 1)*(BLOCK_SIZE + 1) + j] - penalty);  
                }
            }

            // Copy results to global memory
            for ( int i = 0; i < BLOCK_SIZE; ++i )
            {
                //input_itemsets[max_cols*(b_index_y*BLOCK_SIZE + i + 1) + b_index_x*BLOCK_SIZE +  j + 1] = input_itemsets_l[(i + 1)*(BLOCK_SIZE+1) + j + 1];
                //vec_vvcopy_asm(BLOCK_SIZE, &input_itemsets[max_cols*(b_index_y*BLOCK_SIZE + i + 1) + b_index_x*BLOCK_SIZE + 1], &input_itemsets_l[(i + 1)*(BLOCK_SIZE+1) + 1]);
            }

        }
    }    

    printf("Processing bottom-right matrix\n");

    for ( int blk = 2; blk <= (max_cols-1)/BLOCK_SIZE; blk++ )
    {
        for( int b_index_x = blk - 1; b_index_x < (max_cols-1)/BLOCK_SIZE; ++b_index_x)
        {
            int b_index_y = (max_cols-1)/BLOCK_SIZE + blk - 2 - b_index_x;

            int input_itemsets_l[(BLOCK_SIZE + 1) *(BLOCK_SIZE+1)] __attribute__ ((aligned (64)));
            int reference_l[BLOCK_SIZE * BLOCK_SIZE] __attribute__ ((aligned (64)));

            // Copy referrence to local memory
            for ( int i = 0; i < BLOCK_SIZE; ++i )
            {
                //reference_l[i*BLOCK_SIZE + j] = referrence[max_cols*(b_index_y*BLOCK_SIZE + i + 1) + b_index_x*BLOCK_SIZE +  j + 1];
                //vec_vvcopy_asm(BLOCK_SIZE, &reference_l[i*BLOCK_SIZE], &referrence[max_cols*(b_index_y*BLOCK_SIZE + i + 1) + b_index_x*BLOCK_SIZE + 1]);
            }

            // Copy input_itemsets to local memory
            for ( int i = 0; i < BLOCK_SIZE + 1; ++i )
            {
                //input_itemsets_l[i*(BLOCK_SIZE + 1) + j] = input_itemsets[max_cols*(b_index_y*BLOCK_SIZE + i) + b_index_x*BLOCK_SIZE +  j];
                //vec_vvcopy_asm(BLOCK_SIZE + 1, &input_itemsets_l[i*(BLOCK_SIZE + 1)], &input_itemsets[max_cols*(b_index_y*BLOCK_SIZE + i) + b_index_x*BLOCK_SIZE]);
            }

            // Compute
            for ( int i = 1; i < BLOCK_SIZE + 1; ++i )
            {
                for (int j = 1; j < BLOCK_SIZE + 1; ++j) {
                    input_itemsets_l[i*(BLOCK_SIZE + 1) + j] = maximum( 
                        input_itemsets_l[(i - 1)*(BLOCK_SIZE + 1) + j - 1] + reference_l[(i - 1)*BLOCK_SIZE + j - 1],
                        input_itemsets_l[i*(BLOCK_SIZE + 1) + j - 1] - penalty,
                        input_itemsets_l[(i - 1)*(BLOCK_SIZE + 1) + j] - penalty);
                }
            }

            // Copy results to global memory
            for ( int i = 0; i < BLOCK_SIZE; ++i )
            {
                //input_itemsets[max_cols*(b_index_y*BLOCK_SIZE + i + 1) + b_index_x*BLOCK_SIZE +  j + 1] = input_itemsets_l[(i + 1)*(BLOCK_SIZE+1) + j +1];
                //vec_vvcopy_asm(BLOCK_SIZE, &input_itemsets[max_cols*(b_index_y*BLOCK_SIZE + i + 1) + b_index_x*BLOCK_SIZE + 1], &input_itemsets_l[(i + 1)*(BLOCK_SIZE+1) + 1]);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//! Run a simple test for CUDA
////////////////////////////////////////////////////////////////////////////////
    void
runTest( int argc, char** argv) 
{
    if (mlockall(MCL_CURRENT | MCL_FUTURE)) {
        perror("mlockall failed:");
        return;
    }

    int max_rows, max_cols, penalty;
    int *input_itemsets, *output_itemsets, *referrence;
    int *input_itemsets_hwacha, *input_itemsets_4threads;
    //int *matrix_cuda, *matrix_cuda_out, *referrence_cuda;
    //int size;
    int omp_num_threads;

    // the lengths of the two sequences should be able to divided by 16.
    // And at current stage  max_rows needs to equal max_cols
    if (argc == 4)
    {
        max_rows = atoi(argv[1]);
        max_cols = atoi(argv[1]);
        penalty = atoi(argv[2]);
        omp_num_threads = atoi(argv[3]);
    }
    else{
        usage(argc, argv);
    }

    max_rows = max_rows + 1;
    max_cols = max_cols + 1;
    referrence = (int *)malloc( max_rows * max_cols * sizeof(int) );
    input_itemsets = (int *)malloc( max_rows * max_cols * sizeof(int) );
    input_itemsets_hwacha = (int *)malloc( max_rows * max_cols * sizeof(int) );
    input_itemsets_4threads = (int *)malloc( max_rows * max_cols * sizeof(int) );
    output_itemsets = (int *)malloc( max_rows * max_cols * sizeof(int) );

    if (!input_itemsets)
        fprintf(stderr, "error: can not allocate memory");

    srand ( 7 );

    for (int i = 0 ; i < max_cols; i++){
        for (int j = 0 ; j < max_rows; j++){
            input_itemsets[i*max_cols+j] = 0;
            input_itemsets_hwacha[i*max_cols+j] = 0;
            input_itemsets_4threads[i*max_cols+j] = 0;
        }
    }

    printf("Start Needleman-Wunsch\n");

    for( int i=1; i< max_rows ; i++){    //please define your own sequence. 
        input_itemsets[i*max_cols] = input_itemsets_hwacha[i*max_cols] = input_itemsets_4threads[i*max_cols] = rand() % 10 + 1;
    }
    for( int j=1; j< max_cols ; j++){    //please define your own sequence.
        input_itemsets[j] = input_itemsets_hwacha[j] = input_itemsets_4threads[j] = rand() % 10 + 1;
    }

    for (int i = 1 ; i < max_cols; i++){
        for (int j = 1 ; j < max_rows; j++){
            referrence[i*max_cols+j] = blosum62[input_itemsets[i*max_cols]][input_itemsets[j]];
        }
    }

    for( int i = 1; i< max_rows ; i++)
        input_itemsets[i*max_cols] = input_itemsets_hwacha[i*max_cols] = input_itemsets_4threads[i*max_cols] = -i * penalty;
    for( int j = 1; j< max_cols ; j++)
        input_itemsets[j] = input_itemsets_hwacha[j] = input_itemsets_4threads[j] = -j * penalty;

    //Compute top-left matrix 
    printf("Num of threads: %d\n", omp_num_threads);
    printf("Processing top-left matrix\n");

    Timer timer;
    omp_set_num_threads(1);
    start(&timer, 0, 0);
    nw_optimized( input_itemsets, output_itemsets, referrence,
            max_rows, max_cols, penalty );
    stop(&timer, 0);

    start(&timer, 1, 0);
    nw_optimized_hwacha( input_itemsets_hwacha, output_itemsets, referrence,
            max_rows, max_cols, penalty );
    stop(&timer, 1);

    omp_set_num_threads(4);
    start(&timer, 2, 0);
    nw_optimized( input_itemsets_4threads, output_itemsets, referrence,
            max_rows, max_cols, penalty );
    stop(&timer, 2);

    bool correct = true;
    for (int i = 0; i < max_rows*max_cols; i++) {
        if (input_itemsets[i] != input_itemsets_hwacha[i]) {
            printf("Diff hwacha %d : %d %d\n", i, input_itemsets[i], input_itemsets_hwacha[i]);
            correct = false;
        }

        if (input_itemsets[i] != input_itemsets_4threads[i]) {
            printf("Diff 4 threads %d : %d %d\n", i, input_itemsets[i], input_itemsets_4threads[i]);
        }
    }

    if (correct) {
        printf("[" ANSI_COLOR_GREEN "OK" ANSI_COLOR_RESET "] Outputs are equal\n");
    } else {
        printf("[" ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET "] Outputs differ!\n");
    }

    printf("CPU ");
    print(&timer, 0, 1);
    printf("\n");
    printf("Hwacha ");
    print(&timer, 1, 1);
    printf("\n");
    printf("4 threads ");
    print(&timer, 2, 1);
    printf("\n");

#define TRACEBACK
#ifdef TRACEBACK

    FILE *fpo = fopen("result.txt","w");
    fprintf(fpo, "print traceback value GPU:\n");

    for (int i = max_rows - 2,  j = max_rows - 2; i>=0, j>=0;){
        int nw, n, w, traceback;
        if ( i == max_rows - 2 && j == max_rows - 2 )
            fprintf(fpo, "%d ", input_itemsets[ i * max_cols + j]); //print the first element
        if ( i == 0 && j == 0 )
            break;
        if ( i > 0 && j > 0 ){
            nw = input_itemsets[(i - 1) * max_cols + j - 1];
            w  = input_itemsets[ i * max_cols + j - 1 ];
            n  = input_itemsets[(i - 1) * max_cols + j];
        }
        else if ( i == 0 ){
            nw = n = LIMIT;
            w  = input_itemsets[ i * max_cols + j - 1 ];
        }
        else if ( j == 0 ){
            nw = w = LIMIT;
            n  = input_itemsets[(i - 1) * max_cols + j];
        }
        else{
        }

        //traceback = maximum(nw, w, n);
        int new_nw, new_w, new_n;
        new_nw = nw + referrence[i * max_cols + j];
        new_w = w - penalty;
        new_n = n - penalty;

        traceback = maximum(new_nw, new_w, new_n);
        if(traceback == new_nw)
            traceback = nw;
        if(traceback == new_w)
            traceback = w;
        if(traceback == new_n)
            traceback = n;

        fprintf(fpo, "%d ", traceback);

        if(traceback == nw )
        {i--; j--; continue;}

        else if(traceback == w )
        {j--; continue;}

        else if(traceback == n )
        {i--; continue;}

        else
            ;
    }

    fclose(fpo);

#endif

    free(referrence);
    free(input_itemsets);
    free(output_itemsets);

    munlockall();
}
