#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>

void *pthServeClient(void *socket){
	char buff[4];
	char chr;
	int result = 0;
	int sec_interval = 0;
	int i = 0;

	result = read(*(int*)socket, &buff, 4);
	if(result < 0){
		perror("pthread read error");
		exit(1);
	}//if result
	else{
		sscanf(buff, "%s %d", &chr, &sec_interval);
		for(i = 0; i<10; i++){
			sleep(sec_interval);
			result = write(*(int*)socket, &chr, 1);
			if(result < 0){
				perror("Write error");
			}//inner if
			
		}//for	
		close(*(int*)socket);	
	}//else
}//func

int main(void)
{
	int srv_socket, client_socket[4];
	struct sockaddr_un srv_addr;
	char buff[8], chr;
	int sec_interval, i, result;
	int wr_error = 0, childs = 1;
	pid_t c1, c2;
	int N = 0;
	pthread_t threadID;
	unlink("server.sock");

	//create socket
	srv_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	if (srv_socket < 0){
		perror("Error opening socket");
		exit(1);
	}

	//init the address structure
	srv_addr.sun_family = AF_UNIX;
	strcpy(srv_addr.sun_path, "server.sock");
	//bind address to this process
	if((bind(srv_socket, (struct sockaddr*)&srv_addr, sizeof(srv_addr))) < 0){
		perror("Error binding");
		exit(1);	
	}
	printf("Bind success\n");
	//start listening
	listen(srv_socket, 5);

	for(;;){
		//accept connection requests
		client_socket[N] = accept(srv_socket, NULL, 0);
		if(client_socket[N] > 0){
		write(STDOUT_FILENO, "\nClient connected\n", 18);
		//c1 = fork();
		if(pthread_create(&threadID, NULL, pthServeClient, &client_socket[N]) != 0){
			perror("pthread_create");
			exit(1);
		}
		N++;
		if(pthread_detach(threadID) != 0){
			perror("pthread_detach");
			exit(1);
		}
//		if(c1 == 0) { //child 1
//			c2 = fork();
//			if(c2 > 0) //child 1 terminates
//				exit(0);
//			//serve client
//				result = read(client_socket, &buff, 4);
//				if(result < 0){
//					perror("Server read error");
//				}
//				else{
//					sscanf(buff, "%s %d", &chr, &sec_interval);
//					for(i = 0; i<10; i++){
//						sleep(sec_interval);
//						result = write(client_socket, &chr, 1);
//						if(result < 0){
//							perror("Write error");
//							wr_error++;
//							if(wr_error > 3){ // exit if write
//									  // fails 3 times
//								exit(1);
//							} // inner if
//						} // if res < 0
//					}//for loop
//				//shutdown(client_socket, 2);
//				close(srv_socket);
//				close(client_socket);
//				}//else
//
//			write(STDOUT_FILENO, "\nCompleted", 10);
//			exit(0);
//		}//if c1=0
		}
//	close(client_socket);
	wait(NULL);

	}//outer for

	close(srv_socket);
	exit(0);
}//main
