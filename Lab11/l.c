#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <math.h>

#define N 10


int fd[2];
volatile int sig_rec;
unsigned long rec[N];
struct timespec cur[N];
struct timespec rcv[N];

void signal_handler(int signo, siginfo_t *siginfo, void *s){
    char t[21];
    int readed = 0;
    clock_gettime(CLOCK_REALTIME, &(cur[sig_rec]));
    //while (read(fd[0], t, 20) <= 0) {}
    //sscanf(t, "%lu %lu*", &(rcv[sig_rec].tv_sec), &(rcv[sig_rec].tv_nsec));
    sig_rec++;
}

int main()
{
    int pid;
    char t[20];
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    srand(now.tv_nsec);
    printf("Real time clock resoulution is: 0s 1ns:\n  Current "
           "time is sec: %lu, nsec: %lu\n", now.tv_sec, now.tv_nsec);
    
    if (pipe(fd) != 0) {
        printf("Could not create pipe\n");
        return -1;
    }
    sig_rec = 0;
    
    pid = fork();
    /* child */
    if (pid == 0) {
        struct timespec child_time;
        struct sigaction act;
        float sum_dev = 0;
        float mean = 0;
        unsigned long s = 0;
        unsigned long min = 429496725;
        unsigned long max = 0;
        unsigned long dif = 0;
        int i;
        sigset_t mask, wait_mask;
        
        act.sa_sigaction = signal_handler;
        act.sa_flags = SA_SIGINFO;
        sigemptyset(&act.sa_mask);
        sigaction(SIGRTMAX, &act, NULL);
        
        sigemptyset(&mask);
//        sigaddset(&mask, SIGRTMAX);
  //      sigprocmask(SIG_BLOCK, &mask, NULL);
    //    sigfillset(&wait_mask);
      //  sigdelset(&wait_mask, SIGRTMAX);
      sigsuspend(&mask); 
        while (sig_rec != N) {
            //sigsuspend(&wait_mask);
        }
        printf("Received: %d signals\n", sig_rec);
        for(i = 0; i < N; i++) {
            dif = (cur[i].tv_sec - rcv[i].tv_sec)*1000000000 +
            cur[i].tv_nsec - rcv[i].tv_nsec;
            if (dif < min)
                min = dif;
            if (dif > max)
                max = dif;
            s = s + dif;
            rec[i] = dif;
        }
        mean = s/N;
        printf("Mean %f us\n", mean/1000);
        printf("Min %lu us\n", min/1000);
        printf("Max %lu us\n", max/1000);
        for(i = 0; i < N; i++) {
            sum_dev += (rec[i]-mean)*(rec[i]-mean);
        }
        printf("Standard deviation: %f us\n",sqrt(sum_dev/N)/1000);
    }
    /* parent */
    if (pid > 0) {
        struct timespec parent_time;
        union sigval value;
        int sleep_period;
        int i;
        struct timespec nano_sleep;
        
        nano_sleep.tv_sec = 0;
        nano_sleep.tv_nsec;
        value.sival_int = 0;
        sleep(1);
        for(i = 0; i < N; i++) {
            /* get time */
            clock_gettime(CLOCK_REALTIME, &parent_time);
            /* generate signal */
            sigqueue(pid, SIGRTMAX, value);
            /* send time to the pipe */
            sprintf(t, "%lu %lu", parent_time.tv_sec, parent_time.tv_nsec);
            write(fd[1], t, strlen(t));
            /*  generate random pause time */
            nano_sleep.tv_nsec = (rand()%32 + 1)*1000000;
            nanosleep(&nano_sleep, NULL);
        }
        wait (NULL);
    }
    return 0;
}


