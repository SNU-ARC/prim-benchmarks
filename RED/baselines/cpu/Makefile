all:
	g++ -O2 app_baseline.cpp -fopenmp -DTHRUST_HOST_SYSTEM=THRUST_HOST_SYSTEM_CPP -DTHRUST_DEVICE_SYSTEM=THRUST_DEVICE_SYSTEM_OMP -lgomp -I/usr/local/cuda-8.0/include -lm -o red -D${TYPE}
clean:
	rm red
thrust:
	riscv64-unknown-linux-gnu-g++ -D INT32 -march=rv64gcxhwacha -fopenmp vec_vvred_main.cpp vec_vvred_asm.S -o red -static -DTHRUST_HOST_SYSTEM=THRUST_HOST_SYSTEM_CPP -DTHRUST_DEVICE_SYSTEM=THRUST_DEVICE_SYSTEM_OMP -lgomp -lm -I/home2/haeun/Share_Local/haeun/chipyard/thrust
spike:
	spike --extension=hwacha pk red
transfer:
	scp -i ../../../../firesim.pem red centos@34.213.49.152:/home/centos/firesim/sw/firesim-software/workloads/hwacha_fedora/overlay/root/modify
firesim:
	riscv64-unknown-linux-gnu-g++ -D INT32 -march=rv64gcxhwacha -fopenmp app_baseline.cpp vec_vvred_asm.S -o red
