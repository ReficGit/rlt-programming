#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

static void signal_handler(int sig_no);

int main(void)
{
	sigset_t mask;
	int signo;

	sigemptyset(&mask);
	sigaddset(&mask, SIGALRM);
	
	signal(SIGALRM, signal_handler);
	
	sigwait(&mask, &signo);
	if(signo == SIGALRM){
		printf("\n!!!Sigalarm received!!!\n");
	}
	
	exit(0);
}

static void signal_handler(int sig_no)
{
	return;
}