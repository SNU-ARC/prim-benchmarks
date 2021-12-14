all:
	gcc bs_omp.c -o bs_omp -fopenmp -O2 -ftree-vectorize -fopt-info
run:
	./bs_omp 262144 16777216
