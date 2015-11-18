#include <signal.h> //signal
#include <stdio.h> //printf, perror
#include <unistd.h> //_exit, fork
#include <sys/types.h> //pid_t
#include <fcntl.h> //file open flags
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Number of children
#define N 5

// Global variable that indicates how many children have terminated
int n = 0;

// Write signal handler for signal SIGCHLD here.
//static void sig_handler(int sig_no, siginfo_t *si, void *context);
static void sig_handler(int sig_no);
// Signal handler increments n.

int main(void)
{
	pid_t pid;
	int fd, result, i = 0;
	char m_fd[1];
	char m_char[5][5];
	char out;
	sigset_t newmask, oldmask, suspmask, emptyMask;
	int signo;
	struct sigaction c_sig_new, c_sig_old;
	//struct timespec timeout;
	
	strcpy(m_char[0], "AAAA");
	strcpy(m_char[1], "BBBB");
	strcpy(m_char[2], "CCCC");
	strcpy(m_char[3], "DDDD");
	strcpy(m_char[4], "EEEE");
    // Install signal handler  
	//signal(SIGCHLD, sig_handler);
	
	//set handler and save prev info
	c_sig_new.sa_handler = sig_handler;
	sigemptyset(&c_sig_new.sa_mask);
	c_sig_new.sa_flags = 0;
	sigaction(SIGCHLD, &c_sig_new, &c_sig_old);
	
	//block sigchld and save cur sig mask
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGCHLD);
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);
	
    // Open the file data.txt
	fd = open("data.txt", O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR);
	
    
    // Create N children that start to run a separate 
    // executable file child.exe. Pass the file descriptor,
    // the number of writes and the string that the child
    // program writes to the file.
	
	for(i = 0; i<N; i++){
		pid = fork();
		if(fork < 0){ //error
			perror("Fork:");
			exit(1);
		}
		else if(pid == 0){ //child		
			sprintf(m_fd, "%d", fd);
			//sleep(i+1);
			sprintf(m_char[5], "%d", 1);
			execl("./child.exe", "./child.exe", m_fd, m_char[i], m_char[5], NULL);	
			perror("exec");
			exit(0);
		}
		//parent
	}
    // Parent process waits for the children in the following loop.
    // Signal causes pause to return


    while (n < N) {	
		sigsuspend(&emptyMask);
		while(waitpid(-1, NULL, WNOHANG) > 0){
			printf("\nChild term %d", n);
			n++;	
		}    
		printf("\nChild term %d", n);
    }
    // Parent process displays the contents from the file
    
	
	sigaction(SIGCHLD, &c_sig_old, NULL);
	//reset signal
	sigprocmask(SIG_SETMASK, &oldmask, NULL);
    
	(fd, 0L, 0);
	while(result=(read(fd, &out, 1)) != 0){
		write(STDOUT_FILENO, &out, 1);
	}	
		
    return 0;
}

//static void sig_handler(int sig_no, siginfo_t *si, void *context){
static void sig_handler(int sig_no){
	n += 1;
	while(waitpid(-1, NULL, WNOHANG) > 0){
		return;
	}
}
