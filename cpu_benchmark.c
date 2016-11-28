//CPU Benchmark
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>
#define NUM_LOOPS 2e9
#define IOPS 1
#define FLOPS 2

int numThreads, typeTest;

int compute_IOPS(){
   //Number of iterations
   unsigned int i, loops=NUM_LOOPS;

   //Base numbers on operations
   int mul=2;
   int add1=1, add2=-3;
   //Different values to avoid compiler optimization and compensate the sumation or multiplication
   int sum1=1, sum2=2, sum3=3, sum4=-4, sum5=-2;
   int mul1=-1, mul2=1, mul3=2, mul4= 3, mul5=4;

   omp_set_num_threads(numThreads); /* requests numThreads threads */
   #pragma omp parallel for

   for (i=0; i<loops; i++) {
         //Each block are 10 operations + 10 assignments
         //Each iteration are 80 operations + 1 comparison + incremente of i= 82
         mul1*=mul; mul2*=mul; mul3*=mul; mul4*=mul; mul5*=mul;
         sum1+=add1; sum2+=add1; sum3+=add1; sum4+=add1; sum5+=add1;

         mul1*=mul; mul2*=mul; mul3*=mul; mul4*=mul; mul5*=mul;
         sum1+=add1; sum2+=add1; sum3+=add1; sum4+=add1; sum5+=add1;

         mul1*=mul; mul2*=mul; mul3*=mul; mul4*=mul; mul5*=mul;
         sum1+=add1; sum2+=add1; sum3+=add1; sum4+=add1; sum5+=add1;

         mul1*=mul; mul2*=mul; mul3*=mul; mul4*=mul; mul5*=mul;
         sum1+=add2; sum2+=add2; sum3+=add2; sum4+=add2; sum5+=add2;

   }

}

int compute_FLOPS(){
   //Number of iterations
   unsigned int i, loops=NUM_LOOPS;

   //Base numbers on operations
   double mul=1.0000001;
   double add=0.0001;
   //Different values to avoid compiler optimization
   double sum1=0.1, sum2=0.2, sum3=0.3, sum4=-0.4, sum5=-0.2;
   double mul1=1.0, mul2=1.1, mul3=1.2, mul4= 1.3, mul5=1.4;

   omp_set_num_threads(numThreads); /* requests numThreads threads */
   #pragma omp parallel for

   for (i=0; i<loops; i++) {
         //Each block are 10 operations + 10 assignments
         //Each iteration are 80 operations + 1 comparison + incremente of i= 82
         mul1*=mul; mul2*=mul; mul3*=mul; mul4*=mul; mul5*=mul;
         sum1+=add; sum2+=add; sum3+=add; sum4+=add; sum5+=add;

         mul1*=mul; mul2*=mul; mul3*=mul; mul4*=mul; mul5*=mul;
         sum1+=add; sum2+=add; sum3+=add; sum4+=add; sum5+=add;

         mul1*=mul; mul2*=mul; mul3*=mul; mul4*=mul; mul5*=mul;
         sum1+=add; sum2+=add; sum3+=add; sum4+=add; sum5+=add;

         mul1*=mul; mul2*=mul; mul3*=mul; mul4*=mul; mul5*=mul;
         sum1+=add; sum2+=add; sum3+=add; sum4+=add; sum5+=add;

   }
}

int main(int argc, char** argv) {
   
   if(argc == 3){
      typeTest=atoi(argv[1]);
      numThreads=atoi(argv[2]);
   }else{
      printf("Error in the arguments\n");
      printf("Please use this: cpu_benchmark <typeTest> <numThreads>\n");
      printf("Type of test operations: - 1: Integer\t- 2: Floating Point\n");
      return 0;
   }

   char test[6], iops[]="GIOPS ", flops[]="GFLOPS";
   //Create the time structure  
   struct timeval start_t, final_t;
   
   //Get start time and convert to seconds
   gettimeofday(&start_t,NULL);
   double t1= (double) start_t.tv_sec + start_t.tv_usec/1000000.0;

   //RUN the test depending on the option chosen
   if (typeTest == IOPS){
      compute_IOPS();
      strcpy(test, iops);
   }else if (typeTest == FLOPS){
      compute_FLOPS();
      strcpy(test, flops);
   }

   //Get end time and compute execution time
   gettimeofday(&final_t,NULL);
   double t = ((double) final_t.tv_sec + final_t.tv_usec/1000000.0) - t1;

   //Compute GFLOPS and print results
   printf("Time:\t %.3f s, %.3f %s\t -> %d threads\n", test, t, (double)NUM_LOOPS*82/t/1e9, numThreads);
   return 0;
}