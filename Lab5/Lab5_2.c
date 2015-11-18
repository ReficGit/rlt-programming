#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
	pid_t pid;
	int rw_file, fellow_desc, result = 1, i = 0;
	char chr_fellow, chr_my;
	char out[1];
	int n = 0;
	int status;
	int fd[1];
	int val = 0;
	char value[5];
	char pidd[10];
	
	pipe(fd);
	rw_file = open("rw_file.txt", O_CREAT | O_RDWR, S_IRWXU, S_IRWXG);
	
	if(rw_file == 0){
		perror("");
		exit(1);
	}
	
	if((pid = fork()) < 0){
		perror("OOPS:");
		exit(1);
	}
	if(pid == 0){
		fellow_desc = OpenChatFellow();
		close(fd[0]);
		while(result != 0){	
		result = read(fellow_desc, &chr_fellow, 1);
			if (result == 1){
				write(STDOUT_FILENO, &chr_fellow, 1);
				write(STDOUT_FILENO, "\n", 1);
				write(rw_file, &chr_fellow, 1);
				i++;
				val++;
				if(i%7==0){
					write(rw_file, "\n", 1);
				}
			}
		}
		close(fellow_desc);
		write(STDOUT_FILENO, "\nEnd Of File. Child killed\n", 27);
		write(fd[1], &val, sizeof(val));
		close(fd[1]);
		exit(0);
	}
	else if (pid > 0){
		close(fd[1]);
		//write(STDOUT_FILENO, "\nHere is Parent\n", 16);
		while(1){	
		result = read(STDIN_FILENO, &chr_my, 1);
		if (result == 1){
			if(chr_my == 'Q' || chr_my == 'q'){
				break;
			}
			write(STDOUT_FILENO, &chr_my, 1);
			write(STDOUT_FILENO, "\n", 1);
			}
		}
		lseek(rw_file, 0L, 0);
		write(STDOUT_FILENO, "\n", 1);
		while(n=(read(rw_file, &out, 1)) != 0){
			write(STDOUT_FILENO, &out, 1);
		}	
		
		while ((pid = waitpid(0, NULL, 0)) > 0) {
		write(STDOUT_FILENO, "\nChild ", 7);
	    sprintf(pidd, "%9d", pid);
		write(STDOUT_FILENO, pidd, sizeof(pidd));
		write(STDOUT_FILENO, " has terminated\n", 16);
		result = read(fd[0], &val, sizeof(val));
		if(result > 0){
				write(STDOUT_FILENO, "\nChild sent value: ", 19);
				sprintf(value, "%4d", val);
				write(STDOUT_FILENO, value, sizeof(value));
				write(STDOUT_FILENO, "\n", 1);
			}
		}
		write(STDOUT_FILENO, "\n", 1);
		exit(0);
	}	
}