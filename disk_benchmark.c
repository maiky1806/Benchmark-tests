//Disk benchmark
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <omp.h>

#define SEQUENTIAL 0
#define RANDOM 1

#define READ 0
#define WRITE 1

#define NUM_RANDOM 1.0E5

#define BYTE 0
#define KILOBYTE 1
#define MEGABYTE 2

int accessType;
int iotype;
double size;
int condition=1;
double filesize;


int main(int argc, char** argv){

	char *array1, *array2, *filename, *filename2;
	int i=0, index=0, chunkSize, numThreads;
	FILE *fp, *fp2;

	//Create the time structure  
   	struct timeval start_t, final_t;

	//Read the arguments to set the parameters
	if (argc >= 6){
		chunkSize=atoi(argv[1]);
		iotype=atoi(argv[2]);
		accessType=atoi(argv[3]);
		numThreads=atoi(argv[4]);
		filename = argv[5];
		filename2 = argv[6];
	}else{
		printf("Please use: disk_benchmark <blockSize> <IOType> <accessType> <numThreads> <fileTestName> <fileTestname2>\n");
		printf("Block Sizes: - 0: BYTE\t- 1: KILOBYTE\t - 2: MEGABYTE\n");
		printf("IO Operation: - 0: READ\t- 1: WRITE\n");
		printf("Access Type: - 0: SEQUENTIAL\t- 1: RANDOM\n");
		printf("Especify two files if two threads test (same size).\n");
		
		return 0;
	}

	//Define cache size and array size
	int CACHE_SIZE = 1024*1024*25;
	int ARRAY_SIZE = CACHE_SIZE*12;


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
			break;
	}

		//Prepare arrays
	array1=malloc(size);
	array2=malloc(size);

	// Open file for both reading and writing
   	fp = fopen(filename, "r+");
   	fp2 = fopen(filename2, "r+");
   	//Get filesize
   	fseek(fp, 0, SEEK_END); // seek to end of file
	filesize = ftell(fp); // get current file pointer
	fseek(fp, 0, SEEK_SET); // seek back to beginning of file

	//Number of block of the sized chosen
	int numBlocks = filesize/size;
	int r, tid;
	//Get initial time
	gettimeofday(&start_t,NULL);

	omp_set_num_threads(numThreads);
	#pragma omp parallel shared (i)
	{
		tid = omp_get_thread_num();
		while(condition){
			
			if (iotype==READ){
				// Read data
				if (tid==0){
					//Seek to the index position
					fseek(fp, index, SEEK_SET);
	   				fread(array1, 1, size, fp);
	   			}
	   			else{
	   				//Seek to the index position
					fseek(fp2, index, SEEK_SET);
	   				fread(array2, 1, size, fp2);
	   			}
			}else{
				
				if (tid==0){
					//Seek to the index position
					fseek(fp, index, SEEK_SET);
					// Write data to the file 
		   			fwrite(array1, 1, size, fp);
		   		}else{
		   			//Seek to the index position
					fseek(fp2, index, SEEK_SET);
					// Write data to the file 
		   			fwrite(array2, 1, size, fp2);
		   		}
				
			}
			//Get new position
			if (accessType==RANDOM){
				index = rand() % numBlocks;
				if (i>=NUM_RANDOM)
					condition=0;
			}else{
				index+=size;
				if(index>filesize || i>NUM_RANDOM)
					condition=0;
			}
			//Operation counter
			i++;
		}
	}
	//Close file
	fclose(fp);

	//Get final time and compute results
   	gettimeofday(&final_t,NULL);
	double t1= (double) start_t.tv_sec + start_t.tv_usec/1000000.0;
	double t = ((double) final_t.tv_sec + final_t.tv_usec/1000000.0) - t1;

	//Throughput in MB/s
	double throughput = size*i/t/(1024*1024);

	//Latency in us
	double latency = t*1e6/i;


	//Display the results

	char *iotypes[2] = {"READ", "WRITE"};
	char *type[2] = {"SEQUENTIAL", "RANDOM"};
	char *blocks[3] = {"BYTE", "KB", "MB"};
	printf("Throughput: %.3f MB/s, Latency: %.3f us \t-> IO Operation: %s.    Block size: %s.   Type access: %s.   %d threads\n", throughput, latency, iotypes[iotype], blocks[chunkSize], type[accessType], numThreads);
	free(array2);

	return 0;
}



