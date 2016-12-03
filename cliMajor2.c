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

int main(int argc, char *argv[]) {
	int sockfd, portno, n, count;
	int len, nready, nread, maxfd;
	struct sockaddr_in serv_addr;
	fd_set fds;
	int ticket[20];
	int i, j;
	int choice;
	int total = 0;
	
	char buff[1000];
	
	if (argc < 6) {
		error("Usage %s IP_Address svr_port cli1_port cli2_port cpu_%", argv[0]);
	}

	for (i = 0; i < 20; i++) {
		ticket[i] = 0;
	}
	
	portno = atoi(argv[1]);
	memset(buff, '0', sizeof(buff));
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket error\n");
		exit(EXIT_FAILURE);
	}
 
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
 
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("connect error\n");
		exit(EXIT_FAILURE);
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
	}
	return 0;
}

void error(const char *msg) {
	perror(msg);
	exit(1);
}
