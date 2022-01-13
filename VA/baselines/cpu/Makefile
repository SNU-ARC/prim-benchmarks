all:
	gcc -o va -fopenmp app_baseline.c -O2 -ftree-vectorize -fopt-info
clean:
	rm va
firesim:
	riscv64-unknown-linux-gnu-gcc -march=rv64gcxhwacha -fopenmp vec_vvadd_asm.S app_baseline.c -o va -static
spike:
	spike --extension=hwacha pk va
transfer:
	scp -i ../../../../firesim.pem va centos@34.213.49.152:/home/centos/firesim/sw/firesim-software/workloads/hwacha_fedora/overlay/root/modify
