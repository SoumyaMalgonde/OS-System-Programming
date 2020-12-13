/*
Author : Soumya Malgonde
Aim :  Inter-process Communication using Shared Memory using System V.
		Application to demonstrate: Client and Server Programs in which server process creates a shared memory segment and write the message to the shared memory segment. Client process reads the message from the shared memory segment and displays it to the screen.

(Compilation using: gcc filename.c -o server)
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "sm_com.h"
int main()
{
	int running = 1;
	void *shared_memory = (void *)0;
	struct shared_use_st *shared_stuff;
	char buffer[BUFSIZ];
	int shmid;
	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
	if (shmid == -1) 
	{
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}
	shared_memory = shmat(shmid, (void *)0, 0);
	if (shared_memory == (void *)-1) 
	{
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}
	shared_stuff = (struct shared_use_st *)shared_memory;
	while(running) 
	{
		while(shared_stuff->written_by_you == 1) 
		{
			sleep(1);
			printf("waiting for client...\n");
		}
		printf("Enter Account Number: ");
		scanf("%s", shared_stuff->acc_no);
		printf("Enter Name: ");
		scanf("%s", shared_stuff->name);
		printf("Enter Account Balance: ");
		scanf("%d", &shared_stuff->acc_bal);
		shared_stuff->written_by_you = 1;
		running = 0;
	}
	if (shmdt(shared_memory) == -1) 
	{
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
