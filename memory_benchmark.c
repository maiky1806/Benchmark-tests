//Memory benchmark
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>

#define SEQUENTIAL 0
#define RANDOM 1
#define NUM_RANDOM 1.0E7

#define BYTE 0
#define KILOBYTE 1
#define MEGABYTE 2



int main(int argc, char** argv){

	char *array1, *array2, *array3;
	int index=0, chunkSize, numThreads;
	int accessType;
	double size, i=0, loops=NUM_RANDOM;

	//Create the time structure  
   	struct timeval start_t, final_t;

	//Read the arguments to set the parameters
	if (argc == 4){
		chunkSize=atoi(argv[1]);
		accessType=atoi(argv[2]);
		numThreads=atoi(argv[3]);
	}else{
		printf("Please use: memory_benchmark <blockSize> <accessType> <numThreads>\n");
		printf("Block Sizes: - 0: BYTE\t- 1: KILOBYTE\t - 2: MEGABYTE\n");
		printf("Access Type: - 0: SEQUENTIAL\t- 1: RANDOM\n");
		return 0;
	}

	//Define cache size and array size
	int CACHE_SIZE = 1024*1024*25;
	int ARRAY_SIZE = CACHE_SIZE*12;
	//Allocate arrays in memory
	array1 = malloc(ARRAY_SIZE);
	array2 = malloc(ARRAY_SIZE);


	//Size for the test
	switch(chunkSize){
		case BYTE:
			size=1;
			break;
		case KILOBYTE:
			size=1e3;
			break;
		case MEGABYTE:
			size=1E6;
			//Limit for MB size (too much time)
			loops=NUM_RANDOM/100;
			break;
	}


	//Run test depending on the type of access
	if (accessType == RANDOM){
		int numBlocks = ARRAY_SIZE/size;
		int r;
		//Get initial time
		gettimeofday(&start_t,NULL);
		omp_set_num_threads(numThreads);
		#pragma omp parallel shared (i)
		{
			while(i<loops){
				r = index %2;
				//If even, else odd
				if (r==0)
					memcpy(&array1[index], &array2[index], size);
				else
					memcpy(&array2[index], &array1[index], size);
				//Random number modulo numblocks to obtain next index
				index = rand() % numBlocks;

				//Operation counter
				i+=1;
			}
		}

	}else{
		//Compute the increment for a sequential access and the end of the file to stop
		int end = ARRAY_SIZE;
		//Get initial time
		gettimeofday(&start_t,NULL);
		omp_set_num_threads(numThreads);
		#pragma omp parallel shared (i)
		{
			while(end > index+size){
				//Read+write sequential operations per cicle
				memcpy(&array1[index], &array2[index], size);
				index = index+size;
				//operation counter
				i+=1;
			}
		}	

	}

	//Get final time and compute results
   	gettimeofday(&final_t,NULL);
	double t1= (double) start_t.tv_sec + start_t.tv_usec/1000000.0;
	double t = ((double) final_t.tv_sec + final_t.tv_usec/1000000.0) - t1;

	//Throughput in MB/s
	double throughput = size*i/t/(1024*1024);

	//Latency in ms
	double latency = t*1e6/i;

	char *type[2] = {"SEQUENTIAL", "RANDOM"};
	char *blocks[3] = {"BYTE", "KB", "MB"};
	printf("Throughput: %.1f MB/s, Latency: %.3f us \t-> Block size: %s.   Type access: %s.   %d threads\n", throughput, latency, blocks[chunkSize], type[accessType], numThreads);
	free(array1);
	free(array2);

	return 0;
}



