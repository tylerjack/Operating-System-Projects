//Author: Tyler Jackimowicz
//Assignment 3
//CSC 4410 Operating Systems
//Date: 04/01/2016
//Description: Process C transfers the funds from the clearinghouse total to moogle's account, essentially paying moogle, zeroing out the clearinghouse total after. 

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>       
#include <stdlib.h>
#include <pthread.h> 
#include <semaphore.h>
#include "sshm.h"

//clearinghouse key for shared memory
#define chk 97536

int main()
{	
	sleep(3);

	//variables
	int moogleAcc, *clearingHouse, chId;

	//opens semaphore semC	
	sem_t* semC = sem_open("semC\0", O_CREAT, 0666, 1);
	
	//retrieves shared memory of clearinghouse
	sem_wait(semC);//try
	chId = shm_get(chk, (void **) &clearingHouse, 1);//critical section
	sem_post(semC); //exit
	
	moogleAcc = *clearingHouse; //transfer funds to moogle account

	*clearingHouse = 0; //zero clearing house

	printf("Funds added to Moogle account: %d\n", moogleAcc); //prints moogles account balance

	sem_close(semC); //close semaphore semC
	sem_destroy(semC); //destroy semaphore semC
	shm_rm(chId); //remove shared memory for clearinghouse
}//end main
