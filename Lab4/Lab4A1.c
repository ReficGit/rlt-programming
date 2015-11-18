#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	FILE *fp = stdout;
	int i, bsize = 5;
	char my_buff[bsize];
	char c = 'A';
	
	setvbuf(fp, my_buff, _IOFBF, bsize);
	
	for(i = 0; i<13; i++){
		putchar(c);
		sleep(1);
	}
	
	//CASE A: the buffer is forced to print characters after "for-loop" ends with fflush comand.
	//thus, 3 left characters are printed after last 3 seconds
	//fflush(fp);	
	//return 0;
	
	//CASE B: the buffer prints characters despite the fact that we did not call fflush(stdout)
	//terminates the calling process immediately. Any open file descriptors belonging to the 
	//process are closed and any children of the process are inherited by process 1, 
	//init, and the processs parent is sent a SIGCHLD signal.
	//exit(0);
	
	//CASE C: the program just terminates without releazing buffer.
	return 0;
}