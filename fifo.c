#include <stdio.h>

type process struct{
	//process_name string
	arrive_time int
	service_time int
}

struct process running[]; // declaration of structure array

void fifo(process arr[],  Queue * pq){
	for (int i = 0 ; i < total_arrive_time ; i++){				// i = time (x axis) 

		for (int j = 0 ; j < process number ; j++){				// j = number of process
			if (arr[j].arrive_time == i) {						// if there is a process has same arrive time with i
				Enqueue(pq, arr[j]);							// enqueue the process
			}
		}

		if (running[0].arrive_time == -1){						// if none of process is running
			running[0] = QPeek(pq);								// put process into running struct
			Dequeue(pq);										// pull out the process
		}
		else{													// if some process is running
			if (running[0].arrive_time == i){					// if it is the first time the process has runned
				Enqueue(output, running[0]);					// enqueue the process into queue
			}
			running[0].service_time -= 1;						// decreases the remain service time
			if (running[0].service_time == 0) {					// if service time becomes zero
				running[0].arrive_time == -1;					// stop running and initialize it
			}
		}
	}
}
