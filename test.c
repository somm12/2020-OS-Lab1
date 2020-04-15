#include<stdio.h>

typedef struct {
   int arrivetime;
   int servicetime;
}process;
void bubble_sort(process arr[], int count);
void FIFO(process arr[], int size);

int main(void) {
   
   process arr[5] = { {2,6},{4,4},{0,3},{6,5},{8,2} };// 차례로 A B C
   bubble_sort(arr, sizeof(arr) / sizeof(process));//구조체 배열 크기
   FIFO(arr, sizeof(arr) / sizeof(process));

   return 0;
}
void FIFO(process arr[],int size) {
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

void bubble_sort(process arr[], int count)    // 매개변수로 정렬할 배열과 요소의 개수를 받음
{
      int arri_t;
      int serv_t;

      for (int i = 0; i < count; i++)    // 요소의 개수만큼 반복
      {
         for (int j = 0; j < count - 1; j++)   // 요소의 개수 - 1만큼 반복
         {
            if (arr[j].arrivetime > arr[j + 1].arrivetime)          // 현재 요소의 값과 다음 요소의 값을 비교하여
            {                                 // 큰 값을
               arr[j + 1].arrivetime = arri_t;

               serv_t = arr[j].servicetime;
               arr[j].servicetime = arr[j + 1].servicetime;
               arr[j + 1].servicetime = serv_t;  // 다음 요소로 보냄
            }
         }
      }
   }
