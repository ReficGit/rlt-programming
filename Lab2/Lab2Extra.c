#include <stdio.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
	time_t t = time(NULL);
	struct tm t_tm = *localtime(&t);

	printf("Local time is: %d.%d.%d %d:%d:%d\n",  t_tm.tm_mday, t_tm.tm_mon + 1, t_tm.tm_year + 1900, t_tm.tm_hour, t_tm.tm_min, t_tm.tm_sec);
	
	
}
