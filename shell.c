/* Group: William Wolfard, Clay Brant, Hansaj Patel, Jay Bishop 
Course: CSCE 3600.003
Due Date: Monday, November 14
Description:  Makes a shell in C 
Notes: 
*/
#include <stdio.h>	//printf
#include <stdlib.h>	//system
#include <string.h> //strcat
#include <unistd.h>

#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
int main(int argc,char* argv[]){
	if(argc == 1){
		// Interactive mode
		int i,j,status;
		int READ = 0; 					
		int WRITE = 1;
		int fileDesc[2];
		
		char input[128];
		char *string[256];
		char delimit[]=";\n\t\r\v\f";
		char * buffer[128];
		
		
		while(1)
		{

									// Interactive Mode loop
			i=0;					// Iterator for agrument access
			j=0;
			printf("prompt> ");
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
					printf("String [%d]=%s\n",j,string[j]);
					buffer[j] = string[j];				
				}
			}
				if (pipe(fileDesc) == -1)
				{ 
					perror("Pipe failed");
				}
				
				int x = 0;
				switch (fork())
				{
					case -1: // Error Checking
							perror("Error creating fork for command");
							exit(1);
			
					case 0: // First child redirects stdout
							dup2(fileDesc[WRITE], fileno(stdout));  
							close(fileDesc[READ]);
							while(string[x+1] != ' ')
							{
								execvp(string[x], &string[x]);
								x++;
							}
										
					default: // Parent waits for process to terminate
							wait();			
							dup2(fileDesc[READ], fileno(stdin));
							close(fileDesc[WRITE]);
				}// switch	
				
				

				
				switch( fork() )
				{
					case -1:// Error Checking
							perror("Error creating fork for final command"); 
					
					case 0: // Second child redirects stdin
							dup2(fileDesc[WRITE], fileno(stdout)); 
							close(fileDesc[READ]);
							execvp(string[x], &string[x]);

					default: // Parent waits for process to terminate
							wait();
							dup2(fileDesc[READ], fileno(stdin));
							close(fileDesc[WRITE]);
				}
		}
		printf("\n");
		return 0;
	}
	else if (argc==2){
		// Batch mode
		return 0;
	}	
	else{	// argc > 3
		printf("ERROR: Too many command line arguments\n");
		return 0;
	}
}