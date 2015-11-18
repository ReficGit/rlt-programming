#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <aio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include "MultiSensorSimulator.h"

#define n_sensors 5
#define n_values 5
#define optionA 1

void time_init(struct timespec* t);

int main(void)
{
	Tmeas measurement[n_sensors];
	struct timespec m_time, diff_time, sum_time;
	struct aiocb **aiocb;
	fd_set fdset;
	int sensorDescriptors[n_sensors], count, n, i, k;
	int fdset_sum = 0;
	int done = 0;	
	
	
	time_init(&m_time);
	time_init(&diff_time);
	time_init(&sum_time);
	count = 0;

	StartSimulator(sensorDescriptors, n_values);
#if optionA
	while(done != n_sensors){
			FD_ZERO(&fdset);
			for(i=0;i<n_sensors;i++){
			//	if((fcntl(sensorDescriptors[i], F_GETFL) > -1)){ //if fd is open - add
					FD_SET(sensorDescriptors[i], &fdset);
					fdset_sum = (fdset_sum>sensorDescriptors[i])?fdset_sum:sensorDescriptors[i];
			//	}
			}
		n = select(fdset_sum+1, &fdset, NULL, NULL, NULL);
		if (n > 0) {
			for(i=0; i<(n_sensors); i++){
				if(FD_ISSET(sensorDescriptors[i], &fdset)){
					if((read(sensorDescriptors[i], &measurement[i], sizeof(Tmeas))) > 0){
						clock_gettime(CLOCK_REALTIME, &m_time); //get real time
						diff_time = diff_timespec(&measurement[i].moment, &m_time);//show time diff (real-meas)
						increment_timespec(&sum_time, &diff_time);//add time diffs together
						printf("\n\t\t------NEW VALUE-----\n");
						printf("\nMeasured time is: \t %lld.%.9ld sec\nCurrent time is:\t %lld.%.9ld sec\n",measurement[i].moment.tv_sec, measurement[i].moment.tv_nsec, m_time.tv_sec,m_time.tv_nsec);
						printf("Measurement value was %d with time difference of\n\t\t\t\t  %lld.%.9ld sec\n", measurement[i].value, diff_time.tv_sec, diff_time.tv_nsec);
						printf("Sum of all delays is:\t\t  %lld.%.9ld sec\n", sum_time.tv_sec, sum_time.tv_nsec);
						count = count + 1; //count number of values
					}
					else if((read(sensorDescriptors[i], &measurement, sizeof(Tmeas))) == 0) {
						done++; //count fd closed
						printf("%d sensor(s) already closed\n", done);
						if((close(sensorDescriptors[i]) == -1)){
							perror("Closing fd error");
						}
						//for(k=i; k<(n_sensors-done);k++){
							//erase deleted element to avoid unnecessary loop
						//	sensorDescriptors[k] = sensorDescriptors[k+1];
 						//}
					}
					else {
						perror("Read problem");
					}
				} //fd_isset if
			} //for loop			
		}// if (n > 0)		
	} // outer while
#else
	//fil i/o control block
	//and initiate
	assert ((aiocb = (struct aiocb **) malloc (sizeof(struct aiocb *) * n_sensors)) != NULL);
	for(i = 0; i<n_sensors; i++){
		assert((aiocb[i] = (struct aiocb *) malloc (sizeof(struct aiocb))) != NULL);
		aiocb[i]->aio_fildes = sensorDescriptors[i];
		aiocb[i]->aio_buf = &measurement[i];
		aiocb[i]->aio_nbytes = sizeof(Tmeas)+1;
		aiocb[i]->aio_offset = 0;
		aiocb[i]->aio_reqprio = 0;
		aiocb[i]->aio_sigevent.sigev_notify = SIGEV_NONE;
		aiocb[i]->aio_lio_opcode = LIO_READ;
	}
	      /*  aiocb.aio_fildes = sensorDescriptors[1];
		aiocb.aio_buf = &measurement;
		aiocb.aio_nbytes = sizeof(Tmeas);
		aiocb.aio_offset = 0;
		aiocb.aio_reqprio = 0;
		aiocb.aio_sigevent.sigev_notify = SIGEV_NONE;
		aiocb.aio_lio_opcode = 0;
		//aio_read(&aiocb);*/
		assert(lio_listio(LIO_NOWAIT, aiocb, n_sensors, &aiocb[0]->aio_sigevent) == 0);
		count = 0;
	while (aio_suspend((const struct aiocb **) aiocb, n_sensors, NULL) == 0){
		int nreqs = 0; //number of requests
		for(i = 0; i<n_sensors; i++){
			
			if(aiocb[i] == NULL) {
				continue;
			}
			nreqs++;

			 if (aio_error (aiocb[i]) == EINPROGRESS) {
			    	continue;
			 }
			 {
			//number of read bytes
			ssize_t nb; 
			
			
			if((nb = aio_return(aiocb[i])) <= 0){
				//error or end of file
			(void) close(aiocb[i]->aio_fildes);
			nreqs--;
			}
			else { //process data
				//count++;
				//printf("VALUE %d from %d and count %d\n", measurement[i].value, i, count);
				clock_gettime(CLOCK_REALTIME, &m_time); //get real time
				diff_time = diff_timespec(&measurement[i].moment, &m_time);//show time diff (real-meas)
				increment_timespec(&sum_time, &diff_time);//add time diffs together
				printf("\n\t\t------NEW VALUE-----\n");
				printf("\nMeasured time is: \t %lld.%.9ld sec\nCurrent time is:\t %lld.%.9ld sec\n",measurement[i].moment.tv_sec, measurement[i].moment.tv_nsec, m_time.tv_sec,m_time.tv_nsec);
				printf("Measurement value was %d with time difference of\n\t\t\t\t  %lld.%.9ld sec\n", measurement[i].value, diff_time.tv_sec, diff_time.tv_nsec);
				printf("Sum of all delays is:\t\t  %lld.%.9ld sec\n", sum_time.tv_sec, sum_time.tv_nsec);
				count = count + 1; //count number of values
			}
			//new request
			aiocb[i]->aio_offset += nb;
			(void) aio_read (aiocb[i]);
			}
		}//if	
		if(nreqs == 0){
			break;
		}
		
	}
#endif
	printf("Total values %d\n", count);
	printf("Sum of all delays is:\t\t  %lld.%.9ld sec\n", sum_time.tv_sec, sum_time.tv_nsec);
	return 0;

}

void time_init(struct timespec* t)
{
	t->tv_sec = 0;
	t->tv_nsec = 0;
}
