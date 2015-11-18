// Five threads increment the common counter. They know the upper limit that is
// 5 000 000. The main thread waits until upper limit has been reached.
// There are three problems in this program. 
// 1. The main thread wastes resources because it waits in the busy loop.
// 2. Threads at least sometimes do extra steps
// 3. The main thread cannot detect the situation when counter becomes exactly 
//    MAXCOUNT.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>
#include <signal.h>
#include <mqueue.h>
#include <string.h>
#define N 5
#define MAXCOUNT 5000000
#define MSG_SIZE 5

typedef char MSG[5];
int counter;  // Common counter
int notify = 0;
sem_t sem;
struct sigevent sigevent;

void *tf(void *param){
    int i;
    mqd_t mq;
    MSG msg;
    char msg_number[] = "Done";
    while(counter < MAXCOUNT){
        sem_wait(&sem);
	if(counter < MAXCOUNT)
		counter++;
	sem_post(&sem);
    }
	
    sem_wait(&sem);
	if(!notify){
	notify = 1;
	sem_post(&sem);
	
	mq = mq_open("/msgqueue", O_WRONLY);
	if(mq == -1){
		perror("Writer mq_open");
		pthread_exit(NULL);
	}	
	
        strncpy(msg, msg_number, MSG_SIZE-1);
	msg[MSG_SIZE-1] ='\0';
	if(mq_send(mq, msg, MSG_SIZE, 5) == -1){
		perror("msg_send");
	}
	mq_close(mq); 	
    }
    else
        sem_post(&sem);
    
    pthread_exit(NULL);
}

void thread_handler(int signo, siginfo_t* siginfo, void* p){
	int result;
        MSG msg;
	mqd_t mq = siginfo->si_value.sival_int;
	result = mq_receive(mq, msg, MSG_SIZE, NULL);
	if(result > 0){
		printf("%s: Signal sent by thread about counter and it's value is %d\n", msg, counter);
		fflush(stdout);
	}
	else{
		perror("read thred_handler");
	}
	return;
}

int main(void) {
    int i;
    pthread_t ti_arr[N];
    counter = 0;
    mqd_t mq;
    struct mq_attr mqattrb;
    
    mqattrb.mq_msgsize = MSG_SIZE;
    mqattrb.mq_maxmsg = 1;
    mqattrb.mq_flags = 0;
    mqattrb.mq_curmsgs = 0;

    struct sigaction action;

    mq_unlink("/msgqueue");

    mq = mq_open("/msgqueue", O_RDONLY|O_CREAT, S_IRUSR|S_IWUSR, &mqattrb);
    if(mq == -1){
	perror("Main thread mq_open");
	exit(0);	
    }

    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = thread_handler;
    sigaction(SIGRTMIN, &action, NULL);
    
    sigevent.sigev_notify = SIGEV_SIGNAL;
    sigevent.sigev_signo = SIGRTMIN;
    sigevent.sigev_value.sival_int = mq;
    mq_notify(mq, &sigevent);	

    //init not shared semaphore
    if( sem_init(&sem, 0, 1) == -1){
	perror("sem_init");
	exit(0);	
    }
    sem_wait(&sem);    
    for (i = 0 ; i < N ; i++) {
        if (pthread_create(ti_arr+i, NULL, tf, NULL) != 0) {
            printf("Error in creating a thread %d\n", i);
            exit (0);
        }
    }
    sem_post(&sem);
    
  //  while (counter < MAXCOUNT);
  //	printf("Counter is after the while loop %d\n", counter);
  //wait for a notificitation from threads that counter max
    pause();
    mq_close(mq);
    if(mq_unlink("/msgqueue") == -1) {
	perror("Main thread unlink");
	exit(0);
    }

    for ( i = 0 ; i < N ; i++) {
		pthread_join(ti_arr[i], NULL);
      }
	printf("Counter is when all sub threads have finnished %d\n", counter);
    sem_destroy(&sem);       
    return 0;
}

