//generate 2,000,000 data values for the disk scheduling algorithms
#include <stdio.h>

int main()
{
	
	srand();
	int current, prev, prever, distance;
	current = rand() % 2000;
	prev = rand() % 2000;
	prever = rand() % 2000;
	int i;
	for(i = 0; i < 1000000; i++)
	{
		int random = rand() % 1000;
		if(random <= 25)
		{
			distance = (random % 2) + 1;
			if(random % 3 == 0)
			{
				current = (prever - distance) % 2000;
			}
			else
			{
				current = (prever + distance) % 2000; 
			}		
		}
		else if (random <= 75)
		{
			distance = (random % 2) + 1;
			if(random % 3 == 0)
			{
				current = (prev - distance) % 2000;
			}
			else
			{
				current = (prev + distance) % 2000; 
			}
		}
		else if (random <= 175)
		{
			distance = (random % 2) + 1;
			if(random % 3 == 0)
			{
				current = (current - distance) % 2000;
			}
			else
			{
				current = (current + distance) % 2000; 
			}

		}
		else
		{	
			current = rand() % 2000;
		}

		printf("%d\n", current);	
		prever = prev;
		prev = current;	
	}

}
