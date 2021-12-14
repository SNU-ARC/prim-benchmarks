
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <omp.h>
#include <sys/mman.h>

#include "../../support/matrix.h"
#include "../../support/params.h"
#include "../../support/timer.h"
#include "../../support/utils.h"

int main(int argc, char** argv) {
    if (mlockall(MCL_CURRENT | MCL_FUTURE)) {
        perror("mlockall failed:");
        return 0;
    }

    // Process parameters
    struct Params p = input_params(argc, argv);

    // Initialize SpMV data structures
    PRINT_INFO(p.verbosity >= 1, "Reading matrix %s", p.fileName);
    struct COOMatrix cooMatrix = readCOOMatrix(p.fileName);
    PRINT_INFO(p.verbosity >= 1, "    %u rows, %u columns, %u nonzeros", cooMatrix.numRows, cooMatrix.numCols, cooMatrix.numNonzeros);
    struct CSRMatrix csrMatrix = coo2csr(cooMatrix);
    float* inVector = malloc(csrMatrix.numCols*sizeof(float));
    float* outVector = malloc(csrMatrix.numRows*sizeof(float));
    float* outVector_hwacha = malloc(csrMatrix.numRows*sizeof(float));
    float* outVector_multi = malloc(csrMatrix.numRows*sizeof(float));
    initVector(inVector, csrMatrix.numCols);

    // Calculating result on CPU
    PRINT_INFO(p.verbosity >= 1, "Calculating result on CPU");
    
    Timer timer;
    start(&timer, 0, 0);
    for(uint32_t rowIdx = 0; rowIdx < csrMatrix.numRows; ++rowIdx) {
        float sum = 0.0f;
        for(uint32_t i = csrMatrix.rowPtrs[rowIdx]; i < csrMatrix.rowPtrs[rowIdx + 1]; ++i) {
            uint32_t colIdx = csrMatrix.nonzeros[i].col;
            float value = csrMatrix.nonzeros[i].value;
            sum += inVector[colIdx]*value;
        }
        outVector[rowIdx] = sum;
    }
    stop(&timer, 0);
    
    start(&timer, 1, 0);
    for(uint32_t rowIdx = 0; rowIdx < csrMatrix.numRows; ++rowIdx) {
        float sum = 0.0f;
        uint32_t start = csrMatrix.rowPtrs[rowIdx];
        uint32_t end = csrMatrix.rowPtrs[rowIdx + 1];
        float * result = (float *)malloc(sizeof(float)*(end - start));
        for (int i = 0; i < end - start; i++) result[i] = (float) 0.0;
        int size = vec_vvspmv_asm(end - start, result, inVector, &csrMatrix.nonzeros[start].col, &csrMatrix.nonzeros[start].value);
        for (int i = 0; i < size; i++)
            sum += result[i];
        outVector_hwacha[rowIdx] = sum;
        free(result);
    }
    stop(&timer, 1);

    start(&timer, 2, 0);
    omp_set_num_threads(4);
    #pragma omp parallel for
    for(uint32_t rowIdx = 0; rowIdx < csrMatrix.numRows; ++rowIdx) {
        float sum = 0.0f;
        for(uint32_t i = csrMatrix.rowPtrs[rowIdx]; i < csrMatrix.rowPtrs[rowIdx + 1]; ++i) {
            uint32_t colIdx = csrMatrix.nonzeros[i].col;
            float value = csrMatrix.nonzeros[i].value;
            sum += inVector[colIdx]*value;
        }
        outVector_multi[rowIdx] = sum;
    }
    stop(&timer, 2);
    
    //if(p.verbosity == 0) PRINT("%f", getElapsedTime(timer)*1e3);
    //PRINT_INFO(p.verbosity >= 1, "    Elapsed time: %f ms", getElapsedTime(timer)*1e3);

    bool hwacha_correct = true;
    bool multi_correct = true;
    for (int i = 0; i < csrMatrix.numRows; i++) {
        if (outVector[i] != outVector_hwacha[i]) {
            hwacha_correct = false;
        }

        if (outVector[i] != outVector_multi[i]) {
            multi_correct = false;
        }
    }

    printf("******************************\n");
    if (hwacha_correct && multi_correct) {
      printf("Both correct.\n");  
    } else if (!hwacha_correct) {
        printf("Hwacha outputs wrong result!\n");
    } else if (!multi_correct) {
        printf("Multi-threading outputs wrong result!\n");
    } else {
        printf("Both wrong!\n");
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

    // Deallocate data structures
    freeCOOMatrix(cooMatrix);
    freeCSRMatrix(csrMatrix);
    free(inVector);
    free(outVector);

    munlockall();

    return 0;
}
