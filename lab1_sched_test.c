/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32182490 
*		Student name : An Somin
*		Student id : 32182110
*		Student name : Seo Museong
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

/*
 * you need to implement scheduler simlator test code.
 *
 */

int main(int argc, char *argv[]){


	Queue pq; 
	QueueInit(&pq);
//	process arr1[5] = {{0,3,'A'},{2,6,'B'},{4,4,'C'},{6,5,'D'},{8,2,'E'}};
//	process arr1[7] = {{0,4,'A'},{3,2,'B'},{4,4,'C'},{8,4,'D'},{10,5,'E'},{5,3,'F'},{12,4,'G'}};
	process arr1[3] = {{0,7,'A'},{3,5,'B'},{6,8,'C'}};

	int size;
	int process_num1 = sizeof(arr1)/sizeof(process);
	int total_time1 = 0;
	
	for (int i=0 ; i < process_num1 ; i++){
		total_time1 += arr1[i].arrive_time;
		total_time1 += arr1[i].service_time;
	}

	printf("[ FIFO ]\n");
	fifo(arr1, &pq, total_time1, process_num1);
	printf("[  RR  ]\n");
	rr(arr1, &pq, total_time1, process_num1);
	printf("[ MLFQ ]\n");
	mlfq(arr1, total_time1, process_num1);

/*
	process arr2[7] = {{0,3,'A'},{3,2,'B' },{4,4,'C'},{8,4,'D'},{10,5,'E'},{5,3,'F'},{12,4,'G'}};

    int size;
    int process_num2 = sizeof(arr2)/sizeof(process);
    int total_time2 = 0;
    
    for (int i=0 ; i < process_num2 ; i++){
        total_time2 += arr2[i].arrive_time;
        total_time2 += arr2[i].service_time;
    }

    fifo_workload_table(process_num2, arr2);
    printf("============================ FIFO =============================\n");
    fifo(arr2, &pq, total_time2, process_num2);
    printf("============================  RR  =============================\n");
    rr(arr2, &pq, total_time2, process_num2);
    printf("============================ MLFQ =============================\n");
    mlfq(arr2, total_time2, process_num2);
*/


/*
	process arr4[3] = {{0,5,'A',100},{0,5,'B',200},{0,5,'C',40}};
	total_time = 0;
	size = sizeof(arr4)/sizeof(process);
	for(int i = 0; i < size; i++){
            total_time += arr4[i].arrive_time;
            total_time += arr4[i].service_time;
      	}
	stride(arr4,&pq,total_time,size);
*/
	
	return 0;
}
