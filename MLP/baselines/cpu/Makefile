all:
	gcc mlp_openmp.c -o mlp -fopenmp -std=c99 -O2 -ftree-vectorize -fopt-info
run: 
	./mlp
firesim:
	riscv64-unknown-linux-gnu-gcc -march=rv64gcxhwacha -fopenmp -std=c99 vec_vvmlp_asm.S mlp_openmp.c -static -o mlp
spike:
	spike --extension=hwacha pk mlp
transfer:
	scp -i ../../../../firesim.pem mlp centos@52.32.98.71:/home/centos/firesim/sw/firesim-software/workloads/hwacha_fedora/overlay/root/VA
