/*	DKU 2020 Operating System Lab
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
		printf("Queue Memory Error!");;
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
		 printf("location is QPeek");

		exit(-1);
	}

	return pq->front->data;
}

void fifo_workload_table(int size, process arr[]){

	printf("\n");
	printf("==========Workload==========\n");
	printf("arrive time | service time\n");
	printf("----------------------------\n");
	for (int i = 0; i < size; i++) {
		printf("%c",65+i);
		if(arr[i].arrive_time >= 10)
			printf("         %d |  %d          \n",arr[i].arrive_time,arr[i].service_time);
		else
		{printf("         %d  |  %d          \n",arr[i].arrive_time,arr[i].service_time);}
	}
	printf("\n");}

void stride_workload_table(int size, process arr[]){

	printf("\n");
	printf("===============Workload============\n");
	printf("  passvalue | service time | stride\n");
	printf("-----------------------------------\n");
	for (int i = 0; i < size; i++) {
		printf("%c",65+i);
		printf("	 %d  |       %d      |  %d        \n",arr[i].arrive_time,arr[i].service_time,arr[i].priority);
	}
	printf("\n");

}
void fifo(process arr[], Queue* pq, int total_time, int size) {//main함수에서 할당한 각 프로세스의 워크로드를 구조체 배열로, 프로세스 갯수, 총 시간, 큐를 인자로 둠.
	Queue output;// 실행 결과 출력 내용을 담을 Queue
	QueueInit(&output);// 초기화
	int k = 0;
	process running[1] = { { -2,-2,' ' } };//구조체 배열 선언.arrivtime,servitime,프로세스 이름을 멤버로 가짐.
	process init[1] = { { -1,-1,' ' } };// 첫 프로세스의 도착시간이 0이 아닐 때 140번째 줄에서 Qpeek를 만나 프로그램이 종료되지 않기 위해 미리 -1로 큐에 값을 할당.
	Enqueue(pq, init[0]);
	for (int i = 0; i < total_time; i++) {		// i = 전체 프로세스가 수행하는 데 걸리는 시간(시간 변수)

		for (int j = 0; j < size; j++) {	// j = 프로세스 갯수
				if (arr[j].arrive_time == i) {		// 프로세스가 도착하는 시간과 흘러가는 시간이 같을 때(반복문으로 인해 가장 빨리 도착한 프로세스가 Enqueue됨)
				if (k == 0) {
					if (QPeek(pq).arrive_time == -1) {//처음 시작하는 프로세스가 enqueue될 때 위에서 미리 할당한 초기값 {-1,-1} 디큐하기위함

						Dequeue(pq);
					}
				}
				Enqueue(pq, arr[j]);//인자로 받아온 구조체 배열을  해당 인덱스에 맞춰 프로세스를 큐에 할당.
			
				// enqueue the process
				
			}
		}
		if(k==0){// 수행하고 있지 않은 상태여부를 보는 변수 running[0].arrive_time 값을 할당하기 위함.
			// 첫 프로세스가 시작하고나서 부터는 필요하지 않은 조건문.
			if (QPeek(pq).arrive_time == i) {
			running[0].arrive_time = -1;//프로세스가 수행하는 상태가 아닐 때 -1 값을 대입하고. 첫 프로세스 시작전 값을 할당함.
			k = 1;
		}
	}
	
		if (running[0].arrive_time == -1) {// 어떤  프로세스도 수행하고 있는 상태가 아닐 때 수행 시작을 위해 QPeek를하고 첫번째 프로세스를 할당.
			
			running[0] = QPeek(pq);	
			Dequeue(pq);// 그 다음 프로세스 수행을 위해 맨 앞에 있는 먼저 수행하는 프로세스를 빼줌
			Enqueue(&output, running[0]);      //수행한 프로세스를 출력하기 위해 output 큐에 인큐.
			
		}
			running[0].service_time -= 1;		// 프로세스가 1초 수행 했으므로 service time 1초 감소
			if (running[0].service_time == 0) {	// 프로세스가 수행을 끝냈을 때
				running[0].arrive_time = -1;	// 다시 프로세스 수행이 끝났으므로 -1 값을 대임하여 수행상태가 아님을 명시.
			}
		
		if(QIsEmpty(pq) == 1 && running[0].service_time == 0)break;// 만약 모든 프로세스의 수행이 끝났을 경우, 큐가 비었을 때와 마지막 프로세스의 수행시간이 0 이됬을 때 break
	}
	int i = 0;

	while(QIsEmpty(&output)==0) {// output큐에 담아 놓았던 모든 프로세스의 수행 기록을 구조체 배열로 저장
		arr[i] = Dequeue(&output);
		i++;	
	}
	
	int total_service_time = 0;
	for(int i = 0; i < size; i++) {// 총 수행 시간을 인자로 값을 계산하여 출력함수 인자로 쓰임
		total_service_time += arr[i].service_time;

	}
	
	fifograph(arr, size, total_service_time);//그래프 출력 함수를 호출
	////call funtion to draw the FIFO graph

}
	

void fifograph(process arr[], int size,int total_service_time) {// FIFO scheduling방식을 출력해주는 함수.
	 int sum = 0;// 출력시 공백문자를 출력하기 위해 사용되는 변수
   char temp[100];// 프로세스 이름을 담음
   for(int i = 0; i < size; i++){
		temp[i] = arr[i].process_name;
   }
   for (int j = -1; j <= total_service_time; j++){ // 총 수행시간만큼 시간축을 차례로 출력하기 위한 for반복문.
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
   for (int i = 0; i < size; i++) {//그래프 모양을 위해서'|' 문자와 블록 모양 문자를 이용하여 프로세스 수행결과 출력 
      if (i < 1)
      {
         printf("%c", temp[i]);
         printf(" | ");

      }
      for (int k = 0; k < arr[i].service_time; k++) {
         printf("■■ ");//

      }
      printf("\n");
      if (i < size -1)
      {
         printf("%c", temp[i + 1]);// B|    C|   D|  모양을 출력
         printf(" | ");
      }
      sum = sum + arr[i].service_time;// 공백문자 수을 계산
      for (int j = 0; j < sum; j++) {// sum만큼 출력
         printf("   ");

      }
   }
   printf("\n");	

}
/************************************************ RR Graph Implementation *********************************************/

void graph(process arr[], int size, int time) {
   
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
void rr_4(process arr[], Queue* pq, int time, int size) {
	Queue output;
	QueueInit(&output);
	int total_service_time = 0;
	int k = 0;
	int time_slice = 0;
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

				if(time_slice==4)
					Enqueue(pq, running[0]);		// 
			}
			// Queue Front 에 위치한 프로세스를 running 으로
			if (running[0].service_time == 0 || time_slice == 4)
			{
				Dequeue(pq);
				time_slice = 0;
			}
			if (time_slice == 0&&QIsEmpty(pq)==0)running[0] = QPeek(pq);// 첫번째 시작하는 프로세스가 time slice을 다할 때까지 다음 값을 받아오지 않고 마지막 프로세스가 수행을 마치면 더이상 값을 불러오지 않는다.
			Enqueue(&output, running[0]);		// running 에 넣어준 프로세스를 output 으로 Enqueue
		}
		running[0].service_time -= 1;
		time_slice++;		// running의 service_time 감소
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
	graph(sort, size, total_service_time);
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
	graph(sort, size, total_service_time);
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
	graph(sort, size, total_service_time);//  rr방식을 출력하는 함수를 공통으로 사용
}

/************************************************ STRIDE Implementation  **********************************************/
// fifo 함수를 기반으로 구현한 stride 함수.
void stride(process arr[], Queue* pq,int time, int size) {//각 프로세스의 워크로드를 담은 구조체 배열, 프로세스 수행에 사용되는 큐, 전체 수행시간, 프로세스 갯수를 인자로 받음.
	Queue output;	   //프로세스 수행 결과를 출력하기 위해 사용할 output 큐
	QueueInit(&output);
	process make[size];//pass value 비교하여 원래 구조체 배열을 sort시,sort 한 결과를 담기위한 구조체 배열 make.
	int total_service_time = 0;//전체 수행시간을 계산하여 담을 변수.
	int k = 0;
	process init[1] = {{0,0}};////make 구조체 배열 인덱스 자리에 값을 할당하는데 쓰기 위함. 크기가 1 인 구조체 배열
	int count_service_time = 0; // 모든 프로세스 수행이 다 되었는지 확인하기 위한 변수.
	int signal = 0;//다른 프로세스가 수행하기 전 신호변수
	process sort[time];// 프로세스들이 수행한 순서를 차례대로 담은 sort 구조체 배열. 이 배열을 이용하여 출력함수 인자로 전달.
	process running[1] = { { -2,-2 } }; // 실행 중인 프로세스를 보관하기 위한 구조체 배열 -2로 초기화 해놓음.
	
	for (int i = 0; i < time; i++) {					// total_time 만큼 횟수 반복
		for (int j = 0; j < size; j++) {				// process_num  만큼 횟수 반복
			if (arr[j].arrive_time == i) {				// arrive_time 이 현재 시간이랑 같은 프로세스가 있으면 
				Enqueue(pq, arr[j]);					// 프로세스를 pq 에 Enqueue -> 모든 프로세스의 pass value가 0으로 시작=> 모두 한번에 Enqueue
			}
		}
		if (k == 0) { // (일회성) 프로세스 수행 첫 시작 신호
			if (QPeek(pq).arrive_time == i) {	// Queue 의 Front 에 위치한 프로세스의 arrive_time 이 현재 시간과 같다면
				signal = -1;	// running 의 signal 을 -1로 초기화 -> 다음 if문 수행 가능
				k = 1;
			}
		}
		if (signal == -1) {		// 프로세스의 실행이 막 끝났을 때 또는 실행 중인 프로세스가 없을 때
			if (running[0].service_time != 0 && running[0].service_time != -2) {//이전 루프에서 service_time이 0이 안되었다면 또는 아직 수행하지 않아서 -2 값이 아니라면
				
				running[0].arrive_time += running[0].priority;//arrivtime은 passvalue를 나타냄.priority는 stride값을 나타냄.**** 프로세스 수행이 끝나고 pass value update.
				make[0] = running[0];//update된 pass value값을 가진 프로세스를 make 구조체 배열에 대입
				for(int i = 1; i < size;i++){//새로 업데이트 된 pass value 값을 바로바로 비교하기 위해 큐를 다 비우고 구조체 배열에 프로세스들을 담는다.
					if(QIsEmpty(pq) == 0)
					make[i] = Dequeue(pq);
				}

				 bubble_sort(make,size);//각 프로세스들의 pass value를 비교하여 작은 순서대로 배열을 정렬해주는 함수 호출.
                     for (int i=0; i < size; i++) {//정렬된 make구조체 배열을 순서대로 인큐한다.
                         if(make[i].service_time!=0)// 전에 할당된 0을 제외하고 큐에 넣어야 하므로 조건문 생성.
                        {Enqueue(pq,make[i]);}
                             make[i] = init[0];// 처음에 make배열에 프로세스들이 할당되고 난 이후 값이 남아있기 때문에 그후 0으로 할당 
                     }
				
			}
		}
		
			running[0] = QPeek(pq);			// Queue Front 에 위치한(먼저 수행할) 프로세스를 running 으로 할당
			Dequeue(pq);					// 옮긴 프로세스를 Dequeue
			Enqueue(&output, running[0]);// 결과 출력을 위해 수행한 프로세스를 output큐에 Enqueue.
		
		running[0].service_time -= 1;		// running에 들어가 있는 방금 수행한 프로세스의 service_time 감소
		signal = -1;		//  signal 초기화

		if(QIsEmpty(pq) == 1 && running[0].service_time == 0) {//모든 프로세스의 수행이 끝나고 큐가 비었을 때 반복문을 멈춤.
			count_service_time++;
			if(count_service_time==size)
				break;
		}
	
		}
	int i = 0;
	while(QIsEmpty(&output) == 0) {
		sort[i] = Dequeue(&output);// 모든 프로세스의 수행 결과를 sort 구조체 배열에 할당
		i++;
	}
	for (int i = 0; i < size; i++) {
		total_service_time += arr[i].service_time;
	}
	graph(sort, size, total_service_time);// 전체 시간을 위에서 계산 후 stride 스케줄링 결과를 출력할 함수 호출.
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
