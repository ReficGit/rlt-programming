#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

void print_flags(int fd);
void rdwr_term(int fd, int test);


int main(int argc, char *argv[])
{
	int fd, cur_fileflags;
	int RD = 1, RDWR = 2;
	char test1[] = "\n\n|-------------------------------|\nTEST 1\n";
	char test2[] = "\n\n|-------------------------------|\nTEST 2\n";
	char test3[] = "\n\n|-------------------------------|\nTEST 3\n";
	char test4[] = "\n\n|-------------------------------|\nTEST 4\n";
	
	write(STDOUT_FILENO, test1, sizeof(test1)-1);
	
	fd = open("/dev/tty", O_RDWR);			
	print_flags(fd);
	rdwr_term(fd, RDWR);
	close(fd);

	write(STDOUT_FILENO, test2, sizeof(test2)-1);
	
	fd = open("/dev/tty", O_RDONLY);			
	print_flags(fd);
	rdwr_term(fd, RDWR);
			
	write(STDOUT_FILENO, test3, sizeof(test3)-1);		
			
	cur_fileflags = fcntl(fd, F_GETFL, 0);
	cur_fileflags = cur_fileflags & ~O_NONBLOCK;	//set o_nonblock
	cur_fileflags = cur_fileflags | O_APPEND;		//set o_append
	fcntl(fd, F_SETFL, cur_fileflags);
	print_flags(fd);
	rdwr_term(fd, RD);
	
	write(STDOUT_FILENO, test4, sizeof(test4)-1);
			
	cur_fileflags = cur_fileflags | O_NONBLOCK;		//clear o_nonblock
	fcntl(fd, F_SETFL, cur_fileflags);
	print_flags(fd);
	rdwr_term(fd, RD);
	
}

void print_flags(int fd)
{
	int access_mode, file_flags;
	char out_rd[] = "Access mode: Read only";
	char out_wr[] = "Access mode: Write only";
	char out_rdwr[] = "Access mode: Read/Write";
	char blck_on[] = "O_NONBLOCK: ON ";
	char blck_off[] = "O_NONBLOCK: OFF ";
	char apnd_on[] = "O_APPEND: ON ";
	char apnd_off[] = "O_APPEND: OFF ";
	
	file_flags = fcntl(fd, F_GETFL, 0);
	access_mode = file_flags & O_ACCMODE;
	
	if(access_mode == O_RDONLY)
	{
		write(STDOUT_FILENO, out_rd, sizeof(out_rd)-1);		
	}
	else if (access_mode == O_WRONLY)
	{
		write(STDOUT_FILENO, out_wr, sizeof(out_wr)-1);
	}
	else {
		write(STDOUT_FILENO, out_rdwr, sizeof(out_rdwr)-1);
	}
	
	printf("\n");
	
	if(file_flags & O_NONBLOCK)
	{
		write(STDOUT_FILENO, blck_on, sizeof(blck_on)-1);
	}
	else
	{
		write(STDOUT_FILENO, blck_off, sizeof(blck_on)-1);
	}
	
	printf("\n");
	
	if(file_flags & O_APPEND)
	{
		write(STDOUT_FILENO, apnd_on, sizeof(apnd_on)-1);
	}
	else
	{
		write(STDOUT_FILENO, apnd_off, sizeof(apnd_off)-1);
	}
	
	printf("\n");
}

void rdwr_term(int fd, int test){	
	int term_stat, t_stat2;
	int size = 1;
	char read_ok[] = "Read successful, char was: ";
	char write_ok[] = "Write succeeded.";
	char new_line[] = "\n";
	char buff[size];
	char mchar[] = "\nA";
	char wr_error[] = "\nWrite error: ";
	
	
		//Read and Write
		if(test == 2){
			term_stat = read(fd, buff, size);
			if(term_stat == -1){
				write(STDOUT_FILENO, "\nRead error: ", 13);
				//printf("%s\n", strerror(errno));
				//printf("READ ERROR: %d\n", errno);	
				perror("WRITE ERROR: ");				
			} 
			else{
				write(STDOUT_FILENO, read_ok, sizeof(read_ok)-1);
				write(STDOUT_FILENO, buff, size);
			}
							
			t_stat2 = write(fd, mchar, sizeof(mchar)-1);
			
			if(t_stat2 == -1){
				write(STDOUT_FILENO, wr_error, sizeof(wr_error)-1);
				//printf(" %d\n", strerror(errno));
				//char *a;
				//a = strerror(errno);
				//printf("WRITE ERROR: %d\n", errno);
				perror("WRITE ERROR: ");
			}
			else{	
				write(STDOUT_FILENO, new_line, sizeof(new_line)-1);
				write(STDOUT_FILENO, write_ok, sizeof(write_ok)-1);
			}
		}
		
		//Read only
		else if(test == 1){
			term_stat = read(fd, buff, size);
			if(term_stat == -1){
				write(STDOUT_FILENO, "\nRead error: ", 13);
				//printf("%s\n", strerror(errno));
				//printf("READ ERROR: %d\n", errno);
				perror("");
			} 
			else{
				write(STDOUT_FILENO, read_ok, sizeof(read_ok)-1);
				write(STDOUT_FILENO, buff, size);
			}
		}
}
