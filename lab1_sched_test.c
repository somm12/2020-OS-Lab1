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

	Queue pq;													//각 fifo, rr, mlfq 방식에서 사용할 Queue 선언
	QueueInit(&pq);												//Queue 초기화

	process arr1[5] = {{0,3,'A'},{2,6,'B'},{4,4,'C'},{6,5,'D'},{8,2,'E'}};// 각 스케줄링 함수에 인자로 대입할 구조체 배열 초기화(수업시간에 한 워크로드예제)
	int size;
	size = sizeof(arr1) / sizeof(process);						//총 프로세스 갯수		
	fifo_workload_table(size, arr1);							//workload table 함수 호출
	int total_time = 0;										

	for (int i = 0; i<size; i++) {
		total_time += arr1[i].arrive_time;						//각 프로세스가 수행하는 전체 시간을 대략계산
		total_time += arr1[i].service_time;
	}
	fifo(arr1, &pq, total_time, size);							//fifo 함수 호출

	rr(arr1, &pq, total_time, size);								//rr 함수 호출
	
	mlfq(arr1, total_time, size);								//mlfq 함수 호출
	
	process arr2[3] = {{0,5,'A',100},{0,6,'B',50},{0,5,'C',250}};// 주어진 티켓 수 100, 50 ,250을 계산해서  pass value, servictime, process name, stride값으로 할당.
	
	size = sizeof(arr2)/sizeof(process);						//stride 스케줄링에 필요한 구조체 배열 크기(프로세스 갯수)
	
	for(int i = 0; i < size; i++){
		arr2[i].priority = 10000/arr2[i].priority;				//각 프로세스의 stride를 계산한 값을 stride 멤버로 대입(priority자리에 stride가 들어감)
	}
	total_time = 0;
	  for(int i = 0; i < size; i++){
          total_time += arr2[i].arrive_time;					//stride에서 수행할 프로세스들의 총 수행시간 계산
		  total_time += arr2[i].service_time;
      }	
				
	stride(arr2,&pq,total_time,size);							//stride함수 호출
	return 0;
}
