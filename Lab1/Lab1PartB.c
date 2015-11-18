#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	const char msg[] = "1";
	
	while(1){
		write(STDOUT_FILENO, msg, sizeof(msg)-1);
	}
}