#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

static void signal_handler(int sig_no);

int main(void)
{
	sigset_t mask;
	int signo;
	pid_t pid;
	
	sigemptyset(&mask);
	sigaddset(&mask, SIGALRM);
	
	signal(SIGALRM, signal_handler);
	alarm(5); //parent request the alarm
	
	pid = fork();
	if(pid == 0){
		printf("\nChild started\n");
		sigwait(&mask, &signo);
		if(signo == SIGALRM){
			printf("\nSigalarm received by child");
		}
		
		printf("\nChild exit\n");
		exit(1);
	}
	
	sigwait(&mask, &signo);
	if(signo == SIGALRM){
		printf("\nSigalarm received by parent\n");
	}
	
	while((pid=waitpid(-1, NULL, WNOHANG)) >=0){
		printf("\nWaiting for child\n");
		sleep(2);
	}
	
	printf("\nChild Terminated\n");
	
	exit(0);
}

static void signal_handler(int sig_no)
{
	return;
}