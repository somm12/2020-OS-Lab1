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
	process arr[5] = {{2,6,'A'},{4,4,'B'},{0,3,'C'},{6,5,'D'},{8,2,'E'}};
	int process_num = sizeof(arr)/sizeof(process);
	int total_time = 0;
	process test[20] = {{2,1,'B'}, {4,1,'A'}, {6,1,'C'}, {6,1,'A'}, {6,1,'D'}, {3,1,'E'}, {7,1,'B'}};

	for (int i=0 ; i<process_num ; i++){
		total_time += arr[i].arrive_time;
		total_time += arr[i].service_time;
	}

	fifo(arr, &pq, total_time, process_num);
	rr(arr, &pq, total_time, process_num);
//	rrgraph(test, 5, 10);
	return 0;
}
