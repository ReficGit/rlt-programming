#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	int fellow_desc;
	int result;
	char chr_fellow;

	fellow_desc = OpenChatFellow();
	
	while(1){	
		result = read(fellow_desc, &chr_fellow, 1);
		if (result == 1){
			write(STDOUT_FILENO, &chr_fellow, sizeof(&chr_fellow)-1);
			printf("\n");
		}
	}
	close(fellow_desc);
}