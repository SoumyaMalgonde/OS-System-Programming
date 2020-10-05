/*
Author : Soumya Malgonde
Aim : To implement Dining Philosopher’s problem using ‘C’ in Linux
	Implement the deadlock-free solution to Dining Philosophers problem to illustrate the problem of deadlock and/or starvation that can occur when many synchronized threads are competing for limited resources.

(Compilation using: gcc filename.c -lpthread)
*/

// In this solution all odd philosophers are picking their left chopstick followed by their right chopstick whereas all even philosophers are picking their right chopstick followed by their left chopstick.
// Additionally there are alternating time slots for even and odd of 10 seconds each. Even philosophers can only eat in slots for even group and likewise for odd. (basically time 1-10 is for even group then time 11-20 is for odd group then again time 21-30 for even group and so on).
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

void *philosopher_on_odd_table(void *arg);
void *philosopher_on_even_table(void *arg);
void eat();
pthread_mutex_t mutex;
int phil_done = 0, n;
sem_t chopstick[10];
int timeslotchance = 0; // value 0 is for even group and value 1 is for odd group

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
		if(i%2)
			res = pthread_create(&philosopher[i], NULL, philosopher_on_odd_table, &i);
		else
			res = pthread_create(&philosopher[i], NULL, philosopher_on_even_table, &i);
		if (res != 0) 
		{
			perror("Thread creation failed");
			exit(EXIT_FAILURE);
		}
		sleep(0.2);
	}

	//timer for alternate slots for even and odds until every philosopher has eaten
	while(phil_done != n) //each one gets 10 sec
	{
		int temp = timeslotchance;
		sleep(5);
		timeslotchance = 2;//2 is unvalid state where no new philosopher can start eating but previously started can continue and finish
		sleep(5);
		timeslotchance = 1-temp;
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

	exit(EXIT_SUCCESS);
}

// function to just eat for sometime
void eat()
{
	sleep(5);
}

// odd philosopher's thread function
void *philosopher_on_odd_table(void *arg)
{
	int i = *(int *)arg;
	printf("Philosopher %d is thinking.\n", i);
	sleep(rand()%10); // to ensure randomized requests for accessing chopsticks
	printf("Philosopher %d is hungry now.\n", i);

	while(timeslotchance != 1) //i can eat if slotchance is 1
		sleep(2);

	sem_wait(&chopstick[i]); // waiting for left chopstick to be free
	sem_wait(&chopstick[(i+1)%n]); // waiting for right chopstick to be free
	// philosopher i got both chopsticks

	printf("Philosopher %d is eating dinner now. slot %d\n", i, timeslotchance);
	eat();
	
	sem_post(&chopstick[(i+1)%n]); // releasing right chopstick
	printf("Philosopher %d is releasing right chopstick i.e. chopstick %d.\n", i, (i+1)%n);
	// sleep(5);	
	sem_post(&chopstick[i]); // releasing left chopstick
	printf("Philosopher %d is releasing left chopstick i.e. chopstick %d.\n", i, i);

	// mutex lock to ensure only one reader is changing phildone at any point of time
	pthread_mutex_lock(&mutex);
		phil_done++;
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}
// odd philosopher's thread function
void *philosopher_on_even_table(void *arg)
{
	int i = *(int *)arg;
	printf("Philosopher %d is thinking.\n", i);
	sleep(rand()%10); // to ensure randomized requests for accessing chopsticks
	printf("Philosopher %d is hungry now.\n", i);

	while(timeslotchance != 0) //i can eat if slotchance is 0
		sleep(2);

	sem_wait(&chopstick[(i+1)%n]); // waiting for right chopstick to be free
	sem_wait(&chopstick[i]); // waiting for left chopstick to be free
	// philosopher i got both chopsticks

	printf("Philosopher %d is eating dinner now. slot %d\n", i, timeslotchance);
	eat();
	
	sem_post(&chopstick[i]); // releasing left chopstick
	printf("Philosopher %d is releasing left chopstick i.e. chopstick %d.\n", i, i);
	// sleep(5);	
	sem_post(&chopstick[(i+1)%n]); // releasing right chopstick
	printf("Philosopher %d is releasing right chopstick i.e. chopstick %d.\n", i, (i+1)%n);

	// mutex lock to ensure only one reader is changing phildone at any point of time
	pthread_mutex_lock(&mutex);
		phil_done++;
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

/*
Sample output:

Enter the number of philosophers on round table(max seating capacity is 10) : 
6
Philosopher 0 is thinking.
Philosopher 0 is hungry now.
Philosopher 0 is eating dinner now. slot 0
Philosopher 1 is thinking.
Philosopher 2 is thinking.
Philosopher 3 is thinking.
Philosopher 4 is thinking.
Philosopher 5 is thinking.
Philosopher 3 is hungry now.
Philosopher 4 is hungry now.
Philosopher 4 is eating dinner now. slot 0
Philosopher 5 is hungry now.
Philosopher 0 is releasing left chopstick i.e. chopstick 0.
Philosopher 0 is releasing right chopstick i.e. chopstick 1.
Philosopher 1 is hungry now.
Philosopher 2 is hungry now.
Philosopher 4 is releasing left chopstick i.e. chopstick 4.
Philosopher 4 is releasing right chopstick i.e. chopstick 5.
Philosopher 3 is eating dinner now. slot 1
Philosopher 5 is eating dinner now. slot 1
Philosopher 1 is eating dinner now. slot 1
Philosopher 3 is releasing right chopstick i.e. chopstick 4.
Philosopher 3 is releasing left chopstick i.e. chopstick 3.
Philosopher 5 is releasing right chopstick i.e. chopstick 0.
Philosopher 5 is releasing left chopstick i.e. chopstick 5.
Philosopher 1 is releasing right chopstick i.e. chopstick 2.
Philosopher 1 is releasing left chopstick i.e. chopstick 1.
Philosopher 2 is eating dinner now. slot 0
Philosopher 2 is releasing left chopstick i.e. chopstick 2.
Philosopher 2 is releasing right chopstick i.e. chopstick 3.
All philosophers went back to lab.
*/
