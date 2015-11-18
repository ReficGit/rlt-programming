#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(void)
{
	struct timespec nano_sleep;
	pid_t child[5], pid, wpid;
	int n, i,j;
        int fd[3][2];
        //int fd[2];
        int myf[2];
	char chr[1];

	nano_sleep.tv_sec = 0;
	nano_sleep.tv_nsec = 500000000;
	pipe(myf);
	for(i = 0; i<5; i++){
		pipe(fd[i]);	
	}
	//pipe(fd);
	for(i = 0; i<5; i++){
		pid = fork();	
		if(pid == 0){
			nanosleep(&nano_sleep, NULL);
			//child
			if(i>0){
				for(j=0;j<5;j++){
					if(j == (i-1)){
						close(fd[j][0]);
						continue;
					}
					if(j == i){
						close(fd[j][1]);
						continue;
					}
					close(fd[j][0]);
					close(fd[j][1]);
				}
			}
			else{
				close(fd[i][1]);
				for(j=1;j<4;j++){		
					close(fd[j][0]);
					close(fd[j][1]);
				}
			}
			while(1){
				if(i == 0){
					n = read(fd[i][0], &chr, 1);
					if(n>0){
						write(STDOUT_FILENO, &chr, 1);
					}//if n>0
					else if(n==0){
						close(fd[i][0]);
						exit(1);
					}
				}//if i==0
				else if(i < 4){
					n = read(fd[i][0], &chr, 1);
					if(n>0){
						chr[0] = chr[0] + 1;
						write(fd[i-1][1], &chr, 1);
					}//n=if n>0
					else if(n  == 0){
						close(fd[i-1][1]);
						close(fd[i][0]);
						exit(0);
					}
					else{
						perror("Read");
					}
				}//if i<4
				else if(i == 4){
					n = write(fd[i-1][1], "A", 1);
					if(n == -1){
						perror("Write");
					}
					if((close(fd[i-1][1]))==-1){
						perror("Close fd");
					}
					exit(0);	
				}//else if
			}//while
		}//if child[i]
	}//for loop
	for(j=0;j<5;j++){
		close(fd[j][0]);
		close(fd[j][1]);
	} 
	while((wpid = waitpid(-1, NULL, 0)) > 0){
		printf("\n%d terminated\n", wpid);
	}
	printf("\nExit\n");
	exit(0);
}	
