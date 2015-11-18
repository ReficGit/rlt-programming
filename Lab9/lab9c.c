#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h> //exit
#include <stdio.h>

//reader
int main()
{
    int fd, num, n = 0;
    char *myfifo = "fifo_max";
	char buf[3];


    if((mkfifo(myfifo, S_IRWXU)) == -1){
			perror("Mkfifo");
			exit(0);
	}

    fd = open(myfifo, O_RDONLY);

	
    while(1){
		n = read(fd, buf, 1);
		if(n>0){
			num = atoi(buf);
			printf("%d\n", num);
			if(num == 6){
				close(fd);
				break;
			}
		}
		else if(n == -1){
			perror("Read");
			break;
		}
	}

    /* remove the FIFO */
    unlink(myfifo);

    return 0;
}