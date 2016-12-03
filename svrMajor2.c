#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>

void error(const char *msg);

int main(int argc, char *argv[]) {
	int sockfd, cli_size, portno, status;
	int c1, c2, maxfd, nread, trans;
	int t1 = 0;
	int t2 = 0;
	int total = 0;
	int i, j;
	fd_set fds;
	socklen_t clilen;
	char buff[1000];
	
	struct sockaddr_in serv_addr, cli_addr;
	
	if (argc < 2) {
		error("Error: no port number.");
	}
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("socket error\n");
		exit(EXIT_FAILURE);
	}
	
	
	memset(&serv_addr, '\0', sizeof(serv_addr));
	memset(buff, '\0', sizeof(buff));
	portno = atoi(argv[1]);
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	serv_addr.sin_port = htons(portno);
	
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
		error("Error on bind");
	}
	
	if (listen(sockfd, 10) < 0) {
		error("Error on listen");
	}
	
	if ((c1 = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen)) < 0) {
		error("Error: client 1 accept");
	}
	
	if ((c2 = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen)) < 0) {
		error("Error: client 2 accept");
	}
	
	maxfd = (c1 > c2 ? c1 : c2) + 1;
	
	while (1) {
		//clilen = sizeof(cli_addr);
		FD_ZERO(&fds);
		FD_SET(c1, &fds);
		FD_SET(c2, &fds);
		strcpy(buff, "Server connect successful");
		
		status = select(maxfd, &fds, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0);
		
		
		if (FD_ISSET(c1, &fds) && t1 < 20) {
			nread = recv(c1, buff, sizeof(buff), 0);
			if (nread < 1) {
				close(c1);
				close(c2);
				exit(0);
			}
			
			trans = atoi(buff);
			if (trans == 0) {
				printf("Client Disconnected\n");
				t1 = 1;
			}
			else {
				total += trans;
				printf("%d\n", total);
			}
		}
		if (FD_ISSET(c2, &fds) && t2 < 20) {
			nread = recv(c2, buff, sizeof(buff), 0);
			if (nread < 1) {
				close(c1);
				close(c2);
				exit(0);
			}
			
			trans = atoi(buff);
			if (trans == 0) {
				printf("Client Disconnected\n");
				t2 = 1;
			}
			else {
				total += trans;
				printf("%d\n", total);
			}
		}
		
		if (t1 == 1 && t2 == 1) {
			break;
		}
	}
	close(sockfd);
	return 0;
}
//error handling function
void error(const char *msg) {
	perror(msg);
	exit(1);
}
