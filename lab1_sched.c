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

/************************************************ FIFO Implementation  **********************************************/

void fifo(process arr[],  Queue * pq, int total_time){
    Queue output;
    QueueInit(&output);
    int k = 0;
    process running[1] = {{-1,-1}};             // declaration of structure array
    process init[1]= {{-1,-1}};                 // this is for the case the first process's arrive time is not zero. because of Qpeek.
    Enqueue(pq, init[0]);
    for (int i = 0 ; i < total_time ; i++){     // i = time (x axis) 

        for (int j = 0 ; j < process_num ; j++){    // j = number of process
            if (arr[j].arrive_time == i) {                  // if there is a process has same arrive time with i
                if(i == 0){Dequeue(pq);}                    // this is the case for the first procees's arrive time is zero. because of Dequeue.when the queue is empty. error
                Enqueue(pq, arr[j]);                        // enqueue the process
            }
        }
        if(k == 00 && QPeek(pq).arrive_time == i){
            running[0].arrive_time = -1;                            // when the first fast process begin to start, assign -1 value to start work.
            k = 1;
        }
if (running[0].arrive_time == -1){                          // if none of process is running
            running[0] = QPeek(pq);                             // put process into running struct
            Dequeue(pq);                                            // pull out the process
        }
        else{                                                       // if some process is running
            if (running[0].arrive_time == i){                       // if it is the first time the process has runned
                Enqueue(&output, running[0]);                       // enqueue the process into queue
            }
            running[0].service_time -= 1;                           // decreases the remain service time
            if (running[0].service_time == 0) {                     // if service time becomes zero
                running[0].arrive_time == -1;                       // stop running and initialize it
            }

        }
        if(QIsEmpty(pq) && running[0].service_time == 0) break;

    }
    int i = 0;
    while(!QIsEmpty(&output)){
        arr[i] = Dequeue(&output);// make structure array which is sorted for output
    }
for (int i=0 ; i < process_num ; i++){
    print("arr[%d].arrive_time = %d",i, arr[i].arrive_time);
    print("arr[%d].service_time = %d",i, arr[i].service_time);
}
