//Author: Tyler Jackimowicz
//Assignment 3
//CSC 4410 Operating Systems
//Date: 04/01/2016
//Description: Process B reads the account information from shared memory and stores the total amount into the clearinghouse total that is also in shared memory. After, it zeros out each sponsor's account. 

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>       
#include <stdlib.h>
#include <pthread.h> 
#include <semaphore.h>
#include "sshm.h"

//shared memory key for each sponsor account
#define sp0 97531
#define sp1 97532
#define sp2 97533
#define sp3 97534
#define sp4 97535
#define chk 97536


int main()
{
	//opens semaphore semA and initializes semaphore semC
	sem_t* semA = sem_open("semA\0", O_CREAT, 0666, 1);
	sem_t* semC = sem_open("semC\0", O_CREAT, 0666, 1);
	
	//variables
	int chId, *clearingHouse, *spA0, *spA1, *spA2, *spA3, *spA4, id0, id1, id2, id3, id4;
	
	//creates shared memory for clearinghouse total
	chId = shm_get(chk, (void **) &clearingHouse, 1);
	
	//retrieves shared memory for each sponsor account
	sem_wait(semA);//try
	/*
	* Critical section
	*/
	id0 = shm_get(sp0, (void **) &spA0, 1); 
	id1 = shm_get(sp1, (void **) &spA1, 1); 
	id2 = shm_get(sp2, (void **) &spA2, 1);
	id3 = shm_get(sp3, (void **) &spA3, 1); 
	id4 = shm_get(sp4, (void **) &spA4, 1);
	sem_post(semA); //exit

	//stores total sponsor accounts into clearinghouse to share with process C
	sem_wait(semC); //try
	*clearingHouse = *spA0 + *spA1 + *spA2 + *spA3 + *spA4; //critical section
	sem_post(semC); //exit

	//print shared memory information for each sponsor account
	printf("Sponsor 0 Account: %d\n", *spA0);
	printf("Sponsor 1 Account: %d\n", *spA1);
	printf("Sponsor 2 Account: %d\n", *spA2);
	printf("Sponsor 3 Account: %d\n", *spA3);
	printf("Sponsor 4 Account: %d\n", *spA4);	

	//zero each sponsor account
	sem_wait(semA); //try
	/*
	* Critical section
	*/
	*spA0 = 0; 
	*spA1 = 0; 
	*spA2 = 0; 
	*spA3 = 0; 
	*spA4 = 0; 
	sem_post(semA); //exit

	sem_close(semA); //close semaphore semA
	sem_destroy(semA); //destroys semaphore semA
	sem_close(semC); //close semaphore semC

	//removes shared memory for sponsor accounts
	shm_rm(id0);
	shm_rm(id1);
	shm_rm(id2);
	shm_rm(id3);
	shm_rm(id4);
}//end main
