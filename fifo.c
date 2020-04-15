#include <stdio.h>

typedef struct{
	//process_name string
	int arrive_time;
	int service_time;
}process

void fifo(process arr[],  Queue * pq, int total_time){
	Queue* output;
	int k = 0;
	process running[1];/// declaration of structure array
	process arr[1]= {{-1,-1}};// this is for the case the first process's arrive time is not zero. because of Qpeek.
	Enqueue(&pq, arr[0]);
	for (int i = 0 ; i < total_time ; i++){		// i = time (x axis) 

		for (int j = 0 ; j < sizeof(arr) / sizeof(process) ; j++){	// j = number of process
			if (arr[j].arrive_time == i) {		// if there is a process has same arrive time with i
				if(i == 0){Dequeue(&pq);}// this is the case for the first procees's arrive time is zero. because of Dequeue.when the queue is empty. error
				Enqueue(&pq, arr[j]);		// enqueue the process
			}
		}
		if(Qpeek(&pq)==i && k == 0){
			running[0].arrivetime = -1;// when the first fast process begin to start, assign -1 value to start work.
			k = 1;
		}

		if (running[0].arrive_time == -1){		// if none of process is running
			running[0] = QPeek(&pq);			// put process into running struct
			Dequeue(&pq);				// pull out the process
		}
		else{						// if some process is running
			if (running[0].arrive_time == i){	// if it is the first time the process has runned
				Enqueue(&output, running[0]);	// enqueue the process into queue
			}
			running[0].service_time -= 1;		// decreases the remain service time
			if (running[0].service_time == 0) {	// if service time becomes zero
				running[0].arrive_time == -1;	// stop running and initialize it
			}
		
		}
		if(QIsEmpty(&pq)&& running[0].service_time == 0)break;
		
	}

	while(!QIsEmpty(&output)){
		arr[sizeof(arr) / sizeof(process)] = Dequeue(&output);// make structure array which is sorted for output
		//(suit format:struct array) to graph funtion input)	
	}
<<<<<<< HEAD
	graph(arr,sizeof(arr) / sizeof(process));//call funtion to draw the FIFO graph
=======
	graph(arr,sizeof(arr) / sizeof(process));////call funtion to draw the FIFO graph

>>>>>>> 4ef7505fe9af0f57b2c83fee1c4d96f1ab7326cb
}
