#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

//int change_descriptor(int file_descriptor);

int main(int argc, char *argv[]) {
	int fellow_desc;
	int my_desc;
	int file_flags;
	int result;
	char chr_fellow;
	char chr_my;

	fellow_desc = OpenChatFellow();	
	fellow_desc = change_descriptor(fellow_desc);
	
	my_desc = STDIN_FILENO;
	my_desc = change_descriptor(my_desc);

	while(1){	
		result = read(fellow_desc, &chr_fellow, 1);
		if (result == 1){
			write(STDOUT_FILENO, &chr_fellow, sizeof(&chr_fellow)-1);
			printf("\n");
		}
		result = read(my_desc, &chr_my, 1);
		if (result == 1){
			write(STDOUT_FILENO, &chr_my, sizeof(&chr_my)-1);
			printf("\n");
		}
	}
	close(fellow_desc);
}

int change_descriptor(int file_descriptor)
{
	int file_flags;
	file_flags = fcntl(file_descriptor, F_GETFL); //read current file flags
	file_flags = file_flags | O_NONBLOCK; // Add O_NONBLOCK bit
	fcntl(file_descriptor, F_SETFL, file_flags);
	return(file_descriptor);
}