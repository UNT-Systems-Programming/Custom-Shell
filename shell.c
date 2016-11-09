/* Group: William Wolfard, Clay Brandt, Hansaj Patel, Brad uuh 
Course: CSCE 3600.003
Due Date: Monday, November 14
Description:  Makes a shell in C 
Notes: 
*/
#include <stdio.h>	//printf
#include <stdlib.h>	//system
#include <string.h> //strcat

int main(int argc,char* argv[]){
	if(argc == 1){
		// Interactive mode
		int i,j;
		char input[128];
		char *string[256];
		char delimit[]=";\n\t\r\v\f";
		while(1){				// Interactive Mode loop
			i=0;					// Iterator for agrument access
			j=0;
			printf("prompt> ");
			if(fgets(input, sizeof(input),stdin)){
				string[i]=strtok(input,delimit);			// Tokenizes first element puts in string
				while(string[i]!=NULL){
					i++;
					string[i]=strtok(NULL,delimit);			// Puts next token in string
				}
				for(j=0;j<i;j++)
					printf("String [%d]=%s\n",j,string[j]);
				
				// Do stuff 
				// fork();
			}
			
		}		
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