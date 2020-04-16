#include <stdio.h>

typedef struct{
	//process_name string
	int arrive_time;
	int service_time;
}process
void fifo((process arr[],  Queue * pq, int total_time));
void graph(process arr[],int size);
int main(void) {

	Queue pq;
	QueueInit(&pq);
   process arr[5] = { {2,6},{4,4},{0,3},{6,5},{8,2} };// 차례로 A B C
   fifo(arr, &pq,20);

   return 0;
}

void fifo(process arr[],  Queue * pq, int total_time){
	Queue* output;
	QueueInit(&output);
	int k = 0;
	process running[1]= {{-1,-1}};/// declaration of structure array
	process init[1]= {{-1,-1}};// this is for the case the first process's arrive time is not zero. because of Qpeek.
	Enqueue(pq, init[0]);
	for (int i = 0 ; i < total_time ; i++){		// i = time (x axis) 

		for (int j = 0 ; j < sizeof(arr) / sizeof(process) ; j++){	// j = number of process
			if (arr[j].arrive_time == i) {		// if there is a process has same arrive time with i
				if(i == 0){Dequeue(pq);}// this is the case for the first procees's arrive time is zero. because of Dequeue.when the queue is empty. error
				Enqueue(pq, arr[j]);		// enqueue the process
			}
		}
		if(QPeek(pq).arrive_time ==i && k == 0){
			running[0].arrive_time = -1;// when the first fast process begin to start, assign -1 value to start work.
			k = 1;
		}
		// 이 아래 if else문은 첫 프로세스가 실행 시작하고 나서부터 의미있으니까 k변수를 조건문으로 내세운다.
		if (running[0].arrive_time == -1){		// if none of process is running
			running[0] = QPeek(pq);			// put process into running struct
			Dequeue(pq);				// pull out the process
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
		
		if(QIsEmpty(pq)==0&& running[0].service_time == 0)break;
		
	}

	while(!QIsEmpty(&output)){
		arr[sizeof(arr) / sizeof(process)] = Dequeue(&output);// make structure array which is sorted for output
		//(suit format:struct array) to graph funtion input)	
	}

	graph(arr,sizeof(arr) / sizeof(process));////call funtion to draw the FIFO graph
}

void graph(process arr[],int size) {
   int sum = 0;

   char temp[5] = { 'A','B','C','D','E' };
   for (int j = -1; j <= 20; j++){ // 20 = sum of the service time
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
      for (int k = 0; k < arr[i].servicetime; k++) {
            printf("■■ ");

      }
      printf("\n");
      if (i < 4)
      {
         printf("%c", temp[i + 1]);
         printf(" | ");
      }
      sum = sum + arr[i].servicetime;
      for (int j = 0; j < sum; j++) {
         printf("   ");

      }
   }
   printf("\n");

   return;
}
