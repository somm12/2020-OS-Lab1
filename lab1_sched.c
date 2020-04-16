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
#include "ListBaseQueue.h"


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

/************************************************ FIFO Implementation  **********************************************/


