
CC=gcc -O3 -fopenmp
RM=rm -f

all: cpu cpu_long memory disk

cpu: cpu_benchmark

cpu_long: cpu_long_benchmark

memory: memory_benchmark

disk: disk_benchmark
	


