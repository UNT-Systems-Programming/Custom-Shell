/* 
Group Members: William Wolfard, Clay Brant, Hansaj Patel, Jay Bishop 
Course: CSCE 3600.003 Principles of Systems Programming
Due Date: Monday, November 14
Instructor: Professor Mark Thompson
Description:  This C program implements shell. The implemented shell works in a similar way as UNIX/LINUX.
			  The program can read the user commands and execute them accordingly (interactive mode)
			  The program can also read the commands from a file and execute them (batch mode)
			  The "quit" and "exit" command terminates the shell.
Notes: stack overflow 
*/

//include all the headers that needs to used in the program
#include <stdio.h>	
#include <stdlib.h>	
#include <string.h> 	
#include <unistd.h>
#include<sys/types.h>
#include<sys/wait.h>	
#include<sys/mman.h>

// boolean value for true and false
#define FALSE 0
#define TRUE  1

enum{READ, WRITE};

//launch function that will execute the command using system()
void launch(char **args)
{
	pid_t pid;							//declaring child process
	pid = fork();						//creating child
	if (pid == 0)						// in child
	{
		system(*args);					// executing the command 
		return;
	} 
	else if (pid < 0) 					// error handling
	{
		perror("lsh");
	}
	else 								//parent process
	{
		wait(NULL);						//parent will wait for the child to finish
		exit(0);
    // Parent process
    //wpid = waitpid(pid, &status, WUNTRACED);
	}
}

// main function with command line arguments
int main(int argc,char* argv[]){
	if(argc == 1){
		// Interactive mode
		int i,j;							//declaring i and j variables for "for" loop
		char input[128];					//input string 
		//char history_buff[128];
		char *string[256];					//string for tokenize
		char prompt[128] = "prompt> ";		//char prommpt to prompt the user
		char *newDir;						// new directory variable for cd
		char *newPrompt;					// variable to store new prompt
		char delimit[]=";\n\t\r\v\f";		// all the delimiters that needs to be ignored
		int exit_flag = FALSE;				// flag to check for boolean
		
		printf("**** interactive mode ****\n");						
		printf("--> type \"prompt\" at any time to change prompt\n");
		
		FILE *history;							// file pointer to store all the commands
		
		history = fopen(".history", "w+");				//open file
		if (history == NULL) 
		{
			perror("Error opening file");
			return -1;
		}
		while(!exit_flag)									//always true
		{							// Interactive Mode loop
			i=0;					// Iterator for agrument access
			j=0;
			
			printf("%s",prompt);
			
			if(fgets(input, sizeof(input),stdin))				//reads the line from a command line argument
			{
				string[i]=strtok(input,delimit);			// Tokenizes first element puts in string
				while(string[i]!=NULL)
				{
					i++;
					string[i]=strtok(NULL,delimit);			// Puts next token in string
				}
				for(j=0;j<i;j++)
				{
					//fputs(string[j], history); 
					if((strstr(string[j],"quit")) != NULL || (strstr(string[j],"exit")) != NULL){		//"exit" or "quit" command
						exit_flag = TRUE;
					}
					else if(strstr(string[j],"cd") != NULL){
						newDir = strtok(input," ");					// newDir = "cd"
						newDir = strtok(NULL,"");					// newDir = "Path"
						chdir(newDir);
					}
					else if((strstr(string[j],"prompt")) != NULL){
						newPrompt = strtok(input," ");				// newPrompt = "prompt"
						newPrompt = strtok(NULL,"");    			// newPrompt = "desired string"
						strcpy(prompt,newPrompt);					// Copy over
						strcat(prompt," ");							// Add space
					}
				/*	else if((strstr(string[j],"history")) != NULL){
						printf("Were in history!\n");
						while(fgets(history_buff,128,(FILE*)history) != '\0'){	// Doesnt work right now
							printf("History: %s\n",history_buff);
						}
					}
				*/	else{
						//printf("String [%d] = %s\n",j,string[j]);
						launch(&string[j]);							//execute the string
					}
				}
				if(exit_flag){
					printf("\nGoodbye\n");							//if user ends the program, print goodbye
					fclose(history);
					exit(0);
				}
			}			
		}
		return 0;
	}
	else if (argc==2){										//batch mode
		printf("****Batch Mode****\n");
		int i = 0;											// i and j declared to used in for loop
		int j = 0;
		int exit_flag = FALSE;								//boolean flag
		char input[128];									//input string
		char *newDir;										// new directory
		const char delimit[]=";\n\t\r\v\f";					//delimit to ignore
		char *string[256];									// store strings in here that needs to be executed

		char *line;
	
		FILE *batch;										// file pointer
		batch = fopen(argv[1], "r");						//open file
		if (batch == NULL) 
		{
			perror("Error opening file");					//error checking
			return -1;
		}
		printf("--> file: %s\n", argv[1]);							//print which file is open
		while (fgets(input, 128, (FILE*)batch) != '\0' && !exit_flag) 			//read in the commands
		{
			i = 0;
			printf("\n\nbatch line> %s\n",input);
			line = strtok(input, delimit);							//seperate the commands using string token
			while (line != NULL) 
			{
				string[i] = line;
				i++;
				line = strtok(NULL, delimit);						//command that needs to be executed will be stored in here
			}
			for (j = 0; j < i; j++) 
			{
				if((strstr(string[j],"quit")) != NULL || (strstr(string[j],"exit")) != NULL){		//"quit" and "exit" command to end program
						exit_flag = TRUE;												//boolean flag
				}
				else if(strstr(string[j],"cd") != NULL){				// change directory string
					newDir = strtok(input," ");		// newDir = "cd"
					newDir = strtok(NULL,"");		// newDir = "Path"
					chdir(newDir);
				}
				else{
					launch(&string[j]);											//execute the command
				}
			}
		}
		fclose(batch);														//close the file
		printf("\n");
		return 0;
	}	
	else{	// error checking for arguments that are greater than 3
		printf("ERROR: Too many command line arguments\n");
		return 0;
	}
}
