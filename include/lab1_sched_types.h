/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id :32182490 
*	    Student name : An SoMin
*       
*       Student id :32182110
*       Student name: Seo MuSeong
*   lab1_sched_types.h :
*       - lab1 header file.
*       - must contains scueduler algorithm function's declations.
*
*/

#ifndef _LAB1_HEADER_H
#define _LAB1_HEADER_H

//This is structure for store process workload (arrive time,service time)
typedef struct process{
    //process_name string
    int arrive_time;
    int service_time;
	char process_name;
} __attribute__ ((packed)) process;

typedef process Data;

/********************************* Queue Header *************************************/

#define TRUE   1
#define FALSE   0

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

/****************************function for print FIFO*************************************/
void fifo_workload_table(int size, process arr[]);
void fifo(process arr[], Queue * pq, int total_time, int size);
void rr(process arr[], Queue *pq, int time, int size);
void graph(process arr[], int size, int total_service_time);
void rrgraph(process arr[], int size, int time);
#endif /* LAB1_HEADER_H*/



