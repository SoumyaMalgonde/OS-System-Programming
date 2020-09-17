/*
Author : Soumya Malgonde
Aim : Thread synchronization and mutual exclusion using mutex.
	Implement C program to demonstrate Reader-Writer problem with readers having priority using counting semaphores and mutex.

(Compilation using: gcc filename.c -lpthread)
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

void *reader(void *arg);
void *writer(void *arg);
void printshared();
pthread_mutex_t mutex;
sem_t wrt;
int totalreaders = 6, totalwriters = 5, readercount, readersdone, writersdone;
int shared_data[5] = {1, 2, 3, 4, 5}; // this array is the shared data

int main() 
{
	srand(time(0));
	int res;
	pthread_t r_thread[totalreaders], w_thread[totalwriters];
	void *thread_result;
	int i, j, k;
	int maxnum = totalreaders;
	if(totalwriters > maxnum)
		maxnum = totalwriters;
	int index[maxnum];
	for(i=0 ; i<maxnum ; i++)
		index[i] = i;

	res = pthread_mutex_init(&mutex, NULL);
	if (res != 0) 
	{
		perror("Mutex initialization failed");
		exit(EXIT_FAILURE);
	}
	sem_init(&wrt, 0, 1);
	printshared();
	printf("\n");
	
	// creating reader threads
	for(i=0 ; i<totalreaders ; i++)
	{
		res = pthread_create(&r_thread[i], NULL, reader, &index[i]);
		if (res != 0) 
		{
			perror("Thread creation failed");
			exit(EXIT_FAILURE);
		}
		sleep(0.5);
	}

	// creating writer threads
	for(i=0 ; i<totalwriters ; i++)
	{
		res = pthread_create(&w_thread[i], NULL, writer, &index[i]);
		if (res != 0) 
		{
			perror("Thread creation failed");
			exit(EXIT_FAILURE);
		}
		sleep(0.5);
	}

	//waiting for all threads to finish respective jobs
	while(readersdone < totalreaders && writersdone < totalwriters);
	sleep(2);

	// joining reader threads
	for(i=0 ; i<totalreaders ; i++)
	{
		res = pthread_join(r_thread[i], &thread_result);
		if (res != 0) 
		{
			perror("Thread join failed");
			exit(EXIT_FAILURE);
		}
	}

	// joining writer threads
	for(i=0 ; i<totalwriters ; i++)
	{
		res = pthread_join(w_thread[i], &thread_result);
		if (res != 0) 
		{
			perror("Thread join failed");
			exit(EXIT_FAILURE);
		}
	}
	printf("Threads joined.\n");
	pthread_mutex_destroy(&mutex);
	sem_destroy(&wrt);
	exit(EXIT_SUCCESS);
}

// function to print shared array
void printshared()
{
	printf("The shared data now is : ");
	for(int i=0 ; i<5 ; i++)
		printf("%d ", shared_data[i]);
	printf("\n");
}

// reader's thread function
void *reader(void *arg)
{
	int readernum = *(int *)arg;
	sleep(rand()%6); // to ensure randomized requests for access

	// mutex lock to ensure only one reader is changing readercount at any point of time
	pthread_mutex_lock(&mutex);
		readercount++;
		if(readercount == 1) // first reader blocks all writers
			sem_wait(&wrt);
	pthread_mutex_unlock(&mutex);

	printf("+ Reader %d has entered and is reading data.\n", readernum);
	printf("Readers count is : %d\n", readercount);
	printshared();
	printf("\n");
	sleep(rand()%5); // to let other readers enter at the same time

	// mutex lock to ensure only one reader is changing readercount at any point of time
	pthread_mutex_lock(&mutex);
		readercount--;
		printf("- Reader %d is leaving.\n", readernum);
		printf("Readers count is : %d\n\n", readercount);
		if(readercount == 0) // last reader unblocks all writers
			sem_post(&wrt);
		readersdone++;
	pthread_mutex_unlock(&mutex);
}

// writer's thread function
void *writer(void *arg)
{
	int writernum = *(int *)arg;
	sleep(rand()%6); // to ensure randomized requests for access

	sem_wait(&wrt); // any writer blocks all other writers and all readers using wrt semaphore
		int ind = rand()%5;
		printf("+ Writer %d is writing at index %d.\n", writernum, ind);
		shared_data[ind] = rand()%100;
		printshared();
		printf("\n");
		printf("- Writer %d is leaving.\n\n", writernum);
		writersdone++;
	sem_post(&wrt);
}
