Group Members: William Wolfard, Clay Brant, Jay Bishop, Hansaj Patel

Design Overview:
	The first thing this program does is it checks for the total number of command line arguments.
		If the argument equals 1, the program is in interactive mode. The program prompts the user for a user command. The user command is stored as a string.
	The program then executed the command by calling the launch funtion. If a user inputs multiple commands using semi-colon, string token is used
	to tokenize the command. semi-colons will be ignored and all the UNIX/LINUX command will be executed. If the user types in "exit" or "quit", the
	boolean flag will become true and the program will terminate. If the user types "cd " without writng the directory name, the command will be ignored.
	
		If the argument equals 2, the program is in batch mode. The program then opens a file and reads the command listed in the file.
	The program then executed the command by calling the launch funtion. If there are multiple commands seperated by semi-colon. string token is used
	to tokenize the command. semi-colons will be ignored and all the UNIX/LINUX command will be executed. If the file containts "exit" or "quit", the
	boolean flag will become true and the program will terminate. 
	
Complete Specification:
	The program can handle all the UNIX/LINUX commands. If there are multiple commands seperated by semi-colon, the strtok will break the command
	into series of tokens using the delimiter. These tokes are the ones that will be passed to the launch function and they will be executed.
	If the shell recieves a line that have no commands between the semi-colon, it will simply be ignored and the user will be prompted again.
	
Known Bugs or Problems:
	If the user types in "prompt" after "prompt >", the program will terminate.
	The user must type "prompt this is my new prompt$" after "prompt >" in order to change the prompt
