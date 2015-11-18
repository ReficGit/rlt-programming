#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

// struct of FILE http://stackoverflow.com/questions/16424349/where-to-find-struct-io-file

#define s_IO_UNBUFFERED 2
#define s_IO_LINE_BUF 0x200

int main(void)
{
	FILE *fd;
	long fSize;
	char *buffer;
	size_t result;
	//struct _IO_FILE *oFile;
	
	fd = fopen("textfile.txt", "r+");
	if(fd == NULL)
	{
		perror("Error");
		exit(1);
	}	
	// get file size
	fseek(fd, 0, SEEK_END);
	fSize = ftell(fd);
	// set the pos indicator to the beg. of the file
	rewind(fd);
		
	buffer = (char *)malloc(sizeof(char)*fSize);
	if(buffer == NULL){
			perror("Buffer error");
			exit(2);
		}
	result = fread(buffer, 1, fSize, fd);
	if(result != fSize){
		perror("Reading error:");
		exit(3);
	}	
	
	//fd = stdout;
	
	printf("File descriptor: %d\n", fd->_fileno);
	printf("File size: %d bytes\n", fd->_IO_read_end - fd->_IO_read_base);
	write(STDOUT_FILENO, "...\n", 4);
	printf("The size of the buffer: %d bytes\n", fd->_IO_buf_end - fd->_IO_buf_base);
	
	int filD = fileno(fd);
	int cur_fileflags = fcntl(filD, F_GETFL, 0);
	
	if(cur_fileflags & s_IO_LINE_BUF)
		printf("Line buffered\n");
	else if (cur_fileflags & s_IO_UNBUFFERED)
		printf("Unbufferd\n");
	else 
		printf("Fully buffered\n");
		
	fclose(fd);
	free(buffer);
	return 0;	
}