#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int i;
	const char wr[5] = "AAAAA";
	//int fd = atoi(argv[1]);
	int fd = argv[1][0] - '0';
	//int fd;
	sscanf(argv[1], "%d", &fd);

	
	for(i = 0; i<5; i++){
			write(fd, &wr, sizeof(wr));
	}

	close(fd);
	exit(0);
		
}