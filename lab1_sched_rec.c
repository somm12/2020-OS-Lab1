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
   for (int j = -1; j <= time; j++){ // 20 = sum of the service time
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

   return;
}


/************************************************ FIFO Implementation  **********************************************/


void fifo(process arr[],  Queue * pq, int total_time, int process_num){
    Queue output;
    QueueInit(&output);
	int k = 0;
    int total_service_time = 0;
	process running[1] = {{-2,-2}};             // declaration of structure array
    process init[1]= {{-1,-1}};                 // this is for the case the first process's arrive time is not zero. because of Qpeek.
    Enqueue(pq, init[0]);
    for (int i = 0 ; i < total_time ; i++){     // i = time (x axis) 

        for (int j = 0 ; j < process_num ; j++){    // j = number of process
            if (arr[j].arrive_time == i) {                  // if there is a process has same arrive time with i
                if(i == 0){Dequeue(pq);}                    // this is the case for the first procees's arrive time is zero. because of Dequeue.when the queue is empty. error
                Enqueue(pq, arr[j]);                        // enqueue the process
            }
        }
        if(k == 0){
			if (QPeek(pq).arrive_time == i){
				running[0].arrive_time = -1;                            // when the first fast process begin to start, assign -1 value to start work.
				k = 1;
			}
        }
		if (running[0].arrive_time == -1){                          // if none of process is running
            running[0] = QPeek(pq);                             // put process into running struct
			Dequeue(pq);                                            // pull out the process
			printf("i : %d, arrive_time : %d, service_time : %d\n", i, running[0].arrive_time, running[0].service_time);
			Enqueue(&output, running[0]);						// enqueue the process into queue
		}
		        
        running[0].service_time -= 1;                           // decreases the remain service time
        if (running[0].service_time == 0) {                     // if service time becomes zero
            running[0].arrive_time == -1;                       // stop running and initialize it
        }

        if(QIsEmpty(pq) && running[0].service_time == 0) break;

    }
    int i = 0;
    while(!QIsEmpty(&output)){
        arr[i] = Dequeue(&output);// make structure array which is sorted for output
    }
	for (int i=0 ; i < process_num ; i++){
		printf("arr[%d].arrive_time = %d\n",i, arr[i].arrive_time);
		printf("arr[%d].service_time = %d\n",i, arr[i].service_time);
		total_service_time += arr[i].service_time;
	}
	graph(arr, process_num, total_service_time);
}
