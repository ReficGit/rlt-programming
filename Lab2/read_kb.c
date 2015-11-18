#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int result;
	char chr_my;
	

	while(1){	
		result = read(STDIN_FILENO, &chr_my, 1);
		if (result == 1){
			write(STDOUT_FILENO, &chr_my, sizeof(&chr_my)-1);
			printf("\n");
		}
	}
}