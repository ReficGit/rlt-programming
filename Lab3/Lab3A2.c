#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void write_to_file(const char *name);

int main(void)
{
	const char file_name[] = "exlog.txt";
	
	write_to_file(file_name);
}

void write_to_file(const char *name)
{
	int fd;
	int i = 0;
	char buff[4] = "BBBB";
	fd = open(name, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
	for(i = 0; i<100000; i++){
		//lseek(fd, 0L, SEEK_END);
		write(fd, buff, 4);
	}
}
