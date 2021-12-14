all:
	g++ -O2 app_baseline.cpp -fopenmp -DTHRUST_HOST_SYSTEM=THRUST_HOST_SYSTEM_CPP -DTHRUST_DEVICE_SYSTEM=THRUST_DEVICE_SYSTEM_OMP -lgomp -I/usr/local/cuda-8.0/include -lm -o red -D${TYPE}
clean:
	rm red
thrust:
	riscv64-unknown-linux-gnu-g++ -D INT32 -march=rv64gcxhwacha -fopenmp vec_vvred_main.cpp vec_vvred_asm.S -o red -static  -DTHRUST_HOST_SYSTEM=THRUST_HOST_SYSTEM_CPP -DTHRUST_DEVICE_SYSTEM=THRUST_DEVICE_SYSTEM_OMP -lgomp -lm -I/home2/haeun/Share_Local/haeun/chipyard/thrust
spike:
	spike --extension=hwacha pk red
transfer:
	scp -i ../../../../firesim.pem red centos@52.32.98.71:/home/centos/firesim/sw/firesim-software/workloads/hwacha_fedora/overlay/root/VA
firesim:
	riscv64-unknown-linux-gnu-g++ -D INT32 -march=rv64gcxhwacha -fopenmp vec_vvred_main.cpp vec_vvred_asm.S -o red -static
