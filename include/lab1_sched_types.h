/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Studend id : 32182490
*	    Student name : An Somin
*	    Student id : 32182110
*	    Student name : Seo Museong
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
    int arrive_time;
    int service_time;
	char name;
	int priority;
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

/*********************************** Function Header **********************************/

void graph(process arr[], int size, int time);
void fifo_graph(process arr[], int size, int time);
void fifo(process arr[], Queue * pq, int total_time, int process_num);
void rr(process arr[], Queue * pq, int total_time, int process_num);
void mlfq(process arr[], int total_time, int process_num);
void stride(process arr[], Queue* pq,int time, int size);
void bubble_sort(process arr[], int size);

void fifo_workload_table(int size, process arr[]);

#endif /* LAB1_HEADER_H*/
