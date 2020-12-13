/*
Author : Soumya Malgonde
Aim :  Inter process communication in Linux using FIFOs.
		Implementation of Full duplex communication between two independent processes. First process accepts sentences and writes on one pipe to be read by second process and second process counts number of characters, number of words and number of lines in accepted sentences, writes this output in a text file and writes the contents of the file on second pipe to be read by first process and displays on standard output.

(Compilation using: gcc filename.c -o p1)
*/

// C program to implement one side of FIFO 
// This side writes first, then reads 
#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 

int main() 
{ 
	int fd; 
	// FIFO file path 
	char * myfifo = "/tmp/myfifo"; 

	// Creating the named file(FIFO)
	// mkfifo(<pathname>,<permission>) 
	mkfifo(myfifo, 0666); 
	char text[200]; 
	printf("Enter text (enter > to end):\n");
	scanf("%[^>]s", text);
	printf("Writing above text to FIFO...\n");
	
	fd = open(myfifo, O_WRONLY); // Open FIFO for write only 

	// Write the input text on FIFO and close it 
	write(fd, text, strlen(text)+1); 
	close(fd); 

	// Open FIFO for Read only 
	fd = open(myfifo,O_RDONLY); 
	read(fd, text, 200); 
	
	// Print the read string and close 
	printf("\nText read back from FIFO :\n%s", text); 
	close(fd); 
		
	return 0; 
} 

