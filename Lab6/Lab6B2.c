#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
	pid_t pid;
	int fd, result, i;
	const char wr[5] = "AAAAA";
	char out;
	char m_fd[1];
		
	fd = open("Lab6B2.txt", O_CREAT | O_RDWR, S_IRWXU, S_IRWXG);

	if(fd == 0){
		perror("Fd open:");
		exit(1);
	}
	
	pid = fork();
	if(pid < 0){
		perror("Fork:");
		exit(1);
	}
	if(pid == 0) // child
	{
		sprintf(m_fd, "%d", fd);
		execl("./child.exe", "./child.exe", m_fd, NULL);	
		perror("exec");
		exit(0);
	}
	
	// parent	
	while((pid=waitpid(-1, NULL, WNOHANG)) >=0){
		write(STDOUT_FILENO, "Parent is working\n", 18);
		sleep(1);
	}
	
	write(STDOUT_FILENO, "Child terminated\n", 17);
	
	lseek(fd, 0L, 0);
	while(result=(read(fd, &out, 1)) != 0){
		write(STDOUT_FILENO, &out, 1);
	}
	
	write(STDOUT_FILENO, "\n", 1);
	exit(0);		
}