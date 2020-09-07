/*
Author : Soumya Malgonde
Secondary program called from main program using execve

(Program recieves sorted array and integer to be searched from command line arguments and performs binary search)
(Compilation using: gcc filename.c -o bs)
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int n = argc-1, i;
    char *remaining = NULL;
    int tobesearched = strtol(argv[n], &remaining, 10); // last argument is the integer to be searched for
    int arr[n];

    //conversion of characters received from command line to integer array
    printf("Array received in secondary program: ");
    for(i=1 ; i<n ; i++)
    {
        arr[i-1] = strtol(argv[i], &remaining, 10);
        printf("%d ", arr[i-1]);
    }
    printf("\n");
    
    // binary search
    int start = 0, end = n-1, mid, flag = 1;
    while(start <= end)
    {
        mid = (start + end)/2;
        if(arr[mid] == tobesearched)
        {
            printf("Value %d found in the given array at position %d.\n", tobesearched, mid);
            flag = 0;
            break;
        }
        else if(arr[mid] < tobesearched)
            start = mid + 1;
        else
            end = mid - 1;
    }
    if(flag)
        printf("Value %d is not present in the given array.\n", tobesearched);
}