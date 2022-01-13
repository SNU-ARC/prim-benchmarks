# C compiler
CC = g++
ICC = icc
CC_FLAGS = -g -O2 -fopenmp
OFFLOAD_CC_FLAGS = -qoffload-option,mic,compiler,"-no-opt-prefetch"

all: needle needle_offload

needle: 
	$(CC) $(CC_FLAGS) needle.cpp -o needle

needle_offload:
	$(ICC) $(CC_FLAGS) $(OFFLOAD_CC_FLAGS) -DOMP_OFFLOAD needle.cpp -o needle_offload

clean:
	rm -f needle needle_offload

firesim:
	riscv64-unknown-linux-gnu-g++ -march=rv64gcxhwacha -fopenmp -DOMP_OFFLOAD needle.cpp vec_vvnw_asm.S -o nw -static
spike:
	spike --extension=hwacha pk nw 46080 10 4
transfer:
	scp -i ../../../../firesim.pem nw centos@52.32.98.71:/home/centos/firesim/sw/firesim-software/workloads/hwacha_fedora/overlay/root/VA
icc:
	icc -O2 -fopenmp -qopt-report=1 -qopt-report-phase=vec needle.cpp
