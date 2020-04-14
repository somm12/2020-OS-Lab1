#include <stdio.h>

void fifo(process arr[],  Queue * pq){
	for (int i = 0 ; i < total_arrive_time ; i++){ // i = time (x axis) 

		for (int j = 0 ; j < process number ; j++){ // j = number of process
			if (j.arrive_time == i) {
				Enqueue(pq, j);
			}
		}

		if (running == -1){ //if none of process is running
			running = QPeek(pq);
			Dequeue(pq, j);
		}
		else{
			output[i] = running;
			j.service_time -= 1;
			if (j.service_time == 0) {
				running == -1;
			}
		}
	}
