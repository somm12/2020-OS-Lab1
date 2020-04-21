/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32182490 
*	    Student name : Ansomin
*		
*		Student id: 32182110
*       Student name: Seo Museong
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm test code.
*
*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "include/lab1_sched_types.h"



int main(void){

	Queue pq; 
	QueueInit(&pq);
	process arr1[5] = {{0,3,'A'},{2,6,'B'},{4,4,'C'},{6,5,'D'},{8,2,'E'}};
//	process arr1[7] = {{0,4,'A'},{3,2,'B'},{4,3,'C'},{8,2,'D'},{10,4,'E'},{5,3,'F'},{12,1,'G'}};
//	process arr1[3] = {{0,7,'A'},{3,5,'B'},{6,8,'C'}};

	int size;
	int process_num1 = sizeof(arr1)/sizeof(process);
	int total_time1 = 0;
	
	for (int i=0 ; i < process_num1 ; i++){
		total_time1 += arr1[i].arrive_time;
		total_time1 += arr1[i].service_time;
	}
	fifo_workload_table(process_num1, arr1);
    printf("============================ FIFO =============================\n");
    fifo(arr1, &pq, total_time1, process_num1);
	printf("\n");
    printf("==========================  RR(q=4)  ==========================\n");
    rr_4(arr1, &pq, total_time1, process_num1);
	printf("\n");
    printf("============================ MLFQ =============================\n");
    mlfq(arr1, total_time1, process_num1);
	printf("\n");




	process arr4[5] = {{0,5,'A',100},{0,3,'B',200},{0,6,'C',40},{0,2,'D',50},{0,4,'E',80}};
	total_time1 = 0;
	size = sizeof(arr4)/sizeof(process);
	for(int i = 0; i < size; i++){
            total_time1 += arr4[i].arrive_time;
            total_time1 += arr4[i].service_time;
      	}
	 stride_workload_table(size, arr4);
	 printf("============================ STRIDE ===========================\n");
	stride(arr4,&pq,total_time1,size);
	 printf("\n");
							//stride함수 호출
	return 0;
}
