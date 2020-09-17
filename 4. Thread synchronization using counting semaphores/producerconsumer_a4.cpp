/*
Author : Soumya Malgonde
Aim : Producer-consumer problem solution with counting semaphores
and mutex.

(Compilation using: g++ filename.cpp -lpthread)
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <queue>

using namespace std;

void *thread_function(void *arg);
void *producer(void *arg);
void *consumer(void *arg);
void printqueue();
pthread_mutex_t mutex;
sem_t full, empty;
int maxp, maxc, canexitp=0, canexitc=0;
queue <int> q;

int main() 
{
	srand(time(0));
	int res;
	pthread_t a_thread, b_thread; //a_thread is producer thread, and b_thread is consumer thread
	void *thread_result;
	int i, j, k;
	maxc = 5;
	maxp = rand()%5 + maxc;

	res = pthread_mutex_init(&mutex, NULL);
	if (res != 0) 
	{
		perror("Mutex initialization failed");
		exit(EXIT_FAILURE);
	}
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, maxp);

	res = pthread_create(&a_thread, NULL, producer, NULL);
	if (res != 0) 
	{
		perror("Thread creation failed");
		exit(EXIT_FAILURE);
	}
	res = pthread_create(&b_thread, NULL, consumer, NULL);
	if (res != 0) 
	{
		perror("Thread creation failed");
		exit(EXIT_FAILURE);
	}
	while(!canexitp || !canexitc)
	{
		pthread_mutex_unlock(&mutex);
		sleep(1);
		pthread_mutex_lock(&mutex);
	}
	printf("Waiting for threads to finish.\n");
	res = pthread_join(a_thread, &thread_result);
	if (res != 0) 
	{
		perror("Thread join failed");
		exit(EXIT_FAILURE);
	}
	res = pthread_join(b_thread, &thread_result);
	if (res != 0) 
	{
		perror("Thread join failed");
		exit(EXIT_FAILURE);
	}
	printf("Threads joined.\n");
	pthread_mutex_destroy(&mutex);
	sem_destroy(&full);
	sem_destroy(&empty);
	exit(EXIT_SUCCESS);
}
//producer thread function
void *producer(void *arg)
{

	while(maxp--)
	{
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);

		int item = rand()%100 + 1; //item produced
		printf("Producer has produced item %d.\n",  item);
		q.push(item);
		printf("Available items now are : ");
		printqueue();
		printf("\n");

		pthread_mutex_unlock(&mutex);
		sleep(rand()%3);
		sem_post(&full);
	}
	canexitp = 1;
	pthread_exit(NULL);
}
//consumer thread function
void *consumer(void *arg)
{
	while(maxc--)
	{
		sem_wait(&full);
		pthread_mutex_lock(&mutex);

		int item =  q.front(); // item consumed
		q.pop();
		printf("Consumer has consumed item %d.\n", item);
		printf("Available items now are : ");
		printqueue();
		printf("\n");
		
		pthread_mutex_unlock(&mutex);
		sleep(rand()%6);
		sem_post(&empty);
	}
	canexitc = 1;
	pthread_exit(NULL);
}
// function to print queue of items
void printqueue()
{
	queue <int> temp = q;
	while(!temp.empty())
	{
		int itematfront = temp.front();
		temp.pop();
		printf("%d, ", itematfront);
	}
	printf("\n");
}
