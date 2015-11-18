#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h> //exit
#include <stdio.h>
#include <signal.h>

int exit_n = 0;
void sig_handler(int signum);

//writer
int main()
{
    int fd, num, n = 0;
    char *myfifo = "fifo_max";
	char buf[3];

    fd = open(myfifo, O_WRONLY);
	signal(SIGPIPE, sig_handler);
	
	num = 0;
    while(1){
		sleep(1);
		sprintf(buf, "%d", num);
		write(fd, &buf , 1);
		num++;
		//if(exit_n == 1){
		//	break;
		//}
	}
    close(fd);
    printf("Writer exiting\n");

    exit(0);
}

void sig_handler(int signum){
	printf("received sign\n");
	//exit_n = 1;
	exit(0);
}