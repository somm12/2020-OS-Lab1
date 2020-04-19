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
	process arr[5] = {{0,3,'A'},{2,6,'B' },{4,4,'C'},{6,5,'D'},{8,2,'E'}};

	int size;
	int process_num = sizeof(arr)/sizeof(process);
	int total_time = 0;
	
	for (int i=0 ; i<process_num ; i++){
		total_time += arr[i].arrive_time;
		total_time += arr[i].service_time;
	}

	fifo(arr, &pq, total_time, process_num);
	rr(arr, &pq, total_time, process_num);
	mlfq(arr, total_time, process_num);

	process arr4[3] = {{0,5,'A',100},{0,5,'B',200},{0,5,'C',40}};
	total_time = 0;
	size = sizeof(arr4)/sizeof(process);
	for(int i = 0; i < size; i++){
            total_time += arr4[i].arrive_time;
            total_time += arr4[i].service_time;
      	}
	stride(arr4,&pq,total_time,size);

	
	return 0;
}
