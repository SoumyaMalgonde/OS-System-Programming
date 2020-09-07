/*
Author : Soumya Malgonde
Aim : To implement C program for Matrix Multiplication using POSIX pthreads

(Program can perform matrix multiplication normally as well as using POSIX pthreads)
(Compilation using: gcc filename.c -lpthread)
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

struct topass //struct which has all the information to be passed to thread function...address of struct object will be passed to thread function
{
	int **a, **b;
	int posid;
	int k, n2;
	int res;
}topass_para[200];

void *thread_function(void *arg);
int main() 
{
	int ress;
	pthread_t a_thread[200];
	int mat1[10][10], mat2[10][10], res[10][10], resmat[10][10], n1, n2, m1, m2;
	void *thread_result;
	int i, j, k;
	// taking input matrices
	printf("Enter dimensions of first matrix (space separated, max 10 x 10): \n");
	scanf("%d %d", &m1, &n1);
	printf("Enter dimensions of second matrix (space separated, max 10 x 10): \n");
	scanf("%d %d", &m2, &n2);
	
	if(n1 == m2) //checking if dimensions are suitable for multiplication
	{
		printf("Enter elements of matrix 1:\n");
		for(i=0 ; i<m1 ; i++)
			for(j=0 ; j<n1 ; j++)
				scanf("%d", &mat1[i][j]);

		printf("Enter elements of matrix 2:\n");
		for(i=0 ; i<m2 ; i++)
			for(j=0 ; j<n2 ; j++)
				scanf("%d", &mat2[i][j]);

		int loop = 1;
		while(loop)
		{
			int choice;
			clock_t begin, end;
			double time_spent;
			printf("\nMENU\n 1. Matrix multiplication normally.\n 2. Matrix multiplication using threads.\n 3. Exit.\n");
			printf("Enter your choice : ");
			scanf("%d", &choice);
			switch(choice)
			{
				case 1: //noraml multiplication
				{
					begin = clock();
					int resm[10][10];
					for(int ii=0 ; ii<m1 ; ii++)
						for(int jj=0 ; jj<n2 ; jj++)
						{
							resm[ii][jj] = 0;
							for(int kk=0; kk<n1 ; kk++)
								resm[ii][jj] += (mat1[ii][kk]*mat2[kk][jj]);
						}
					printf("Resultant matrix : \n");
					for(i=0 ; i<m1 ; i++)
					{
						for(j=0 ; j<n2 ; j++)
							printf("%d  ", resm[i][j]);
						printf("\n");
					}
					end = clock();
					time_spent = (double)(end - begin);
					printf("Time taken : %f\n", time_spent);
					printf("\n");
				}
				break;

				case 2: // multiplication using threads...one thread for calculating each resultant element
				{
					begin = clock();
					int *aux1[10];
					int *aux2[10];
					for(int ii=0 ; ii<m1 ; ii++)
						aux1[ii] = (int *)(mat1[ii]);
					for(int ii=0 ; ii<m2 ; ii++)
						aux2[ii] = (int *)(mat2[ii]); 

					// thread creation
					for(i=0 ; i<m1 ; i++)
						for(j=0 ; j<n2 ; j++)
						{
							int pos = i*n1 + j;
							topass_para[pos].a = (int **)aux1;//mat1;
							topass_para[pos].b = (int **)aux2;
							topass_para[pos].posid = pos;
							topass_para[pos].k = n1;
							topass_para[pos].n2 = n2;
							ress = pthread_create(&a_thread[pos], NULL, thread_function, &(topass_para[pos]));
							if (ress != 0) 
							{
								perror("Thread creation failed");
								exit(EXIT_FAILURE);
							}
							sleep(2);
						}

						// joining threads
						for(i=0 ; i<m1*n2 ; i++)
						{
							ress = pthread_join(a_thread[i], &thread_result);
							if (ress != 0) 
							{
								perror("Thread join failed.\n");
								exit(EXIT_FAILURE);
							}
							resmat[i/n2][i%n2] = *(int *)thread_result;
						}
						printf("Resultant matrix : \n");
						for(i=0 ; i<m1 ; i++)
						{
							for(j=0 ; j<n2 ; j++)
								printf("%d  ", resmat[i][j]);
							printf("\n");
						}
				
				end = clock();
				time_spent = (double)(end - begin);
				printf("Time taken : %f\n", time_spent);
				}
				break;

				case 3: //exit menu
					loop = 0;
				break;

				default:
					printf("Enter valid choice.\n");
			}
		}
	}
	else
	{
		printf("Matrices not fit for multiplication.\n");
	}
	exit(EXIT_SUCCESS);
}

void *thread_function(void *arg) 
{
	struct topass temp = *(struct topass *)arg;
	int r = temp.posid / temp.n2;
	int c = temp.posid % temp.n2;
	printf("Thread_function is running for calculating element resultant[%d][%d].\n", r, c);

	int res = 0;
	int n1 = temp.k;
	for(int k=0; k<n1 ; k++)
	{
		res += (*(*(temp.a+r)+k)) * (*(*(temp.b+k)+c));// (mat1[r][k]*mat2[k][c]);
	}
	//returning pointer to object created in thread won't work as it's lifetime is till thread terminates
	(*(struct topass *)arg).res = res; //storing result in res variable of the passed structure object as it's scope is main
	void *p = &((*(struct topass *)arg).res); //passing back address
	sleep(2);
	pthread_exit(p);
}

