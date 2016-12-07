#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

void error(const char *msg);
int getCpuUsage(){
	float usage;
	int cpu_load;
	FILE *fp;
	
	fp = fopen("/proc/loadavg","r");
	if(fp == NULL){
		perror("ERROR");
	}
	else{
		fscanf(fp,"%f",&usage);		// Read usage from file
		fclose(fp);
		cpu_load = (int)(usage * 100);	// Typecast to int
		return cpu_load;
	}	
}
int main(int argc, char *argv[]) {
	int s, s2, portno, n;
	int disconnect, nready, nread, maxfd;
	struct sockaddr_in serv_addr;
	fd_set fds;
	int input;
	int serv = 0;
	int cpu_usage;
	
	char buff[1000];
	char *output;
	
	if (argc < 6) {
		error("Invalid Arguments");
	}
	
	portno = atoi(argv[2]);
	memset(buff, '0', sizeof(buff));
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket error\n");
		exit(EXIT_FAILURE);
	}
 
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
 
	if (connect(s, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		serv = 1;
		if (connect(s, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
			error("Connect");
		}
	}
 	
	if ((s2 = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("socket error\n");
		exit(EXIT_FAILURE);
	}
	
	portno = atoi(argv[3]);
	if (serv == 1) {
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
		serv_addr.sin_port = htons(portno);
		
		if (bind(s2, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
			error("Error on bind");
		}
		else {
			printf("Connection accepted\n");
		}
	}
	else {
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(portno);
		serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	}
	maxfd = s + 1;
	while (1) {
		
		/* Set up polling. */
		FD_ZERO(&fds);
		FD_SET(s,&fds);
		FD_SET(0,&fds);

		/* Wait for some input. */
		nready = select(maxfd, &fds, (fd_set *) 0, (fd_set *) 0,
						(struct timeval *) 0);
		
		if( FD_ISSET(s, &fds)) {
				cpu_usage = getCpuUsage();
				if(cpu_usage > atoi(argv[i]){
					// Disconnect 
				}   
				nread = recv(s, buff, sizeof(buff), 0);
				/* If error or eof, terminate. */
				if(nread < 1) {
						close(s);
						exit(0);
				}
				//disconnect = atoi(buff);
				/*if (disconnect == -1) {
					n = 3;
					printf("CLIENT Disconnected\n");
				}
				else {
					printf("SERVER Total:  %s\n", buff);
				}*/
		}

		if( FD_ISSET(0, &fds)) {
			cpu_usage = getCpuUsage();
			if(cpu_usage > atoi(argv[i]){
				// Disconnect 
			}  
			nread = read(0, buff, sizeof(buff));
			/* If error or eof, terminate. */
			//if(nread < 1) {
					//close(s);
					//exit(0);
			//}
			disconnect = atoi(buff);
			send( s, buff, nread, 0);
			if (disconnect == 0) {
				close(s);
			}
		}
				
	}
	close(s2);
	return 0;
}

void error(const char *msg) {
	perror(msg);
	exit(1);
}
