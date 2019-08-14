#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
#include <pthread.h> 
#include "time.h"

void* func(void* vargp){
	sleep(1);
	
	printf("Printing GeeksQuiz from Thread \n"); 
    return NULL; 
}

int main(){
	time_t start = time(0);
	sleep(1);
	int diff = (int) difftime(time(0), start);
	printf("%d \n", diff);
	printf("compileTest\n");
	pthread_t thread_id; 
	printf("Before Thread\n"); 
    
    printf("Blarg");
    printf("Blarg");
    printf("Blarg");
    printf("Blarg");
    pthread_create(&thread_id, NULL, func, NULL);
    printf("Blarg");
    printf("Blarg");
    printf("Blarg");
    printf("Blarg"); 
    //pthread_join(thread_id, NULL); 
    printf("After Thread\n"); 
	
	int a[15];
	memset(a, 1, 15);
	
	//int b[15];
	
	return 0;
}
