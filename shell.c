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

int launch(char **args)
{
  pid_t pid, wpid;
  int status;
  int READ = 0; 					
  int WRITE = 1;
  int fileDesc[2];
  
  pid = fork();
  if (pid == 0) {
    // Child process
    system(args);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int main(int argc,char* argv[]){
	if(argc == 1){
		// Interactive mode
		int i,j,status;		
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
					printf("String [%d] = %s\n",j,string[j]);
					launch(&string[j]);
				}
			}			
		}
		printf("\n");
		return 0;
	}
	else if (argc==2){
		// Batch mode
		int i = 0;
		int j = 0;

		char input[128];
		const char delimit[]=";\n\t\r\v\f";

		char *line;

		FILE *batch;
		batch = fopen(argv[1], "r");
		if (batch == NULL) 
		{
			perror("Error opening file");
			return -1;
		}

		while (fgets(input, 128, (FILE*)batch) != '\0') 
		{

			i = 0;
			line = strtok(input, delimit);
			while (line != NULL) 
			{
				string[i] = line;
				i++;
				line = strtok(NULL, delimit);
			}
			for (j = 0; j < i; j++) 
			{
				launch(&string[j]);
			}
		}
		fclose(batch);
		return 0;
	}	
	else{	// argc > 3
		printf("ERROR: Too many command line arguments\n");
		return 0;
	}
}
