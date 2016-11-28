//CPU 10 min benchmark
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>
#define NUM_LOOPS 1e9
#define MAX_SECONDS 600
#define IOPS 1
#define FLOPS 2

int result_index=0;
double *results;
int numThreads=5, typeTest;
struct timeval start_t, inter_t;
volatile double t1, t, throughput;
unsigned volatile int i=0, condition=1;

int compute_IOPS(){
   //Number of iterations
   int loops=NUM_LOOPS;

   //Base numbers on operations
   int mul=2;
   int add1=1, add2=-3;
   //Different values to avoid compiler optimization and compensate the sumation or multiplication
   int sum1=1, sum2=2, sum3=3, sum4=-4, sum5=-2;
   int mul1=-1, mul2=1, mul3=2, mul4= 3, mul5=4;


      while(condition) {

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

         i++;
      
      }
}

int compute_FLOPS(){
   //Number of iterations
   int loops=NUM_LOOPS;

   //Base numbers on operations
   double mul=1.0000001;
   double add=0.0001;
   //Different values to avoid compiler optimization
   double sum1=0.1, sum2=0.2, sum3=0.3, sum4=-0.4, sum5=-0.2;
   double mul1=1.0, mul2=1.1, mul3=1.2, mul4= 1.3, mul5=1.4;

   while(condition){
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

         i++;
      }
   }


int main(int argc, char** argv) {
   
   if(argc == 2){
      typeTest=atoi(argv[1]);
   }else{
      printf("Error in the arguments\n");
      printf("Please use this: cpu_benchmark <typeTest>\n");
      printf("Type of test operations: - 1: Integer\t- 2: Floating Point\n");
      return 0;
   }
   results = malloc(600*sizeof(double));
   printf("Test starting...\n");   
   

   int tid;
   #pragma omp parallel shared(i, condition) num_threads(5)
   {
      if(tid = omp_get_thread_num()<4){
         if (typeTest == IOPS){
            compute_IOPS();
         }else if (typeTest == FLOPS){
            compute_FLOPS();
         }
      }else{
         while(condition){
            //Monitoring thread
            sleep(1);
            
            //Compute throughput and 
            throughput = (double)(i-t1)*82/(1*1e9);
            t1=i;
           
            //Store the result in the array
            results[result_index]=throughput;
            result_index++;
            
            //If 10min->end
            if(result_index>MAX_SECONDS)
               condition=0;
         }
      }
   }

   //Write the output file Excel compatible
   FILE *fp;
   char filename[12];

   sprintf(filename, "testcpu%d.csv", typeTest);
   fp=fopen(filename, "w");
   fprintf(fp, "index,GOPS\n");
   int p;
   for(p=0; p<result_index; p++){
      fprintf(fp, "%d,%.2f\n", p, results[p]);
   }
   fclose(fp);


   printf("Test finalized\n");
   return 0;
}