/*
Author : Soumya Malgonde
Aim :  Inter-process Communication using Shared Memory using System V.
		Application to demonstrate: Client and Server Programs in which server process creates a shared memory segment and write the message to the shared memory segment. Client process reads the message from the shared memory segment and displays it to the screen.

(Compilation using: gcc filename.c -o client)
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
	int shmid;
	srand((unsigned int)getpid());
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
	shared_stuff->written_by_you = 0;
	while(running) 
	{
		if (shared_stuff->written_by_you) 
		{
			printf("Details received from server.\n");
			printf("Account No.: %s\nName: %s\nAccount Balance: %d\n", shared_stuff->acc_no,shared_stuff->name, shared_stuff->acc_bal);
			int loop = 1;
			while(loop)
			{
				int ch, amt;
				printf("\nMenu:\n 1. Withdraw money\n 2. Deposit money\n 3. Exit\nEnter your choice: ");
				scanf("%d", &ch);
				switch(ch)
				{
					case 1:
							printf("Enter amount to withdraw: ");
							scanf("%d", &amt);
							if(amt > shared_stuff->acc_bal)
								printf("You do not have enough balance. Withdrawal failed.\n");
							else
							{
								printf("Withdraw successful collect your money.\n");
								shared_stuff->acc_bal -= amt;
								printf("Account Bal. = %d\n", shared_stuff->acc_bal);
							}
						break;
					case 2:
							printf("Enter amount to deposit: ");
							scanf("%d", &amt);
							printf("Amount deposited.\n");
							shared_stuff->acc_bal += amt;
							printf("Account Bal. = %d\n", shared_stuff->acc_bal);
						break;
					case 3:
							loop = 0;
						break;
					default: printf("Enter valid choice.\n");
				}
			}
			shared_stuff->written_by_you = 0;
			running = 0;
		}
	}
	if (shmdt(shared_memory) == -1) 
	{
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}
	if (shmctl(shmid, IPC_RMID, 0) == -1) 
	{
		fprintf(stderr, "shmctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
