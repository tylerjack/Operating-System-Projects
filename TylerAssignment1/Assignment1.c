//Author: Tyler Jackimowicz
//Assignment 1 
//CSC 4410 Operating Systems
//Date: 02/12/2016

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	//variables
	int fd[2];
	char buffer[BUFSIZ];
	int num_read, fd2;
	pid_t fork_return;

	//create pipe
	pipe(fd);
	
	//spawn child
	fork_return = fork();
		
	//if child
	if(fork_return == 0)
	{	
		//Child's stdin reads from read end of pipe
		dup2(fd[0], 0);

		close(fd[0]); //Close input end of pipe
		close(fd[1]); //Close output end of pipe

		//Replace child process with "more" program
		execl("/bin/more", "more", NULL);

		//Close stdin and stdout of Child
		close(0);
		close(1);
		
		//exit Child
		exit(0);
	}//end if
		
	//else if parent
	else if(fork_return > 0)
	{
		//Parent's stdout writes to pipe
		dup2(fd[1], 1);
		
		close(fd[1]); //Close output end of pipe
		close(fd[0]); //Close input end of pipe
		close(0); //Close stdin of parent	

		//opens input file argument
		int fd2 = open(argv[1], O_RDONLY);
		
		/*while there is more characters in the text file, 
		  write to pipe and wait for child*/
		while((num_read = read(fd2, &buffer, sizeof(buffer))) > 0)
		{
			write(1, &buffer, (strlen(buffer)));
			close(fd2); //Close file descripter for open file
			close(1); //Close stdout of parent
			waitpid(fork_return, NULL, 0);
		}//end while
		
		//exit parent process
		exit(0);
	}//end else if

	return 0;
}//end main

