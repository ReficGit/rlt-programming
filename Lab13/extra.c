#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <semaphore.h>

#define MAXCOUNTER 500000
#define MSG_SIZE 1

typedef char MSG[MSG_SIZE];
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t con = PTHREAD_COND_INITIALIZER;
int turn = 1;
mqd_t main_thr, sub_thr;
 
void *tf(void *param){
	int i, result;
	MSG msg;
	
	for(i = 0; i< MAXCOUNTER; i++){
//		mq_receive(sub_thr, msg, MSG_SIZE, NULL);
//		printf("2");
//		mq_send(main_thr, " ", MSG_SIZE, 5);
	pthread_mutex_lock(&mut);
		while(turn == 1){
			pthread_cond_wait(&con, &mut);
		//	turn = 1;
		}
		turn = 1;
		printf("2");
		pthread_cond_signal(&con);
		pthread_mutex_unlock(&mut);
	}
	pthread_exit(NULL);
}

int main(void){
	pthread_t thr;
	int i, result;
	MSG msg;
	struct mq_attr mq;
	
	mq.mq_flags = 0;
	mq.mq_maxmsg = 4;
	mq.mq_msgsize = 1;
	mq.mq_curmsgs = 0;

	main_thr = mq_open("/main", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR, &mq);
	sub_thr = mq_open("/sub", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR, &mq);
	if(main_thr == -1 || sub_thr == -1){
		perror("mq_open");
		exit(0);
	}
	if(pthread_create(&thr, NULL, tf, NULL) != 0){
		perror("pthread_create");
		exit(0);
	}
	for(i = 0; i < MAXCOUNTER; i++){
	//	printf("1");
	//	mq_send(sub_thr, " ", MSG_SIZE, 5);
	//	mq_receive(main_thr, msg, MSG_SIZE, NULL);
		pthread_mutex_lock(&mut);
		while(turn == 2){
			pthread_cond_wait(&con, &mut);
			//turn = 2;
		}
		turn = 2;
		printf("1");
                pthread_cond_signal(&con);
		pthread_mutex_unlock(&mut);
	}
	mq_close(main_thr);
	mq_close(sub_thr);
	mq_unlink("/main");
	mq_unlink("/sub");
	pthread_join(thr, NULL);
	return;
}
