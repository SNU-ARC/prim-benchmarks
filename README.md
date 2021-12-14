This directory contains vectorized version PrIM workloads.
(In other words, it is patched with hwacha instructions.)

All modifications are done at ./$TASK/baselines/cpu, where $TASK is BFS as an example.
The patched code aims to run hwacha instructions at built image with hwacha processor at AWS firesim.
Commands such as 'make transfer' transfers cross-compiled binary file to aws server.
One needs to modify Makefile ip address of aws server to use their server.
'make firesim' command cross-compiles files to make binary file of RISC-V ISA.
'make spike' command runs simulation of built binary using spike.
However, spike cannot test multi-threading. 
Thus, one needs to remove all omp.h related codes before compiling binary to be tested on spike.
Cross-compilation assumes toolchians for compiling hwacha instructions are pre-installed.
Note that not all tasks can be patched with hwacha instructions. 
(Not all tasks can be vectorized, or more efficient when vectorized.)
Thus, some directories remain unchanged. 
Currently vectorized kernels are : VA, RED, MLP, HST-S, GEMV, SpMV.

Hwacha currently seems to have some memory consistency issue when dealing with small size dataset.
Thus, vectorization is tested only for large dataset that does not fit into L1 cache of Rocket core.
Also, since hwacha currently does not support page faults, we have used mlockall.

README.md of PrIM benchmarks can be found at https://github.com/CMU-SAFARI/prim-benchmarks.
