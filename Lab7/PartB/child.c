#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int i, t, fd;
	char *wr = argv[2];
	
	fd = atoi(argv[1]);
	t = atoi(argv[3]);

	
	for(i = 0; i<t; i++){
			write(fd, (void *)wr, 4);
			//sleep(1);
	}

	close(fd);
	exit(0);		
}