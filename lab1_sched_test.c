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
	process arr[5] = {{2,6},{4,4},{0,3},{6,5},{8,2}};
	int process_num = sizeof(arr)/sizeof(process);
	int total_time = 0;

	for (int i=0 ; i<process_num ; i++){
		total_time += arr[i].arrive_time;
		total_time += arr[i].service_time;
	}

	fifo(arr, &pq, total_time, process_num);
//	rr(arr, &pq, total_time, process_num);

	return 0;
}
