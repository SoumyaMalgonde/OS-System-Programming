/*
Author : Soumya Malgonde
Aim : To implement Dining Philosopher’s problem using ‘C’ in Linux
	Implement the deadlock-free solution to Dining Philosophers problem to illustrate the problem of deadlock and/or starvation that can occur when many synchronized threads are competing for limited resources.

(Compilation using: gcc filename.c -lpthread)
*/

//chair solution is used i.e., at a time at max n-1 philosophers can pick left chopstick simultaneously 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

void *philosopher_on_table(void *arg);
void eat();
pthread_mutex_t mutex;
int phil_done = 0, n;
sem_t chopstick[10], max_philosopher_at_once;

int main() 
{
	srand(time(0));
	int res;
	pthread_t philosopher[10];
	void *thread_result;
	int i, j, k;
	printf("Enter the number of philosophers on round table(max seating capacity is 10) : \n");
	scanf("%d", &n);
	if(n > 10)
		n = 10;
	if(n == 1)
	{
		printf("The only philosopher cannot eat using only 1 chopstick!\n");
		return 0;
	}
	// initializing the semaphores
	sem_init(&max_philosopher_at_once, 0, n-1);
	for(i=0 ; i<n ; i++)
		sem_init(&chopstick[i], 0, 1);
	res = pthread_mutex_init(&mutex, NULL);
	if (res != 0) 
	{
		perror("Mutex initialization failed");
		exit(EXIT_FAILURE);
	}
	
	// creating philosophers threads
	for(i=0 ; i<n ; i++)
	{
		res = pthread_create(&philosopher[i], NULL, philosopher_on_table, &i);
		if (res != 0) 
		{
			perror("Thread creation failed");
			exit(EXIT_FAILURE);
		}
		sleep(0.1);
	}

	// waiting for all threads to finish respective jobs
	while(phil_done != n)
		sleep(0.2);

	// joining philosopher threads
	for(i=0 ; i<n ; i++)
	{
		res = pthread_join(philosopher[i], &thread_result);
		if (res != 0) 
		{
			perror("Thread join failed");
			exit(EXIT_FAILURE);
		}
	}

	printf("All philosophers went back to lab.\n");

	sem_destroy(&max_philosopher_at_once);
	exit(EXIT_SUCCESS);
}

// function to just eat for sometime
void eat()
{
	sleep(5);
}

// philosopher's thread function
void *philosopher_on_table(void *arg)
{
	int i = *(int *)arg;
	printf("Philosopher %d is thinking.\n", i);
	// sleep(rand()%10); // to ensure randomized requests for accessing chopsticks
	sem_wait(&max_philosopher_at_once);
	printf("Philosopher %d is hungry now.\n", i);

	sem_wait(&chopstick[i]); // waiting for left chopstick to be free
	sem_wait(&chopstick[(i+1)%n]); // waiting for right chopstick to be free
	// philosopher i got both chopsticks
	printf("Philosopher %d is eating dinner now.\n", i);
	eat();
	
	sem_post(&chopstick[(i+1)%n]); // releasing right chopstick
	printf("Philosopher %d is releasing right chopstick i.e. chopstick %d.\n", i, (i+1)%n);
	sleep(5);	
	sem_post(&chopstick[i]); // releasing left chopstick
	printf("Philosopher %d is releasing left chopstick i.e. chopstick %d.\n", i, i);

	// mutex lock to ensure only one reader is changing phildone at any point of time
	pthread_mutex_lock(&mutex);
		phil_done++;
	pthread_mutex_unlock(&mutex);
	sem_post(&max_philosopher_at_once);
	pthread_exit(NULL);
}
