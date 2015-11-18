#include <stdio.h>
#include <unistd.h>

int main(void)
{
	//FILE *fd = fdopen(OpenRandomGenerator(),"r");
	
	int i, k[20], result, randGen_desc;
	int count = 0;
	char randGen_char[20];
	char counter[20];
	
	int fd = OpenRandomGenerator();
	
	while(1){
	   result = read(fd, &randGen_char[i++], 1);
       if(result == 0) {
           break;
		}
       write(STDOUT_FILENO, &randGen_char[i-1], 1);		
	}
	
	sscanf(randGen_char, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n", &k[0],&k[1],
							&k[2],&k[3],&k[4],&k[5], &k[6]);
							
	/*
	i=0;
	while(1){
		result = fscanf(fd, "%d", &k[i++]);
		if(result == 1){
			write(STDOUT_FILENO, &k[i-1], 1);
		}
		else	
			break;
	}*/
	i=0;
	while(i != 7){
	//printf("Result: %d\n", k[i]);
		if(k[i++] < 20)
			count++;		
	}

	sprintf(counter, "%d", count);
	write(STDOUT_FILENO, "\nThe amount of numbers bellow 20 is ", 36);
	write(STDOUT_FILENO, &counter, 1);
	

	return 0;
}