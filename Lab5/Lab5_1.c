#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void print(char c);

int main(void)
{	
	FILE *fp = stdout;
	struct rlimit old;
	pid_t pid;
	int i = 0;
	int j = 0;
	
	setbuf(fp, NULL);
	
	if(getrlimit(RLIMIT_NPROC, &old) == -1 ){
		perror("");
	}
	else {		
		printf("Current process limit is %d processes.\n", (int)old.rlim_cur);
	}
	
	pid = fork();
	if(pid == 0){
		print('A');
		exit(0);
	}
	pid = fork();
	if(pid == 0){
		print('B');
		exit(0);
	}
	pid = fork();
	if(pid == 0){
		print('C');
		exit(0);
	}
	pid = fork();
	if(pid == 0){
		print('D');
		exit(0);
	}
	pid = fork();
	if(pid == 0){
		print('E');
		exit(0);
	}
	
	while ((pid = waitpid(-1, NULL, 0)) > 0) {
	printf("\nChild %d has terminated\n", pid);
	}

	
}

void print(char c)
{
	int i = 0;
	FILE *fp = stdout;
	setbuf(fp, NULL);
	for(i=0; i<5; i++){
		printf("%c", c);
		sleep(1);
	}
	
}