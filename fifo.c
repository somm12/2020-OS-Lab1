#include <stdio.h>

void fifo(process arr[],  Queue * pq){
	for (int i = 0 ; i < total_arrive_time ; i++){			// i = time (x axis) 

		for (int j = 0 ; j < process number ; j++){			// j = number of process
			if (arr[j].arrive_time == i) {					// if there is a process has same arrive time with i
				Enqueue(pq, arr[j]);						// enqueue the process
			}
		}

		if (running == -1){									// if none of process is running
			running = QPeek(pq);							// put process into running struct
			Dequeue(pq);									// pull out the process
		}
		else{												// if some process is running
			output[i] = running;							// save that process to the output struct
			arr[j].service_time -= 1;						// decreases the remain service time
			if (arr[j].service_time == 0) {					// if service time becomes zero
				running == -1;								// stop running and initialize it
			}
		}
	}
