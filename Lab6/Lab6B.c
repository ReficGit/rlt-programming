#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int create_p(pid_t pid, int i);

int main(void)
{
	int i = 0;
	int stat = 0;
	pid_t pid;
	pid_t pidd;
	
	create_p(pidd, i);
	for(i = 0; i<5; i++){
		pid = fork();
		if(pid < 0){
			perror("Fork:");
			exit(1);
		}
		if(pid > 0){
			printf("Waiting for child %d\n", pid);
			wait(&stat);
			sleep(1);
			if(WIFEXITED(stat)){
					printf("Child's %d exit code %d\n", pid, WEXITSTATUS(stat));
					stat;
			}
				else
					printf("Child did not terminate with exit\n");
				
				
				exit(WEXITSTATUS(stat)+1);
		}
		
	}
	
	sleep(1);
	exit(0);
}

int create_p(pid_t pid, int i)
{
	int stat;
	pid = fork();
	if(pid < 0){
		perror("Fork:");
		exit(1);
	}
	if (pid == 0){
		i++;
		if(i == 5){
			sleep(1);
			exit(0);
		}
		else {
			create_p(pid, i);
			exit(i);
		}
	}
	if(pid > 0){
		printf("Waiting for child %d\n", (long)getpid());
		wait(&stat);
		sleep(1);
		if(WIFEXITED(stat)){
				printf("Child's %d exit code %d\n", (long)getpid(), WEXITSTATUS(stat));
				stat++;
		}
			else
				printf("Child did not terminate with exit\n");
		return(stat);
	}
	sleep(1);
	exit(0);
}
