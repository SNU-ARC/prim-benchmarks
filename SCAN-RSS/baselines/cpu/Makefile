all:
	g++ -O2 app_baseline.cpp -fopenmp -DTHRUST_HOST_SYSTEM=THRUST_HOST_SYSTEM_CPP -DTHRUST_DEVICE_SYSTEM=THRUST_DEVICE_SYSTEM_OMP -lgomp -I/usr/local/cuda-8.0/include -lm -o scan -D${TYPE} -O2 -ftree-vectorize -fopt-info

clean:
	rm scan
