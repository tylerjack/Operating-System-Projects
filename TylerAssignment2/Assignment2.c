//Author: Tyler Jackimowicz
//Assignment 2
//CSC 4410 Operating Systems
//Date: 03/03/2016
//Description: Assignemnt2.c creates 5 "children" processes from one parent that utilizes the lock and unlock methods, in which create or delete files in a way that mimics semaphores. This is utilized in the main function for each child process or "Philosopher" to go through states of eating, thinking, being hungry or sated. This program is intended to run through the dining philosopher's problem and recognize deadlock.  

#include <stdio.h>
#include <unistd.h>
#include <string.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

//functions
int lock(int cNum);
int unlock(int cNum);

int main()
{
	//for loop to delete any cp files previously created in the tmp folder. 
	for(int i = 0; i < 5; i++)
		unlock(i);


	//variables
	pid_t pid[5];//array of process IDs 
	int i;
	int n = 5;//number of children
		
	
	//for loop to create 5 children processes and run the code
	for(i = 0; i < n; i++) 
	{
		pid[i] = fork(); //spawn child
		
		do//do-while 
		{
			if(lock(i) == 0) //if cp(i) file is not already created, create file
			{
				/*checks if i < 4 (not philosopher 5) and that cp(i+1) file is not 					  already created and creates the file. (Philosopher picks up right 					  chopstick)*/
				if((i < 4) && (lock(i+1) == 0))
				{	
					//change state to hungry
					printf("Philosopher %d hungry\n", i); 
					sleep(3);
					
					//change state to eating
					printf("Philosopher %d eating\n", i);
					sleep(3);

					/*delete cp(i)*left* and cp(i+1)*right* to unlock chopsticks  						  for other philosophers*/
					unlock(i);
					unlock(i+1);

					//changes state to sated	
					printf("Philosopher %d sated\n", i);	
					sleep(3);

					//changes state back to thinking
					printf("Philosopher %d thinking\n", i);
					sleep(3);	
				}//end if

				else if((i == 4) && (lock(0) == 0))
				/*checks if i = 4 (philosopher 5) and that cp(0) has not been created 					  and creates the file (Philosopher picks up right chopstick)*/
				{
					//change state to hungry
					printf("Philosopher %d hungry\n", i); 
					sleep(3);
					
					//changes state to eating
					printf("Philosopher %d eating\n", i);
					sleep(3);

					/*delete cp(i)*left* and cp(i+1)*right* to unlock chopsticks  						  for other philosophers*/
					unlock(i);
					unlock(0);
				
					//changes state to sated	
					printf("Philosopher %d sated\n", i);	
					sleep(3);

					//changes state back to thinking
					printf("Philosopher %d thinking\n", i);
					sleep(3);
				}//end else if
				else
				{
					/*if right chopstick is being used, wait and unlock left 						  chopstick*/
					sleep(5);
					unlock(i);
					sleep(3);
				}//end else		
			}//end if
		}//end do
		while(pid[i]);//do above code while child process is running	
	}//end for
	return 0;
}//end main()


/*Function name: lock(int)
* Description: Creates file called cp(cNum passed through parameter) and returns -1 if the file  * was not able to be created or 0 if the file was created.
* Parameters: int cNum
* Return: int
*/
int lock (int cNum){
	//char variable to hold fileName
	char fileName[20];
	
	//creates fileName with the directory and adds the number passed through parameters. 
	sprintf(fileName, "/tmp/cp%d", cNum);

	//creates file 
	int fd = creat(fileName, S_IRUSR);

	//if fd returns -1 (error), the file has already been created.
	if(fd == -1){
		//Could not create file
		return -1;
	}//end if
	else{   //else file was created
		close(fd);
		return 0;
	}//end else
}//end lock(int)

/*Function name: unlock(int)
* Description: Unlock function deletes the file and returns -1 if file was not found, or 0 if    the file was deleted. 
* Parameters: int cNum
* Return: int
*/
int unlock (int cNum){
	//char variable to hold fileName
	char fileName[20];

	//creates fileName with the directory and adds the number passed through parameters.
	sprintf(fileName, "/tmp/cp%d", cNum);

	//deletes file and stores result in i;
	int i = unlink(fileName);

	//if i returns -1 (error) return -1
	if(i == -1){
		//File not found
		return -1;
	}//end if
	else if(i == 0){
		//File was deleted, return 0
		return 0;
	}//end else if
}//end unlock(int)


