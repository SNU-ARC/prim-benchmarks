all:
	gcc -o spmv -fopenmp app.c -O2 -ftree-vectorize -fopt-info
clean:
	rm spmv spmv_spike
firesim:
	riscv64-unknown-linux-gnu-gcc -march=rv64gcxhwacha -fopenmp -static vec_vvspmv_asm.S app.c -o spmv
spike:
	spike --extension=hwacha pk spmv -f ../../data/bcsstk30.mtx 
transfer:
	scp -i ../../../../firesim.pem spmv centos@52.32.98.71:/home/centos/firesim/sw/firesim-software/workloads/hwacha_fedora/overlay/root/VA
