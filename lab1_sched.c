/*
*	DKU 2020 Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Studend id : 32182490
*	    Student name : An Somin
*	    Student id : 32182110
*	    Student name : Seo Museong
*
*	lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm function definition.
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
#include <stdio.h>

#include "include/lab1_sched_types.h"

/******************************** Queue Implementation  *************************************/
void QueueInit(Queue * pq)
{
	pq->front = NULL;
	pq->rear = NULL;
}

int QIsEmpty(Queue * pq)
{
	
	if (pq->front == NULL)
		return TRUE;
	else
		return FALSE;
}

void Enqueue(Queue * pq, Data data)
{
	Node * newNode = (Node*)malloc(sizeof(Node));
	newNode->next = NULL;
	newNode->data = data;

	if (QIsEmpty(pq))
	{
		pq->front = newNode;
		pq->rear = newNode;
	}
	else
	{
		pq->rear->next = newNode;
		pq->rear = newNode;
	}
}

Data Dequeue(Queue * pq)
{
	Node * delNode;
	Data retData;


	if (QIsEmpty(pq))
	{
		printf("Queue Memory Error!");
		exit(-1);
	}

	delNode = pq->front;
	retData = delNode->data;
	pq->front = pq->front->next;

	free(delNode);
	return retData;
}

Data QPeek(Queue * pq)
{	
	
	if (QIsEmpty(pq))
	{
		printf("Queue Memory Error!");
		exit(-1);
	}

	return pq->front->data;
}

void fifo_workload_table(int size, process arr[]){

	printf("\n");
	printf("=======Workload in FIFO======\n");
	printf("arrive time | service time\n");
	printf("----------------------------\n");
	for (int i = 0; i < size; i++) {
		printf("%c",65+i);
		printf("         %d  |  %d          \n",arr[i].arrive_time,arr[i].service_time);
	}
	printf("\n");}


void fifo(process arr[], Queue* pq, int total_time, int size) {
	Queue output;
	QueueInit(&output);
	int k = 0;
	process running[1] = { { -2,-2,' ' } };/// declaration of structure array
	process init[1] = { { -1,-1,' ' } };// this is for the case the first process's arrive time is not zero. because of Qpeek.
	Enqueue(pq, init[0]);
	for (int i = 0; i < total_time; i++) {		// i = time (x axis) 

		for (int j = 0; j < size; j++) {	// j = number of process
				if (arr[j].arrive_time == i) {		// if there is a process has same arrive time with i
				if (k == 0) {
					if (QPeek(pq).arrive_time == -1) {//처음 시작하는 프로세스가 enqueue될 때 초기값 {-1,-1} 디큐하기위함

						Dequeue(pq);
					}
				}// this is the case for the first procees's arrive time is zero. because of Dequeue.when the queue is empty. error
				Enqueue(pq, arr[j]);
			
				// enqueue the process
				
			}
		}
		if(k==0){// 수행하고 있지 않은 상태여부를 보는 변수 running[0].arrive_time 값을 할당하기 위함.
			// 첫 프로세스가 시작하고나서 부터는 필요하지 않은 조건문.
			if (QPeek(pq).arrive_time == i) {
			running[0].arrive_time = -1;// when the first fast process begin to start, assign -1 value to start work.
			k = 1;
		}
	}
	
		if (running[0].arrive_time == -1) {		// if none of process is running
			running[0] = QPeek(pq);	
		    // put process into running struct
			Dequeue(pq);
			// pull out the process
		
		// if some process is running
		// if it is the first time the process has runned
			Enqueue(&output, running[0]);
			// enqueue the process into queue
		}
			running[0].service_time -= 1;		// decreases the remain service time
			if (running[0].service_time == 0) {	// if service time becomes zero
				running[0].arrive_time = -1;	// stop running and initialize it
			}
		//if some process is running
		

		if(QIsEmpty(pq) == 1 && running[0].service_time == 0)break;

	}
	int i = 0;

	while(QIsEmpty(&output)==0) {
		arr[i] = Dequeue(&output);// make structure array which is sorted for output
		i++;//(suit format:struct array) to graph funtion input)	
	}
	
	int total_service_time = 0;
	for(int i = 0; i < size; i++) {
		total_service_time += arr[i].service_time;

	}
	
	graph(arr, size, total_service_time);////call funtion to draw the FIFO graph


	
	////call funtion to draw the FIFO graph

}
	

void graph(process arr[], int size,int total_service_time) {
	 int sum = 0;

   char temp[100];
   for(int i = 0; i < size; i++){
		temp[i] = arr[i].process_name;
   }
   for (int j = -1; j <= total_service_time; j++){ // 20 = sum of the service time
      if (j >  0){
	     if (j > 9){
		    printf("%d ", j);
		 }
		 else {
	        printf(" %d ", j);
		 }
	  }
	  else{
         printf("  ");
      }
   }

   printf("\n");
   for (int i = 0; i < size; i++) {
      if (i < 1)
      {
         printf("%c", temp[i]);
         printf(" | ");

      }
      for (int k = 0; k < arr[i].service_time; k++) {
         printf("■■ ");

      }
      printf("\n");
      if (i < 4)
      {
         printf("%c", temp[i + 1]);
         printf(" | ");
      }
      sum = sum + arr[i].service_time;
      for (int j = 0; j < sum; j++) {
         printf("   ");

      }
   }
   printf("\n");	

}
/************************************************ RR Graph Implementation *********************************************/

void rrgraph(process arr[], int size, int time) {
   
   /***************************** x축 그리기 *****************************/

   for (int j = 0; j <= time; j++){ // 20 = sum of the service time
      if (j >  0){
         if (j > 10){
            printf("%d ", j);
         }
         else {
            printf(" %d ", j);
         }
      }
      else{
         printf("   ");
      }
   }
   printf("\n");

   /*********************************************************************/

    for (int i = 0; i < size; i++) {
        printf("%c", 65 + i);
        printf("|");
        for (int j = 0; j < time ; j++){
            if (arr[j].process_name == 65 + i){
                printf("■■ ");
            }
            else printf("   ");
        }
        printf("\n");
    }
    return;
}

void rr(process arr[], Queue* pq, int time, int size) {
	Queue output;
	QueueInit(&output);
	int total_service_time = 0;
	int k = 0;
	int signal = 0;//다른 프로세스가 수행하기 전 신호변수
	process sort[time];
	process running[1] = { { -2,-2 } }; // 실행 중인 프로세스를 보관한다
	process init[1] = { { -1,-1 } };	// 첫 프로세스가 실행하기 전까지 큐가 비어있지 않게 해주는 역할
	Enqueue(pq, init[0]);
	for (int i = 0; i < time; i++) {					// total_time 만큼 횟수 반복 
		for (int j = 0; j < size; j++) {				// process_num  만큼 횟수 반복
			if (arr[j].arrive_time == i) {				// arrive_time 이 현재 시간이랑 같은 프로세스가 있으면
				if (k == 0) {
					if (QPeek(pq).arrive_time == -1) {
						Dequeue(pq);					// Init 에서 Queue 에 넣어둔 초기 값을 제거 (일회성)
					}
				}
				Enqueue(pq, arr[j]);					// 프로세스를 pq 에 Enqueue
			}
		}
		if (k == 0) { // (일회성)
			if (QPeek(pq).arrive_time == i) {	// Queue 의 Front 에 위치한 프로세스의 arrive_time 이 현재 시간과 같다면//******************************************
				signal = -1;	// running 의 signal 을 -1로 초기화 -> 다음 if문 수행 가능
				k = 1;
			}
		}
		if (signal == -1) {		// 프로세스의 실행이 막 끝났을 때 또는 실행 중인 프로세스가 없을 때
			if (running[0].service_time != 0 && running[0].service_time != -2) {//이전 루프에서 service_time이 0이 안되었다면

				Enqueue(pq, running[0]);		// 
			}
			running[0] = QPeek(pq);			// Queue Front 에 위치한 프로세스를 running 으로
			Dequeue(pq);					// 옮긴 프로세스를 Dequeue
			Enqueue(&output, running[0]);		// running 에 넣어준 프로세스를 output 으로 Enqueue
		}
		running[0].service_time -= 1;		// running의 service_time 감소
		signal = -1;		// running의 signal 초기화

		if(QIsEmpty(pq) == 1 && running[0].service_time == 0) {
			break; // 더 이상 Queue에 남은 프로세스가 없으면 종료
		}
	}

	int i = 0;
	while(QIsEmpty(&output) == 0) {
		sort[i] = Dequeue(&output);
		i++;
	}
	for (int i = 0; i < size; i++) {
		total_service_time += arr[i].service_time;
	}
	rrgraph(sort, size, total_service_time);
}

/**************************************************** MLFQ Implementation ****************************************************************/
void mlfq(process arr[], int time, int size) {
	Queue P1;
	Queue P2;
	Queue P3;
	Queue P4;
	Queue output;
	QueueInit(&P1);
	QueueInit(&P2);
    QueueInit(&P3);
    QueueInit(&P4);
	QueueInit(&output);
	int total_service_time = 0;
	int k = 0;
	int signal = 0;						//다른 프로세스가 수행하기 전 신호변수
	process sort[time];
	process running[1] = { { -2,-2 } }; // 실행 중인 프로세스를 보관한다
	process init[1] = { { -1,-1 } };	// 첫 프로세스가 실행하기 전까지 큐가 비어있지 않게 해주는 역할
	Enqueue(&P1, init[0]);
	for (int i = 0; i < time; i++) {					// total_time 만큼 횟수 반복
		for (int j = 0; j < size; j++) {				// process_num  만큼 횟수 반복
			if (arr[j].arrive_time == i) {				// arrive_time 이 현재 시간이랑 같은 프로세스가 있으면
				if (k == 0) {
					if (QPeek(&P1).arrive_time == -1) {
						Dequeue(&P1);					// Init 에서 Queue 에 넣어둔 초기 값을 제거 (일회성)
					}
				}
				arr[j].priority = 1;
				Enqueue(&P1, arr[j]);					// 프로세스를 P1 에 Enqueue
			}
		}
		if (k == 0) { // (일회성)
			if (QPeek(&P1).arrive_time == i) {	// Queue 의 Front 에 위치한 프로세스의 arrive_time 이 현재 시간과 같다면
				signal = -1;	// running 의 signal 을 -1로 초기화 -> 다음 if문 수행 가능
				k = 1;
			}
		}
		if (signal == -1) {		// 프로세스의 실행이 막 끝났을 때 또는 실행 중인 프로세스가 없을 때
			if (running[0].service_time != 0 && running[0].service_time != -2) {	//이전 루프에서 service_time이 0이 안되었다면
				switch(running[0].priority){ // running[0]의 priority 값에 따라서 다른 Queue에 Enqueue를 해준다
					case 1:
						if (QIsEmpty(&P1) && QIsEmpty(&P2) && QIsEmpty(&P3) && QIsEmpty(&P4) == 1){
							running[0].priority = 1;
							Enqueue(&P1,running[0]); // 프로세스가 단독으로 실행중이여서 Queue가 전부 비어있을 때는 우선순위를 그대로
						}
						else{
							running[0].priority = 2;
							Enqueue(&P2,running[0]);
						}
						break;
					case 2:
						running[0].priority = 3;
						Enqueue(&P3,running[0]);
						break;
					case 3:
					case 4:
						running[0].priority = 4;
						Enqueue(&P4,running[0]);
						break;
				}
			}
			// 모든 Queue 중에서 우선순위가 제일 높은 Queue의 Front에 위치한 프로세스를 running으로
			if (QIsEmpty(&P1) == 0){
				running[0] = QPeek(&P1);
				Dequeue(&P1);
			}
			else if (QIsEmpty(&P2) == 0){
				running[0] = QPeek(&P2);
				Dequeue(&P2);
			}
			else if (QIsEmpty(&P3) == 0){
				running[0] = QPeek(&P3);
				Dequeue(&P3);
			}
			else if (QIsEmpty(&P4) == 0){
				running[0] = QPeek(&P4);
				Dequeue(&P4);
				//rr(&P4, pq, time, size);
			}							// running[0] = QPeek(pq);
										// Dequeue(pq);
			Enqueue(&output, running[0]);	// running 에 넣어준 프로세스를 output 으로 Enqueue
		}
		running[0].service_time -= 1;		// running의 service_time 감소
		signal = -1;		// running의 signal 초기화

		if(QIsEmpty(&P1) && QIsEmpty(&P2) && QIsEmpty(&P3) && QIsEmpty(&P4)  == 1 && running[0].service_time == 0) {
			break; //i 더 이상 Queue에 남은 프로세스가 없으면 종료 (혹시 중간에 전부 비어버려서 멈출 수도 있으니 수정 필요할지도?)
		}
	}

	int i = 0;
	while(QIsEmpty(&output) == 0) {
		sort[i] = Dequeue(&output);
		i++;
	}
	for (int i = 0; i < size; i++) {
		total_service_time += arr[i].service_time;
	}
	rrgraph(sort, size, total_service_time);//  rr방식을 출력하는 함수를 공통으로 사용
}

/************************************************ STRIDE Implementation  **********************************************/

void stride(process arr[], Queue* pq,int time, int size) {
	Queue output;
	QueueInit(&output);
	process make[size];//pass value 비교하여 원래 구조체 배열을 sort시, 담기위한 구조체 배열 make.
	int g = 0;// index 3개의 프로세스를 pass value 비교해서 sort하기위한 배열의 index
	int total_service_time = 0;
	int k = 0;
	int signal = 0;//다른 프로세스가 수행하기 전 신호변수
	process sort[time];
	process running[1] = { { -2,-2 } }; // 실행 중인 프로세스를 보관한다
	process init[1] = { { -1,-1 } };	// 첫 프로세스가 실행하기 전까지 큐가 비어있지 않게 해주는 역할
	Enqueue(pq, init[0]);
	for (int i = 0; i < time; i++) {					// total_time 만큼 횟수 반복
		for (int j = 0; j < size; j++) {				// process_num  만큼 횟수 반복
			if (arr[j].arrive_time == i) {				// arrive_time 이 현재 시간이랑 같은 프로세스가 있으면
				if (k == 0) {
					if (QPeek(pq).arrive_time == -1) {
						Dequeue(pq);					// Init 에서 Queue 에 넣어둔 초기 값을 제거 (일회성)
					}
				}
				Enqueue(pq, arr[j]);					// 프로세스를 pq 에 Enqueue
			}
		}
		if (k == 0) { // (일회성)
			if (QPeek(pq).arrive_time == i) {	// Queue 의 Front 에 위치한 프로세스의 arrive_time 이 현재 시간과 같다면
				signal = -1;	// running 의 signal 을 -1로 초기화 -> 다음 if문 수행 가능
				k = 1;
			}
		}
		if (signal == -1) {		// 프로세스의 실행이 막 끝났을 때 또는 실행 중인 프로세스가 없을 때
			if (running[0].service_time != 0 && running[0].service_time != -2) {//이전 루프에서 service_time이 0이 안되었다
				
				running[0].arrive_time += running[0].priority;//arrivtime은 passvalue를 나타냄.priority는 stride값을 나타냄.
				make[g] = running[0];
				//Enqueue(pq, running[0]);
				g++;
				if (g == size ) { 
					g = 0;
					bubble_sort(make,size);//***************
					for (int i; i < size; i++) {
						Enqueue(pq,make[i]);
					}
				}
				//
			}
			running[0] = QPeek(pq);			// Queue Front 에 위치한 프로세스를 running 으로
			Dequeue(pq);					// 옮긴 프로세스를 Dequeue
			Enqueue(&output, running[0]);		// running 에 넣어준 프로세스를 output 으로 Enqueue
		}
		running[0].service_time -= 1;		// running의 service_time 감소
		signal = -1;		// running의 signal 초기화

		if (QIsEmpty(pq) == 1 && running[0].service_time == 0) {
			break; // 더 이상 Queue에 남은 프로세스가 없으면 종료
		}
	}

	int i = 0;
	while (QIsEmpty(&output) == 0) {
		sort[i] = Dequeue(&output);
		i++;
	}
	for (int i = 0; i < size; i++) {
		total_service_time += arr[i].service_time;
	}
	rrgraph(sort, size, total_service_time);
}

void bubble_sort(process arr[], int size)    // 매개변수로 정렬할 배열과 요소의 개수를 받음
{
	int passvalue;
	int stride;
	int servicetime;
	char process_name;

	for (int i = 0; i < size; i++)    // 요소의 개수만큼 반복
	{
		for (int j = 0; j < size - 1; j++)   // 요소의 개수 - 1만큼 반복
		{
			if (arr[j].arrive_time > arr[j + 1].arrive_time)          // 현재 요소의 값과 다음 요소의 값을 비교하여
			{                                 // 큰 값을
				passvalue = arr[j].arrive_time;
				stride= arr[j].priority;
				servicetime = arr[j].service_time;
				process_name = arr[j].process_name;


				arr[j].arrive_time = arr[j + 1].arrive_time;
				arr[j].priority = arr[j + 1].priority;
				arr[j].service_time = arr[j + 1].service_time;
				arr[j].process_name = arr[j+1].process_name;

				arr[j + 1].arrive_time = passvalue;
				arr[j + 1].priority = stride;
				arr[j + 1].service_time = servicetime;
				arr[j + 1].process_name = process_name;
       // 다음 요소로 보냄
			}
		}
	}
}

