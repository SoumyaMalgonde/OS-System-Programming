/*
Author : Soumya Malgonde
Aim :  Inter process communication in Linux using FIFOs.
		Implementation of Full duplex communication between two independent processes. First process accepts sentences and writes on one pipe to be read by second process and second process counts number of characters, number of words and number of lines in accepted sentences, writes this output in a text file and writes the contents of the file on second pipe to be read by first process and displays on standard output.

(Compilation using: gcc filename.c -o p2)
*/

// C program to implement one side of FIFO 
// This side reads first, then writes 
#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 

int main() 
{ 
	int fd1; 
	// FIFO file path 
	char * myfifo = "/tmp/myfifo"; 
	char text[200], text1[200]; 
	FILE *fp;

	// First open in read only and read 
	fd1 = open(myfifo,O_RDONLY); 
	read(fd1, text, 200); 
	// Print the read string and close 
	printf("Text read from FIFO :\n%s\n", text); 
	close(fd1); 

	//write in file and in text1
	int chars = 0, words = 0, lines = 0, c = 0, i, j;
	for(i=0 ; text[i] != '\0' ; i++)
	{
		if(text[i] != '\n' && text[i] != ' ')
		{	
			chars++;
			c++;
		}
		if(text[i] == '\n' && c > 0) 
		{
			lines++;
			c = 0;
		}
	}
	lines++;
	for(i=0 ; text[i] != '\0' ; i++)
	{
		if((text[i] == ' ' || text[i] == '\n') && c > 0)
		{
			words++;
			
			c = 0;
		}
		else
		{
			if(c == 0 && text[i] != ' ' && text[i] !=  '\n')
				c = 1;
		}
	}
	if(c)
		words++;
	
	sprintf(text1, "Number of characters = %d\nNumber of words = %d\nNumber of lines = %d\n", chars, words, lines);

    fp = fopen ("output.txt", "w");
    fputs(text1, fp);
    fclose(fp);

	// Now open in write mode and write the details
	fd1 = open(myfifo, O_WRONLY);
	printf("Writing number of chars, words, and lines read back to FIFO...\n");
	
	// Write the input text on FIFO and close it 
	write(fd1, text1, strlen(text1)+1); 
	close(fd1);
	return 0; 
} 

