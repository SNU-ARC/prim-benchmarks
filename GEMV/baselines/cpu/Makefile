all:
	gcc -o gcc_all -fopenmp gemv_openmp.c -O2 -ftree-vectorize -fopt-info
clean:
	rm gemv
spike:
	spike --extension=hwacha pk gemv
transfer:
	scp -i ../../../../firesim.pem gemv centos@52.32.98.71:/home/centos/firesim/sw/firesim-software/workloads/hwacha_fedora/overlay/root/VA
firesim:
	riscv64-unknown-linux-gnu-gcc -march=rv64gcxhwacha -fopenmp -static vec_dgemm_opt.c vec_vvgem_asm.S gemv_openmp.c -o gemv
icc:
	icc -O2 -fopenmp -qopt-report=1 -qopt-report-phase=vec gemv_openmp.c
