/*
Author : Soumya Malgonde
Aim : File Handling System Calls:- Implement an assignment using File Handling System Calls (Low level system calls like open, read, write, etc).

(Compilation using: gcc filename.c )
*/

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define SZ 2000

int main()
{
	char c, text1[SZ], text2[SZ], data1[SZ*2], data2[SZ*2];
	int fd, n;
	
	// writing data in file1.txt
    fd = open("file1.txt", O_WRONLY | O_CREAT, 0777);  // create if not present and open in write only
    if (fd == -1)  
    { 
        printf("Error while opening file.");                
    } 
	printf("Enter text (enter > to end):\n");
	scanf("%[^>]s", text1);
	text1[strlen(text1)] = '\0';
	n = write(fd, text1, strlen(text1)+1);
	close(fd);
	
	// reading from file1.txt and calculating required stuff
	char vowelsfromfile1[SZ];
	fd = open("file1.txt", O_RDONLY);
	if (fd == -1)  
    { 
        printf("Error while opening file.");                
    }
    n = read(fd, text2, SZ);
    if(n == -1)
    {
    	printf("Error while reading file.");
    }
    int i, j = 0;
    for(i=0 ; i<n ; i++)
    {
    	if(text2[i] == 'a' || text2[i] == 'e' || text2[i] == 'i' || text2[i] == 'o' || text2[i] == 'u' || 
    	text2[i] == 'A' || text2[i] == 'E' || text2[i] == 'I' || text2[i] == 'O' || text2[i] == 'U')
    	{
    		vowelsfromfile1[j] = text2[i];
    		j++;
    	}
    }
    vowelsfromfile1[j] = '\0';
    sprintf(data1, "Number of vowels in file1.txt = %d\nVowels in sequence are: %s\n", j, vowelsfromfile1);
    
    // writing the findings in file2.txt
    fd = open("file2.txt", O_WRONLY | O_CREAT, 0777);  // create if not present and open in write only
    if (fd == -1)  
    { 
        printf("Error while opening file.");                
    }
	n = write(fd, data1, strlen(data1)+1);
	close(fd);
	
	// reading file2.txt and writing to console
	fd = open("file2.txt", O_RDONLY);
	if (fd == -1)  
    { 
        printf("Error while opening file.");                
    }
    n = read(fd, data2, 2*SZ);
    if(n == -1)
    {
    	printf("Error while reading file.");
    }
    printf("Data read from file2.txt: \n%s", data2);
	exit(0);
}
