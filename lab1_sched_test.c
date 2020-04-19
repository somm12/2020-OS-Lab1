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

/*
 * you need to implement scheduler simlator test code.
 *
 */

int main(void){
	/*****example workload FIFO in textbook****/
	Queue pq;
	QueueInit(&pq);
	process arr[7] = {{0,3,'A'},{3,2,'B' },{4,4,'C'},{8,4,'D'},{10,5,'E'},{5,3,'F'},{12,4,'G'}};
	int size;
	size = sizeof(arr) / sizeof(process);
	fifo_workload_table(size,arr);	

	int total_time = 0;

	for (int i = 0; i<size; i++) {
		total_time += arr[i].arrive_time;
		total_time += arr[i].service_time;
	}
	fifo(arr, &pq, total_time, size);
	rr(arr, &pq, total_time, size);
/****************MLFQ************************/	
	process arr3[7] = {{0,3,'A'},{3,2,'B' },{4,4,'C'},{8,4,'D'},{10,5,'E'},{5,3,'F'},{12,4,'G'}};
	total_time = 0;
	for(int i = 0; i < size; i++){
		total_time += arr3[i].arrive_time;
		total_time += arr3[i].service_time;
	}
	mlfq(arr3, total_time, size);

	process arr4[3] = {{0,5,'A',100},{0,6,'B',200},{0,5,'C',40}};
	total_time = 0;
	size = sizeof(arr4)/sizeof(process);
	  for(int i = 0; i < size; i++){
          total_time += arr4[i].arrive_time;
          total_time += arr4[i].service_time;
      }
	stride(arr4,&pq,total_time,size);


	return 0;
}
