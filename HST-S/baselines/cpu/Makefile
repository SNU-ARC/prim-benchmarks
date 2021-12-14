all:
	gcc -S -fopenmp app_baseline.c -O2 -ftree-vectorize -fopt-info
clean:
	rm hist
firesim:
	riscv64-unknown-linux-gnu-gcc -march=rv64gcxhwacha -fopenmp vec_vvhst_asm.S app_baseline.c -static -o hst
spike:
	spike --extension=hwacha pk hst
transfer:
	scp -i ../../../../firesim.pem hst image_VanHateren.iml centos@52.32.98.71:/home/centos/firesim/sw/firesim-software/workloads/hwacha_fedora/overlay/root/VA
