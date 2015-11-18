#include <pthread.h> //threads
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define N 10
#define T 1000000

int inc_all;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *threadIncrement(void *p){
	int g = 0;

	*(int*)p = 0;
	
	for(g = 0; g<T; g++){
		//pthread_mutex_lock(&mutex);
		(*(int*)p)++;
		pthread_mutex_lock(&mutex);
		inc_all++;		
		pthread_mutex_unlock(&mutex);	
	}
	pthread_exit(NULL);
}

int main(void){
	int i, j, k;
	pthread_t threadID[N];
	int inc[N];
			
	inc_all = 0;
//	pthread_mutex_lock(&mutex);
	//initialize inc values and create threads
	for(i = 0; i<N; i++){
	//	inc[i] = 0;
		if(pthread_create(&threadID[i], NULL, threadIncrement, &inc[i]) != 0) {
			perror("pthread_create");
			exit(1);
		}
	}
//	pthread_mutex_unlock(&mutex);
	//wait for threads
	for(j = 0; j<N; j++){
		if(pthread_join(threadID[j], NULL) != 0){
	//		perror("pthread_join");
			exit(1);
		}
	}		
	
	//print each thread's own increment value
	for(k = 0; k<N; k++){
		printf("Thread %d: %d\n", k, inc[k]);
	}
	//print common increment value
	printf("Common increment value: %d", inc_all);
	return 0;
}
