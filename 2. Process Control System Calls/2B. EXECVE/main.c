/*
Author : Soumya Malgonde
Aim : Execve system call
Implement the C program in which main program accepts an integer array. Main program uses the FORK system call to create a new process called a child process. Parent process sorts an integer array and passes the sorted array to child process. The child process uses EXECVE system call to load new program that uses this sorted array for performing the binary search to search the particular item in the array.

(Compilation using: gcc filename.c)
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

void bubblesortascend(int arr[], int n)
{
    int i, j;
    for (i=0 ; i<n-1 ; i++)
        for (j=0 ; j<n-i-1 ; j++)  
            if (arr[j] > arr[j+1])  
                swap(&arr[j], &arr[j+1]);
}

int main(int argc, char *argv[])
{
    int n, i;
    int arr[200];
    printf("Enter size of the array : ");
    scanf("%d", &n);
    printf("Enter space separated array elements : \n");
    for(i=0 ; i<n ; i++)
        scanf("%d", &arr[i]);
    
    printf("In parent process...sorting the array.\n");
    bubblesortascend(arr, n);

    pid_t cpid;
    cpid = fork();
    if (cpid == -1) 
    {
       perror("fork");
       exit(EXIT_FAILURE);
    }
    if(cpid > 0) //parent
    {
        printf("In parent process.(Waiting for child to finish)\n");
        wait(0);
        printf("\nChild process completed. Parent Exiting...\n");
        
    }
    else //child cpid=0
    {
        printf("\nIn child process.\n");
        char *arc[200], str[20];
        for(i=0 ; i<n ; i++)
        {
        	sprintf(str, "%d", arr[i]);
        	arc[i+1] = (char *)malloc(20*sizeof(char));
        	strcpy(arc[i+1], str);
        }
        int num;
        printf("Which integer do you want to search : ");
        scanf("%d", &num);
        sprintf(str, "%d", num);
    	arc[n+1] = (char *)malloc(20*sizeof(char));
    	strcpy(arc[n+1], str);

        execve("./bs", arc, NULL); //execve call
    }
}