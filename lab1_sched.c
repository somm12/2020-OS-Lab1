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

#include "lab1_sched_types.h"

/******************************** Queue Implementation  *************************************/

void QueueInit(Queue * pq) 
{
   pq->front = NULL;
   pq->rear = NULL;
}

int QIsEmpty(Queue * pq)
{
   if(pq->front == NULL)
      return TRUE;
   else
      return FALSE;
}

void Enqueue(Queue * pq, Data data)
{
   Node * newNode = (Node*)malloc(sizeof(Node));
   newNode->next = NULL;
   newNode->data = data;

   if(QIsEmpty(pq))
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

   if(QIsEmpty(pq))
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
   return pq->front->data;
}
/************************************************ Graph Implementation *********************************************/

void graph(process arr[], int size, int time) {
   int sum = 0;

   char temp[5] = { 'A','B','C','D','E' };
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
   for (int i = 0; i < size; i++) {
      if (i < 1)
      {
         printf("%c", temp[i]);
         printf("|");

      }
      for (int k = 0; k < arr[i].service_time; k++) {
         printf("■■ ");

      }
      printf("\n");
      if (i < 4)
      {
         printf("%c", temp[i+1]);
         printf("|");
      }
      sum = sum + arr[i].service_time;
      for (int j = 0; j < sum; j++) {
         printf("   ");

      }
   }
   printf("\n");

   return;
}


/************************************************ RR Graph Implementation *********************************************/

void rrgraph(process arr[], int size, int time) {
   int sum = 0;


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
            if (arr[j].name == 65 + i){ 
                printf("■■ ");
            }
            else printf("   ");
        }
        printf("\n");
    }
    return;
}





/************************************************ FIFO Implementation  **********************************************/

void fifo(process arr[], Queue* pq, int time, int size) {
	Queue output;
	QueueInit(&output);
	int total_service_time = 0;
	int k = 0;
	process sort[time];
	process running[1] = { { -2,-2 } }; // 실행 중인 프로세스를 보관한다
	process init[1] = { { -1,-1 } };	// 첫 프로세스가 실행하기 전까지 큐가 비어있지 않게 해주는 역할
	Enqueue(pq, init[0]);
	for (int i = 0; i < time; i++) {					// total_time 만큼 횟수 반복 
		for (int j = 0; j < size; j++) {				// process_num  만큼 횟수 반복
			if (arr[j].arrive_time == i) {				// arrive_time 이 현재 시간이랑 같은 프로세스가 있으면
				if (k == 0) {
					if (QPeek(pq).arrive_time == -1){
						Dequeue(pq);					// Init 에서 Queue 에 넣어둔 초기 값을 제거 (일회성)
					}
				}
				Enqueue(pq, arr[j]);					// 프로세스를 pq 에 Enqueue
			}
		}
		if(k==0){ // (일회성)
			if (QPeek(pq).arrive_time == i) {	// Queue 의 Front 에 위치한 프로세스의 arrive_time 이 현재 시간과 같다면
				running[0].arrive_time = -1;	// running 의 arrive_time 을 -1로 초기화 -> 다음 if문 수행 가능
				k = 1;
			}	
		}
		if (running[0].arrive_time == -1) {		// 프로세스의 실행이 막 끝났을 때 또는 실행 중인 프로세스가 없을 때
			running[0] = QPeek(pq);				// Queue Front 에 위치한 프로세스를 running 으로
			Dequeue(pq);						// 옮긴 프로세스를 Dequeue
			Enqueue(&output, running[0]);		// running 에 넣어준 프로세스를 output 으로 Enqueue
		}
			running[0].service_time -= 1;		// running의 service_time 감소
			if (running[0].service_time == 0) {	// service_time 이 0이 되면
				running[0].arrive_time = -1;	// running의 arrive_time 초기화
			}
		if (QIsEmpty(pq) == 1 && running[0].service_time == 0){
			break; // 더 이상 Queue에 남은 프로세스가 없으면 종료
		}
	}
	
	int i = 0;
	while (QIsEmpty(&output)==0) {
		sort[i] = Dequeue(&output);
		i++;	
	}
	for (int i = 0; i < size; i++) {
		printf("%d %d\n", sort[i].arrive_time, sort[i].service_time);
		total_service_time += sort[i].service_time;
	}
	graph(sort, size, total_service_time);
}

/************************************************ RR Implementation  **********************************************/

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
			if (QPeek(pq).arrive_time == i) {	// Queue 의 Front 에 위치한 프로세스의 arrive_time 이 현재 시간과 같다면
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


void mlfq(process arr[], Queue* pq, int time, int size) {
	Queue P1;
	Queue P2;
	Queue P3;
	Queue P4;
	Queue output;
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
				Enqueue(&P1, arr[j]);					// 프로세스를 P1 에 Enqueue
				arr[j].priority = 1;
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
				switch(running[0].priority){ // arr[j]의 priority 값에 따라서 다른 Queue에 Enqueue를 해준다
					case 1:
						running[0].priority = 2;
						Enqueue(&P2,running[0]);
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
			for (int l=0 ; l<4; l++){	// 모든 Queue 중에서 우선순위가 제일 높은 Queue의  Front에 위치한 프로세스를 running으로
				if (QIsEmpty(&P1) != 0){
					running[0] = QPeek(&P1);
					Dequeue(&P1);
				}
				else if (QIsEmpty(&P2) != 0){
					running[0] = QPeek(&P2);
					Dequeue(&P2);
				}
				else if (QIsEmpty(&P3) != 0){
					running[0] = QPeek(&P3);
					Dequeue(&P3);
				}
				else if (QIsEmpty(&P4) != 0){
					running[0] = QPeek(&P4);
					Dequeue(&P4);
				}							// running[0] = QPeek(pq);
			}								// Dequeue(pq);
			Enqueue(&output, running[0]);	// running 에 넣어준 프로세스를 output 으로 Enqueue
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
