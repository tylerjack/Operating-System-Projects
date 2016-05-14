//Author: Tyler Jackimowicz
//Assignment 3
//CSC 4410 Operating Systems
//Date: 04/01/2016
//Description: Process A has two threads in order to retrieve data from moogle.txt, update the sponsor's array, and create a shared memory variable to send sponsor account information. 

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>       
#include <stdlib.h>
#include <string.h>
#include <pthread.h> 
#include <semaphore.h>
#include <stdbool.h>
#include "sshm.h"

//shared memory key for each sponsor account
#define sp0 97531
#define sp1 97532
#define sp2 97533
#define sp3 97534
#define sp4 97535

//global variables
sem_t* semA;
int sponsors[5];
bool isDone;

//Thread 1 reads file and stores information into array
void* Thread1()
{
	//variables
	int company_id, charge;

	//opens text file and stores pointer to file
	FILE *fp= fopen("Moogle.txt", "r");

	//error checking
	if (fp == 0)
	{
		fprintf(stderr, "failed to open Moogle.txt\n");
		exit(1);	
	}	
	
	//while it scans file and stores the information into company_id and charge
	while(fscanf(fp, "%d %d", &company_id, &charge) > 0)
	{
		
		sem_wait(semA); //Try
		sponsors[company_id] = charge; //Critical section
		sem_post(semA); //Exit
		//printf("sponsor: %i  charge: %i\n", company_id, sponsors[company_id]);
		isDone = false; //more data in file
	}

	isDone = true; //no more data in file
	fclose(fp); //close file pointer
	pthread_exit(NULL); //exit pthread
}//end thread1

//Thread 2 stores array values into sponsor account in shared memory 
void* Thread2()
{
	//variables
	int chargeValue, i;
	int *spA0, *spA1, *spA2, *spA3, *spA4;

	//retrieves shared memory for each sponsor account
	shm_get(sp0, (void **) &spA0, 1);
	shm_get(sp1, (void **) &spA1, 1);
	shm_get(sp2, (void **) &spA2, 1);
	shm_get(sp3, (void **) &spA3, 1);
	shm_get(sp4, (void **) &spA4, 1);
	
	//while thread 1 is still reading file
	while(!isDone)
	{
		//if sponsor 0 has a charge
		if(sponsors[0] != 0)
		{
			sem_wait(semA); //try
			*spA0 = *spA0 + sponsors[0];//critical section
			sponsors[0] = 0; //zeroing array value
			sem_post(semA); //exit
		}//end if
		//if sponsor 1 has a charge
		if(sponsors[1] != 0)
		{
			sem_wait(semA); //try
			*spA1 = *spA1 + sponsors[1]; //critical section
			sponsors[1] = 0; //zeroing array value
			sem_post(semA); //exit
		}//end if
		//if sponsor 2 has a charge
		if(sponsors[2] != 0)
		{
			sem_wait(semA); //try
			*spA2 = *spA2 + sponsors[2]; //critical section
			sponsors[2] = 0; //zeroing array value
			sem_post(semA); //exit
		}//end if
		//if sponsor 3 has a charge
		if(sponsors[3] != 0)
		{
			sem_wait(semA); //try
			*spA3 = *spA3 + sponsors[3]; //critical section
			sponsors[3] = 0; //zeroing array value
			sem_post(semA); //exit
		}//end if
		//if sponsor 4 has a charge
		if(sponsors[4] != 0)
		{
			sem_wait(semA); //try
			*spA4 = *spA4 + sponsors[4]; //critical section
			sponsors[4] = 0; //zeroing array value
			sem_post(semA); //exit
		}//end if
	}//end while

	//print each sponsor's account 
	printf("Sponsor 0 Account: %d\n", *spA0);
	printf("Sponsor 1 Account: %d\n", *spA1);
	printf("Sponsor 2 Account: %d\n", *spA2);
	printf("Sponsor 3 Account: %d\n", *spA3);
	printf("Sponsor 4 Account: %d\n", *spA4);
	
	pthread_exit(NULL); //exit pthread
}//end thread2

int main(int argc, char* argv[])
{
	//variables
	int *spA0, *spA1, *spA2, *spA3, *spA4, id0, id1, id2, id3, id4; //shared memory variables
	pthread_t thd1, thd2; //thread ids
  	int val1, val2; //return of thread creation

	semA = sem_open("semA\0", O_CREAT, 0666, 1); //initialize semaphore
	
	//create shared memory to store sponsor's account	
	id0 = shm_get(sp0, (void **) &spA0, 1);
	id1 = shm_get(sp1, (void **) &spA1, 1);
	id2 = shm_get(sp2, (void **) &spA2, 1);
	id3 = shm_get(sp3, (void **) &spA3, 1);
	id4 = shm_get(sp4, (void **) &spA4, 1);

	//create thread one and two
	val1 = pthread_create(&thd1, NULL, Thread1, NULL);
	val2 = pthread_create(&thd2, NULL, Thread2, NULL);

	//Error checking
	if (val1){
	   printf("\n ERROR: return code from pthread_create is %d \n", val1);
	   exit(1);
	}
	//Error checking
	if (val2){
	   printf("\n ERROR: return code from pthread_create is %d \n", val2);
	   exit(1);
	}
	
	//waits for thread 1 and 2 to terminate
	pthread_join(thd1, NULL);
	pthread_join(thd2, NULL);

	sem_close(semA); //close semaphore semA

	return 0;
}//end main
