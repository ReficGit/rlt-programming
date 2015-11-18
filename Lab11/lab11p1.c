#include <unistd.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

#define c_val 10

int num_of_signals;
struct timespec cur_time;

void rtsig_handler(int signo, siginfo_t *info, void* p){
	int rc;
	//if(signo == SIGRTMAX)
	//printf("Sig 1 caught %d\n", signo);
	rc = clock_gettime(CLOCK_MONOTONIC, &cur_time);
	if(rc == -1)
		perror("Clock_gettime");
		//printf("Current time is: %ld s and %ld ns\n", cur_time.tv_sec, cur_time.tv_nsec);
//	else
//		perror("Clock_gettime:");
	num_of_signals++;
}

int main(void)
{
	struct itimerspec res;
	struct timespec t_res;
	struct timespec t_recvd;
	struct timespec t_sum;
	struct timespec t_nanosleep;
	struct sigaction sa;
	struct sigevent sevent;
	pid_t m_pid;
	sigset_t sig_mask;
	int rc;	
	timer_t timerid;
	union sigval value;
	int fd[2];
	int i;
	long time_sum = 0;
	 long variance[c_val];
	long deviation = 0;
	long mean = 0;
	long min = 0;
	long max = 0;
	num_of_signals = 0;
	if (pipe(fd) == -1){
		perror("Pipe");
	}	
		
	//display the resolution of the timers
	rc = clock_getres(CLOCK_MONOTONIC, &t_res);
	if(rc != -1)
		printf("Real time clock resolution is: %d s and %ld ns\n", t_res.tv_sec, t_res.tv_nsec);
	else
		perror("Clock_getres");
	
	
	m_pid = fork();
	if(m_pid == 0){//child
		//close writing pipe
		close(fd[1]);
		//install real time signal handler
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_SIGINFO;
	//	sa.sa_flags |= SA_RESTART;
		sa.sa_sigaction = rtsig_handler;
		sigaction(30, &sa, NULL);	
		
		//add sigrtmax to wait for. block others except sigrtmax
		sigfillset(&sig_mask);
		sigdelset(&sig_mask, 30);
		
		//read from pipe
		t_sum.tv_sec = 0;
		t_sum.tv_nsec = 0;
		i = 0;		
		while(num_of_signals != c_val){
			sigsuspend(&sig_mask);
			rc = read(fd[0], &t_recvd, sizeof(t_recvd));
			if(rc == -1){
				perror("read");			
			}
			else {
		//		printf("Sending time is: %ld s and %ld ns\n", t_recvd.tv_sec, t_recvd.tv_nsec);
		//		printf("Diff is: %d s and %ld ns\n", cur_time.tv_sec - t_recvd.tv_sec, cur_time.tv_nsec-t_recvd.tv_nsec);
		//		t_sum.tv_sec += cur_time.tv_sec - t_recvd.tv_sec;
				t_sum.tv_nsec += cur_time.tv_nsec - t_recvd.tv_nsec;
				variance[i++] = cur_time.tv_nsec-t_recvd.tv_nsec;
				if(num_of_signals == 1){
					min = variance[i-1];
					max = variance[1-1];
				}	
				else{
						min = (min < variance[i-1]) ? min : variance[i-1];	
						max = (max > variance[i-1]) ? max : variance[i-1];
				}
			}
		}
		
		mean = (t_sum.tv_nsec/c_val);
		for(i = 0; i<c_val;i++){
			deviation += ((variance[i] - mean)*(variance[i] - mean));
		//	printf("Dev %ld\n", deviation);
		}
		deviation = deviation/(c_val-1);
		deviation = sqrt(deviation);
		printf("%d signals received\nMean: %.2f us. Deviation %.2f us. Min %ld us. Max %ld us\n", num_of_signals, (mean/1000.0), deviation/1000.0, min/1000, max/1000);

		exit(0);
		
	}
	
	else {	
		//close reading pipe
		close(fd[0]);
		
		sevent.sigev_notify = SIGEV_SIGNAL;
		sevent.sigev_signo = 30;
		sevent.sigev_value.sival_int = 2;

		if(timer_create(CLOCK_MONOTONIC, &sevent, &timerid) == -1){
			perror("timer create");
		}
		res.it_value.tv_sec = 2;
		res.it_value.tv_nsec = 0;
		res.it_interval.tv_sec = 0;
		res.it_interval.tv_nsec = 0;

		if(timer_settime(timerid, 0, &res, NULL) == -1){
			perror("ts");

		}/*
		t_nanosleep.tv_sec = 0;
		t_nanosleep.tv_nsec = 0;
		for(i = 0; i<c_val; i++){
			t_nanosleep.tv_nsec = (rand()%32+1)*1000000;
			nanosleep(&t_nanosleep, NULL);
			value.sival_int = 1;
			clock_gettime(CLOCK_MONOTONIC, &t_recvd);
			sigqueue(m_pid, SIGRTMAX, value);
			rc = write(fd[1], &t_recvd, sizeof(t_recvd));
			if(rc == -1)
				perror("write");
	//		else	
	//			printf("Clock is sent\n");
		}*/
		wait(NULL);
		exit(0);
	}

}
