#include <unistd.h> //pipe, fork, write
#include <stdio.h> //perror
#include <stdlib.h> //exit


int main(void)
{
	int my_fd[2];
	pid_t pid;
	int n,i,num = 0;
	char chr, num_chr[1];

	if((pipe(my_fd))==-1){
		perror("Pipe error");
	}	
	pid = fork();
	if(pid == -1){
		perror("Fork error:");
	}
	else if(pid == 0){
		//child	
		close(my_fd[0]); // read descr
		for(i=0;i<10;i++){
		 // sleep(1);
	          sprintf(num_chr, "%d", i);
		  write(my_fd[1], &num_chr , 1);	
  		  sleep(1);
		}
		close(my_fd[1]);
		exit(0);
	}

	close(my_fd[1]);
	while(1){
		n = read(my_fd[0], &chr, 1);
		if(n > 0){
			write(STDOUT_FILENO, &chr, 1);
		}
		else if(n==0) {
			printf("End of file\n");
			close(my_fd[0]);
			break;
		}
		else {
		 	perror("Read error:");
		}
	}	
}




