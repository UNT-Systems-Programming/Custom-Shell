/* Group: William Wolfard, Clay Brant, Hansaj Patel, Jay Bishop 
Course: CSCE 3600.003
Due Date: Monday, November 14
Description:  Makes a shell in C 
Notes: 
*/


#include <stdio.h>	//printf
#include <stdlib.h>	//system
#include <string.h> 	//strcat
#include <unistd.h>
#include<sys/types.h>
#include<sys/wait.h>	// wait(NULL)
#include<sys/mman.h>

#define FALSE 0
#define TRUE  1
enum{READ, WRITE};

void launch(char **args)
{
  //int status;
  pid_t pid;
  pid = fork();
  if (pid == 0) {
    // Child process
    system(*args);
	return;
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
	wait(NULL);
	exit(0);
    // Parent process
    //wpid = waitpid(pid, &status, WUNTRACED);
  }
}

int main(int argc,char* argv[]){
	if(argc == 1){
		// Interactive mode
		int i,j;		
		char input[128];
		char history_buff[128];
		char *string[256];
		char prompt[128] = "prompt> ";
		char *newDir;
		char *newPrompt;
		char delimit[]=";\n\t\r\v\f";
		int exit_flag = FALSE;
		printf("**** interactive mode ****\n");
		printf("--> type \"prompt\" at any time to change prompt\n");
		FILE *history;
		history = fopen(".history", "w+");
		if (history == NULL) 
		{
			perror("Error opening file");
			return -1;
		}
		while(!exit_flag)
		{							// Interactive Mode loop
			i=0;					// Iterator for agrument access
			j=0;
			printf("%s",prompt);
			if(fgets(input, sizeof(input),stdin))
			{
				string[i]=strtok(input,delimit);			// Tokenizes first element puts in string
				while(string[i]!=NULL)
				{
					i++;
					string[i]=strtok(NULL,delimit);			// Puts next token in string
				}
				for(j=0;j<i;j++)
				{
					fputs(string[j], history); 
					if((strstr(string[j],"quit")) != NULL || (strstr(string[j],"exit")) != NULL){
						exit_flag = TRUE;
					}
					else if(strstr(string[j],"cd") != NULL){
						newDir = strtok(input," ");		// newDir = "cd"
						newDir = strtok(NULL,"");		// newDir = "Path"
						chdir(newDir);
					}
					else if((strstr(string[j],"prompt")) != NULL){
						newPrompt = strtok(input," ");	// newPrompt = "prompt"
						newPrompt = strtok(NULL,"");    // newPrompt = "desired string"
						strcpy(prompt,newPrompt);		// Copy over
						strcat(prompt," ");				// Add space
					}
					else if((strstr(string[j],"history")) != NULL){
						printf("Were in history!\n");
						while(fgets(history_buff,128,(FILE*)history) != '\0'){	// Doesnt work right now
							printf("History: %s\n",history_buff);
						}
					}
					else{
						//printf("String [%d] = %s\n",j,string[j]);
						launch(&string[j]);
					}
				}
				if(exit_flag){
					printf("\nGoodbye\n");
					fclose(history);
					exit(0);
				}
			}			
		}
		return 0;
	}
	else if (argc==2){
		printf("****Batch Mode****\n");
		int i = 0;
		int j = 0;
		int exit_flag = FALSE;

		char input[128];
		char *newDir;
		const char delimit[]=";\n\t\r\v\f";
		char *string[256];

		char *line;

		FILE *batch;
		batch = fopen(argv[1], "r");
		if (batch == NULL) 
		{
			perror("Error opening file");
			return -1;
		}
		printf("--> file: %s\n", argv[1]);
		while (fgets(input, 128, (FILE*)batch) != '\0' && !exit_flag) 
		{
			i = 0;
			printf("batch line> %s\n\n",input);
			line = strtok(input, delimit);
			while (line != NULL) 
			{
				string[i] = line;
				i++;
				line = strtok(NULL, delimit);
			}
			for (j = 0; j < i; j++) 
			{
				if((strstr(string[j],"quit")) != NULL || (strstr(string[j],"exit")) != NULL){
						exit_flag = TRUE;
				}
				else if(strstr(string[j],"cd") != NULL){
					newDir = strtok(input," ");		// newDir = "cd"
					newDir = strtok(NULL,"");		// newDir = "Path"
					chdir(newDir);
				}
				else{
					launch(&string[j]);
				}
			}
		}
		fclose(batch);
		printf("\n");
		return 0;
	}	
	else{	// argc > 3
		printf("ERROR: Too many command line arguments\n");
		return 0;
	}
}
