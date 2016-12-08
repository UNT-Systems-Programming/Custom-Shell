/*
 * minor5.c - using producer-consumer paradigm.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NITEMS 10		// number of items in shared buffer

// shared variables
char shared_buffer[NITEMS];	// echo buffer
int shared_count;		// item count

pthread_mutex_t mutex;		// pthread mutex
unsigned int prod_index = 0; 	// producer index into shared buffer
unsigned int cons_index = 0; 	// consumer index into shard buffer

// function prototypes
void * producer(void *arg);
void * consumer(void *arg);

int main() 
{ 
	pthread_t prod_tid, cons_tid1, cons_tid2; 

	// initialize pthread variables
	pthread_mutex_init(&mutex, NULL);
	
	// start producer thread
	pthread_create(&prod_tid, NULL, producer, NULL);

	// start consumer threads
	pthread_create(&cons_tid1, NULL, consumer, NULL);
	pthread_create(&cons_tid2, NULL, consumer, NULL);
	
	// wait for threads to finish
	pthread_join(prod_tid, NULL);
	pthread_join(cons_tid1, NULL);
	pthread_join(cons_tid2, NULL);
			
	// clean up
	pthread_mutex_destroy(&mutex);
	
	return 0;
}

// producer thread executes this function
void * producer(void *arg)
{
	char key;
	long double usage;
	double load[1];
	int cpu_load;
    FILE *fp;
	while (1)
	{
		fp = fopen("/proc/loadavg","r");
		if(fp == NULL){		// File open error
			perror("Error");
		}
		else{
			fscanf(fp,"%Lf",&usage);
			fclose(fp);
			printf("The current CPU utilization is : %Lf\n",usage);
			cpu_load = (int)(usage * 100);
			//return cpu_load;
			printf("The percent of usage is: %d\n",cpu_load);
		}
		/* 
		getloadavg(load,1);
		printf("load average : %f\n",load[0]); 
		*/
			
		// read input key
		scanf("%c", &key);

		while (1)
		{
			// acquire mutex lock
			pthread_mutex_lock(&mutex);
			
			// if buffer is full, release mutex lock and check again
			if (shared_count == NITEMS)
				pthread_mutex_unlock(&mutex);
			else
				break;
		}
		
		// store key in shared buffer
		shared_buffer[prod_index] = key;

		// update shared count variable
		shared_count++;

		// update producer index
		if (prod_index == NITEMS - 1)
			prod_index = 0;
		else
			prod_index++;
		
		// release mutex lock
		pthread_mutex_unlock(&mutex); 
	}

	return NULL;
}

// consumer thread executes this function
void * consumer(void *arg)
{
	char key;

	int id = (int)pthread_self();

	while (1)
	{
		while (1)
		{
			// acquire mutex lock
			pthread_mutex_lock(&mutex);

			// if buffer is empty, release mutex lock and check again
			if (shared_count == 0)
				pthread_mutex_unlock(&mutex);
			else
				break;
		}

		// read key from shared buffer
		key = shared_buffer[cons_index];
		
		// echo key
		printf("consumer %d %c\n", id, key);

		// update shared count variable
		shared_count--;

		// update consumer index
		if (cons_index == NITEMS - 1)
			cons_index = 0;
		else
			cons_index++;
	
		// release mutex lock
		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}
