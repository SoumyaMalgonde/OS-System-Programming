/*
Author : Soumya Malgonde
Aim : Process Control System Calls (Demonstrate zombie state)

(Compilation using: gcc filename.c)
(Run program in one terminal and rum command 'ps -elf' after child has exited and parent is still running to see zombie status of child i.e., <defunct>)
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void swap(int *x, int *y)  
{  
    int temp = *x;  
    *x = *y;  
    *y = temp;  
}

void insertionsortascend(int arr[], int n)  
{  
    int i, temp, j;  
    for(i=1 ; i<n ; i++) 
    {  
        temp = arr[i];
        j = i-1;
        while(j>=0 && arr[j]>temp) 
        {  
            arr[j+1] = arr[j];  
            j = j-1;  
        }  
        arr[j+1] = temp;
    }  
}

void bubblesortdescend(int arr[], int n)
{
	int i, j;
	for (i=0 ; i<n-1 ; i++)
        for (j=0 ; j<n-i-1 ; j++)  
            if (arr[j] < arr[j+1])  
                swap(&arr[j], &arr[j+1]);
}

int main(int argc, char *argv[])
{
	int n, i, j;
    int arr[200];
    printf("Enter size of the array : ");
    scanf("%d", &n);
    printf("Enter space separated array elements : \n");
    for(i=0 ; i<n ; i++)
        scanf("%d", &arr[i]);
	pid_t cpid;
	cpid = fork();

	if (cpid == -1) 
	{
	   perror("fork");
	   exit(EXIT_FAILURE);
	}

	if (cpid == 0) //child
	{
		printf("\nChild Process\n");
		//sorting array using insertion sort in ascending order
        insertionsortascend(arr, n);
        printf("\nArray in ascending order is (child process) : ");
        for(i=0 ; i<n ; i++)
        	printf("%d ", arr[i]);
        printf("\nChild exits.\n");
	} 
	else //parent
	{
		printf("\nParent Process\n");
		sleep(10); //control goes to child
		wait(NULL); //removes child entry after handling the signal
		//sorting array using bubble sort in decending order
        bubblesortdescend(arr, n);
        printf("\nArray in descending order is (parent process) : ");
        for(i=0 ; i<n ; i++)
        	printf("%d ", arr[i]);
        printf("\nParent exits.\n");
	}
}
