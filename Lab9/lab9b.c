#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(void)
{
	struct timespec nano_sleep;
	pid_t child[5], pid, wpid;
	int n, i,j;
  //      int fd[3][2];
        //int fd[2];
//        int myf[2];
	int mfd[2];
	int save_stdout;
	char chr[1];
	save_stdout = dup(1);
	nano_sleep.tv_sec = 0;
	nano_sleep.tv_nsec = 500000000;
	pipe(mfd);
	//for(i = 0; i<5; i++){
	//	pipe(fd[i]);	
	//}
	//dup2(mfd[1], 1);
	//close(mfd[1]);
	//pipe(fd);
	for(i = 0; i<5; i++){
		pid = fork();
		printf("FORKING");	
		if(pid == 0){
			dup2(mfd[1], STDOUT_FILENO);
			close(mfd[1]);
			nanosleep(&nano_sleep, NULL);
			while(1){
				if(i == 0){
					n = read(mfd[0], &chr, 1);
					if(n>0){
						dup2(save_stdout, STDOUT_FILENO);
						write(STDOUT_FILENO, &chr, 1);
					}//if n>0
					else if(n==0){
						close(mfd[0]);
						exit(1);
					}
				}//if i==0
				else if(i < 4){
					n = read(mfd[0], &chr, 1);
					if(n>0){
						chr[0] = chr[0] + 1;
						write(STDOUT_FILENO, &chr, 1);
					}//n=if n>0
					else if(n  == 0){
						close(STDOUT_FILENO);
						close(mfd[0]);
						exit(1);
					}
					else{
						perror("Read");
						exit(1);
					}
				}//if i<4
				else if(i == 4){
			//		printf("WRITING\n");
					n = write(STDOUT_FILENO, "A", 1);
					if(n == -1){
						perror("Write");
					}
					if((close(STDOUT_FILENO))==-1){
						perror("Close fd");
					}
					exit(0);	
				}//else if
			}//while
		}//if child[i]
	}//for loop
	close(mfd[1]);
//	close(STDOUT_FILENO);
	//restore
//	dup2(save_stdout, 1);
//	close(save_stdout);
	printf("HERE\n");
	while((wpid = waitpid(-1, NULL, 0)) > 0){
		printf("\n%d terminated\n", wpid);
	}
	printf("\nExit\n");
	exit(0);
}	
