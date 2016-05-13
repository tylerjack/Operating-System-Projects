//Author: Tyler Jackimowicz
//Assignment 4
//CSC 4410 Operating Systems
//Date: 04/28/2016
//Description: diskScheduling.c creates three methods to imitate first come first served algorithm, shortest seek time first algorithm, and look algorithm. Main will print out the total head movements for each algorithm using cylinder values from a text file. 

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>       
#include <stdlib.h>
#include <string.h>

//FCFS reads file and conducts FCFS algorithm with the data and returns total head movement
int FCFS (const char *file)
{
	//variables
	int totalHeadmvmt = 0, prevNum = 0, cylNum = 0;

	FILE *fp = fopen(file, "r");//opens text file and stores pointer to file
	
	//error handling
	if (fp == 0)
	{
		fprintf(stderr, "failed to open text file\n");
		exit(1);	
	}//end if

	//scans file and stores information into cylNum
	while(fscanf(fp, "%d", &cylNum) > 0)
	{
		totalHeadmvmt += abs(prevNum - cylNum); //adds distance to total head movement
		prevNum = cylNum; //prevNum will be used in next iteration
	}//end while
	
	fclose(fp); //close file pointer
	return totalHeadmvmt; //returns total head movement
}//end FCFS

//SSTF reads a text file, stores 40 integers at a time and utlizes the SSTF algorithm 
//to sort through integers and adds to total head movement
int SSTF (const char *file){
	//variables
	int totalHeadmvmt = 0, cylNum, head = 0, i = 0, j, k, closest;
	int position, prequests[40];

	FILE *fp = fopen(file, "r");//opens text file and stores pointer to file

	//error handling
	if (fp == 0)
	{
		fprintf(stderr, "failed to open text file\n");
		exit(1);	
	}//end if

	//until end of file is reached iterate this section of code 
	while(!feof(fp))
	{
		int total = 0; //total values read in for loop
		for(i = 0; i < 40; i++) //loop 40 times
		{
			fscanf(fp, "%d", &cylNum);//scan file and store integer into cylNum
			if(feof(fp))//if end of file is reached, break for loop
				break;
			prequests[i] = cylNum; //store cylNum in array prequests
			total++; //adds to local total number in loop 
		}//end if

		for(j = 0; j < total; j++) //loop code for total amount of cylinders in array
		{
			closest = abs(prequests[j]-head); //store closest value to head location
			position = j; //position of loop and array
			for(k = j; k < total; k++) //loop for each element in array
			{
				if(closest>abs(head-prequests[k])) //if there is a closer value
				{
					position = k; //position of array and loop
					closest = abs(head-prequests[k]); //store the closest value
				}//end if
			}//end for

			//add distance to total head movement
			totalHeadmvmt += abs(head - prequests[position]); 
			//head location is now equal to array element
			head = prequests[position];
			//closest element is now equal to current position in array
			prequests[position] = prequests[j];
			//array element is equal to head location to be used in next iteration
			prequests[j] = head;
		}//end for		
	}//end while

	fclose(fp); //close file pointer
	return totalHeadmvmt; //returns total head movement
}//end SSTF

//Look reads a text file, stores 40 integers at a time and utlizes the Look algorithm 
//to sort through integers and adds to total head movement
int Look (const char *file)
{
	//variables
	int totalHeadmvmt = 0, cylNum, head = 0, i, j, k, position = 0, prequests[40];

	FILE *fp = fopen(file, "r");//opens text file and stores pointer to file

	//error handling
	if (fp == 0)
	{
		fprintf(stderr, "failed to open text file\n");
		exit(1);	
	}
	
	//until end of file is reached iterate this section of code 
	while(!feof(fp))
	{
		int total = 0;//total values read in for loop
		for(i = 0; i < 40; i++)//loop 40 times
		{
			fscanf(fp, "%d", &cylNum);//scan file and store integer into cylNum
			if(feof(fp))//if end of file is reached, break for loop
				break;
			prequests[i] = cylNum;//store cylNum in array prequests
			total++; //adds to local total number in loop 
		}//end for

		for(i = 0; i < total; i++)//loop code for total amount of cylinders in array
		{
			//iterate through code for each element in array and subtracts from total
			//to avoid unnecessary checking
			for(j = 0; j < (total-i-1); j++)
			{
				//if current cylinder number is greater than next element
				if(prequests[j]>prequests[j+1])
				{
					head = prequests[j];//set head to current array position
					prequests[j] = prequests[j+1]; //switch current with next
					prequests[j+1] = head; //next element is now = to current
				}//end if
			}//end for
		}//end for

		for(i = 0; i < position; i++)//loop code depending on start position
		{
			//loop code for each element in array and subtracts from position
			//to avoid unnecessary checking
			for(j = 0; j < (position-i-1); j++) 
			{
				//if current array element is less than next element
				if(prequests[j]<prequests[j+1])
				{
					//head location is set to current array element
					head = prequests[j];
					//current array element is set to next element
					prequests[j] = prequests[j+1];
					//next element in array is set to current
					prequests[j+1] = head;	
				}//end if
			}//end for
		}//end for

		head = 0;//reset head location to start point

		for(i = 0; i < position; i++)//loop through start position
		{
			totalHeadmvmt += abs(prequests[i]-head);//add distance to total head movement
			head = prequests[i]; //head location is set to current array element
		}//end for

		for (i = position; i < total; i++)//loop from position till total elements are reached
		{
			totalHeadmvmt += abs(prequests[i]-head);//add distance to total head movements
			head = prequests[i];//head location is set to current array element
		}//end for
	}//end while
	
	fclose(fp); //close file pointer
	return totalHeadmvmt;//returns total head movement
}//end Look

//Main sends file, provided in command line, to each method and prints out 
//total head movements for each algorithm.
int main(int argc, char* argv[])
{
	printf("FCFS Total Head movements %s: %d\n", argv[1], FCFS(argv[1]));//FCFS
	printf("SSTF Total Head movements %s: %d\n", argv[1], SSTF(argv[1]));//SSTF
	printf("Look Total Head movements %s: %d\n", argv[1], Look(argv[1]));//Look
	
	return 0;
}//end main
