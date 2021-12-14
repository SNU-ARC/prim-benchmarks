all:
	gcc -o va -fopenmp app_baseline.c 
clean:
	rm va
firesim:
	riscv64-unknown-linux-gnu-gcc -march=rv64gcxhwacha -fopenmp vec_vvadd_asm.S vec_vvadd_main.c -static -o va
spike:
	spike --extension=hwacha pk va
transfer:
	scp -i ../../../../firesim.pem va centos@52.32.98.71:/home/centos/firesim/sw/firesim-software/workloads/hwacha_fedora/overlay/root/VA
