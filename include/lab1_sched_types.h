/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 
*	    Student name : 
*
*   lab1_sched_types.h :
*       - lab1 header file.
*       - must contains scueduler algorithm function's declations.
*
*/

#ifndef _LAB1_HEADER_H
#define _LAB1_HEADER_H


/*
 * You need to Declare functions in  here
 */

typedef struct process{
	//process_name string
	int arrive_time;
	int service_time;
} __attribute__ ((packed)) process;

typedef process data;


/********************************* Queue Header *************************************/

#define TRUE   1
#define FALSE   0

typedef int Data;

typedef struct _node
{
   Data data;
   struct _node * next;
} Node;

typedef struct _lQueue
{
   Node * front;
   Node * rear;
} LQueue;

typedef LQueue Queue;

void QueueInit(Queue * pq);
int QIsEmpty(Queue * pq);

void Enqueue(Queue * pq, Data data);
Data Dequeue(Queue * pq);
Data QPeek(Queue * pq);

/**************************************************************************/


void fifo(process arr[],  Queue * pq, int total_time);



#endif /* LAB1_HEADER_H*/
