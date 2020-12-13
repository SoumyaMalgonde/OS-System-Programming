/*
Author : Soumya Malgonde
Aim :  Inter process communication in Linux using pipes.

(Compilation using: gcc filename.c)
*/

// simple pipe usage with fork ... parent child inter process communication
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include<sys/wait.h> 
#include <string.h>
int main()
{
	int data_processed;
	int fd1[2], fd2[2]; // file descriptors (r and w)
	char buffer[BUFSIZ + 1];
	pid_t fork_result;
	memset(buffer, '\0', sizeof(buffer));
	if (pipe(fd1) == 0 && pipe(fd2) == 0) // creating pipes
	{
		fork_result = fork(); // forking
		if (fork_result == -1) 
		{
			fprintf(stderr, "Fork failure");
			exit(EXIT_FAILURE);
		}
		if (fork_result == 0) // child process
		{
			printf("In child process... (PID: %d)\n", getpid());
			close(fd1[1]); // closing write end of pipe 1
			read(fd1[0], buffer, sizeof(buffer)); // reading req data msg from pipe 1
			printf("\nData required from parent process : %s\n",buffer);
			printf("Enter space separated data : \n");
			char data[200];
			gets(data);
			
			close(fd1[0]); // closing read end of pipe 1
			close(fd2[0]); // closing read end of pipe 2
			write(fd2[1], &data, sizeof(data));   // writing contents of file on Pipe 2
			close(fd2[1]); // closing write end of pipe 2
			printf("(Child Process exiting..)\n");
			exit(0); 
		}
		else // parent process
		{
			close(fd1[0]); // closing read end of pipe 1
			printf("In parent process... (PID: %d)\n", getpid());
			const char data_req[] = "Read and send roll number, name and mobile number of student.";
			data_processed = write(fd1[1], data_req, strlen(data_req));
			
			wait(NULL); // waiting for child
			
			close(fd1[1]); // closing write end of pipe 1
			close(fd2[1]);  // closing write end of pipe 2
			
			printf("\nReceived Details (in parent process):\n");
			char ch;
			int num_bytes = read(fd2[0], &ch, sizeof(ch));
			while(num_bytes != 0) 
			{
				printf("%c", ch);
				num_bytes = read(fd2[0], &ch, sizeof(ch));   // reading contents of file from Pipe 2
			}
			printf("\n");
			close(fd2[0]); // closing read end of pipe 2
		}
	}
	exit(EXIT_SUCCESS);
}
