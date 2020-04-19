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

/************************************************ FIFO Graph Implementation *********************************************/

void fifo_graph(process arr[], int size, int time) {
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

/************************************************ Graph Implementation *********************************************/

void graph(process arr[], int size, int time) {
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
	fifo_graph(sort, size, total_service_time);
}

/************************************************ RR Implementation  **********************************************/

void rr(process arr[], Queue* pq, int time, int size) {
	Queue output;						// 실행하고 있는 프로세스를 저장해두는 큐
	QueueInit(&output);
	int total_service_time = 0;			// 전체 실행 시간
	int k = 0;							// 전체 루프에서 단 한 번만 함수를 실행시키기 위한 변수
	int signal = 0;						// 프로세스가 실행 중이지 않음을 알려주는 변수
	process sort[time];					// 출력을 위해 Queue 에 있는  결과값을 저장해두는 배열
	process running[1] = { { -2,-2 } }; // 실행 중인 프로세스를 보관하기 위한 구조체 배열
	process init[1] = { { -1,-1 } };	// 첫 프로세스가 실행하기 전까지 큐가 비어있지 않게 해주기 위한 구조체 배열
	Enqueue(pq, init[0]);
	
	for (int i = 0; i < size; i++) {	// 전체 실행 시간 total_service_time 계산
		total_service_time += arr[i].service_time;
	}

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
			if (QPeek(pq).arrive_time == i) {			// Queue 의 Front 에 위치한 프로세스의 arrive_time 이 현재 시간과 같다면
				signal = -1;							// running 의 signal 을 -1로 초기화 -> 다음 if문 수행 가능
				k = 1;
			}
		}
		if (signal == -1) {									   // 프로세스의 실행이 막 끝났을 때 또는 실행 중인 프로세스가 없을 때
			if (running[0].service_time != 0 && running[0].service_time != -2) {	// 이전 루프에서 service_time이 0이 안되었다면

				Enqueue(pq, running[0]);											// running[0]에 있는 값을 pq Queue 에 넣어준다
			}
			running[0] = QPeek(pq);			// Queue Front 에 위치한 프로세스를 running 으로
			Dequeue(pq);					// 옮긴 프로세스를 Dequeue
			Enqueue(&output, running[0]);	// running 에 넣어준 프로세스를 output Queue 에 저장한다
		}
		running[0].service_time -= 1;		// running의 service_time 감소
		signal = -1;						// running의 signal 초기화

		if(QIsEmpty(pq) == 1 && running[0].service_time == 0) {
			break;							// 더 이상 Queue에 남은 프로세스가 없으면 종료
		}
	}

	int i = 0;
	while(QIsEmpty(&output) == 0) {			// output Queue 에 있는 프로세스 값들을 출력하기 위해 배열로 바꾸는 과정
		sort[i] = Dequeue(&output);
		i++;
	}
	graph(sort, size, total_service_time);	// graph 함수 호출
}

/**************************************************** MLFQ Implementation ****************************************************************/


void mlfq(process arr[], int time, int size) {
	Queue P1;
	Queue P2;
	Queue P3;							// 우선순위를 정해주기 위한 큐 4개
	Queue P4;							// P1이 우선순위가 제일 높으며, P4가 우선순위가 제일 낮다
	QueueInit(&P1);
	QueueInit(&P2);
    QueueInit(&P3);
    QueueInit(&P4);
	Queue output;						// 실행하고 있는 프로세스를 저장해두는 큐
	QueueInit(&output);
	int total_service_time = 0;			// 전체 실행 시간
	int k = 0;							// 전체 루프에서 단 한 번만 함수를 실행시키기 위한 변수
	int signal = 0;						// 프로세스가 실행 중이지 않음을 알려주는 변수
	process sort[time];					// 출력을 위해 Queue 에 있는  결과값을 저장해두는 배열
	process running[1] = { { -2,-2 } }; // 실행 중인 프로세스를 보관하기 위한 구조체 배열
	process init[1] = { { -1,-1 } };	// 첫 프로세스가 실행하기 전까지 큐가 비어있지 않게 해주기 위한 구조체 배열
	Enqueue(pq, init[0]);
	
	for (int i = 0; i < size; i++) {	// 전체 실행 시간 total_service_time 계산
		total_service_time += arr[i].service_time;
	}

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
				signal = -1;					// 다음 if문을 수행하도록 signal 을 -1로 초기화해준다
				k = 1;
			}
		}
		if (signal == -1) {									  // 프로세스의 실행이 막 끝났을 때 또는 실행 중인 프로세스가 없을 때
			if (running[0].service_time != 0 && running[0].service_time != -2) {   // 이전 루프에서 service_time이 0이 안되었다면
				switch(running[0].priority){				   // running[0]의 priority 값에 따라서 다른 Queue에 Enqueue를 해준다
					case 1:
						if (QIsEmpty(&P1) && QIsEmpty(&P2) && QIsEmpty(&P3) && QIsEmpty(&P4) == 1){
							running[0].priority = 1;
							Enqueue(&P1,running[0]); // 프로세스가 단독으로 실행중이여서 Queue가 전부 비어있을 때는 우선순위를 그대로
						}
						else{
							running[0].priority = 2;
							Enqueue(&P2,running[0]); // 다른 프로세스가 존재한다면 우선순위를 한단계 낮춘다
						}
						break;
					case 2:
						if (QIsEmpty(&P1) && QIsEmpty(&P2) && QIsEmpty(&P3) && QIsEmpty(&P4) == 1){
							running[0].priority = 2;
							Enqueue(&P2,running[0]);
						}
						else{
							running[0].priority = 3;
							Enqueue(&P3,running[0]);
						}
						break;
					case 3:	
						if (QIsEmpty(&P1) && QIsEmpty(&P2) && QIsEmpty(&P3) && QIsEmpty(&P4) == 1){
							running[0].priority = 3;
							Enqueue(&P3,running[0]);
						}
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
			}
			Enqueue(&output, running[0]);	// running 에 넣어준 프로세스를 output 으로 Enqueue
		}
		running[0].service_time -= 1;		// running의 service_time 감소
		signal = -1;						// running의 signal 초기화


		if(QIsEmpty(&P1) && QIsEmpty(&P2) && QIsEmpty(&P3) && QIsEmpty(&P4)  == 1 && running[0].service_time == 0) {
			count_service_time++;
			if (count_service_time == size) break; // 더 이상 Queue에 남은 프로세스가 없으면 종료
		}
	}

	int i = 0;
	while(QIsEmpty(&output) == 0) {
		sort[i] = Dequeue(&output);
		i++;
	}

	graph(sort, size, total_service_time);
}

/************************************************ STRIDE Implementation  **********************************************/

void stride(process arr[], Queue* pq,int time, int size) {
	Queue output;
	QueueInit(&output);
	process make[size];//pass value 비교하여 원래 구조체 배열을 sort시, 담기위한 구조체 배열 make.
	int g = 0;// index 3개의 프로세스를 pass value 비교해서 sort하기위한 배열의 index
	int total_service_time = 0;
	int k = 0;
	int decrease_size = size;
	process init[1] = {{0,0}};////make 구조체 배열 인덱스자리에 값을 할당하는데 쓰인 크기가 1 인 배열
	int count_service_time = 0;
	int signal = 0;//다른 프로세스가 수행하기 전 신호변수
	process sort[time];
	process running[1] = { { -2,-2 } }; // 실행 중인 프로세스를 보관한다
	
	for (int i = 0; i < time; i++) {					// total_time 만큼 횟수 반복
		for (int j = 0; j < size; j++) {				// process_num  만큼 횟수 반복
			if (arr[j].arrive_time == i) {				// arrive_time 이 현재 시간이랑 같은 프로세스가 있으면 
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
				make[0] = running[0];
				for(int i = 1; i < size;i++){
					if(QIsEmpty(pq) == 0)
					make[i] = Dequeue(pq);
				}
				printf("%d %d %d\n",make[0].arrive_time,make[1].arrive_time,make[2].arrive_time);
				bubble_sort(make,size);//***************
				printf("%d %d %d\n",make[0].arrive_time,make[1].arrive_time,make[2].arrive_time);
                for (int i=0; i < size; i++) {
					if(make[i].service_time!=0) Enqueue(pq,make[i]);
                    make[i] = init[0];
				}
			}
		}
		//전 프로세스가 서비스 타임이 0이 되었을 때 QPeek을 만나 함수가 끝나버려서 make[]안에 있는 프로세스들을enque
		
		running[0] = QPeek(pq);			// Queue Front 에 위치한 프로세스를 running 으로
		Dequeue(pq);					// 옮긴 프로세스를 Dequeue
		Enqueue(&output, running[0]);
				// running 에 넣어준 프로세스를 output 으로 Enqueue
		
		running[0].service_time -= 1;		// running의 service_time 감소
		signal = -1;		// running의 signal 초기화

		if(QIsEmpty(pq) == 1 && running[0].service_time == 0) {
			count_service_time++;
			if (count_service_time==size) break; // 더 이상 Queue에 남은 프로세스가 없으면 종료
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
	graph(sort, size, total_service_time);
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
				process_name = arr[j].name;


				arr[j].arrive_time = arr[j + 1].arrive_time;
				arr[j].priority = arr[j + 1].priority;
				arr[j].service_time = arr[j + 1].service_time;
				arr[j].name = arr[j+1].name;

				arr[j + 1].arrive_time = passvalue;
				arr[j + 1].priority = stride;
				arr[j + 1].service_time = servicetime;
				arr[j + 1].name = process_name;
       // 다음 요소로 보냄
			}
		}
	}
}
