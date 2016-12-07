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
void *connection_handler(void *);
int clientCount = 0;
int sockfd, cli_size, portno, status;
int client_sock, c, c1, c2, maxfd, nread, trans;
int t1 = 0;
int t2 = 0;
int total = 0;
int done = 0;
int i, j;
fd_set fds;
socklen_t clilen;
char buff[1000];


int main(int argc, char *argv[]) {

		// keep track of clients

	int *new_sock;
	
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
	
	c = sizeof(struct sockaddr_in);
	while ((client_sock = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*)&c))) {
		if (client_sock < 0) {
			error("Error: Unable to accept client");
		}
		clientCount = clientCount + 1;
        
		printf("CLIENT[%d%s",clientCount,"] Connection Established\n");
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

	}
			
	close(sockfd);
	return 0;
}
//error handling function
void error(const char *msg) {
	perror(msg);
}

			void *connection_handler(void *socket_desc){

			int uc = clientCount; // unique client
			//clilen = sizeof(cli_addr);
			FD_ZERO(&fds);
			FD_SET(uc, &fds);
			if (uc > maxfd){
				maxfd = uc;
			}
			//FD_SET(c2, &fds);
			strcpy(buff, "Server connect successful");
			
			status = select(maxfd, &fds, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0);
			
		
			if (FD_ISSET(uc, &fds)) {
				nread = recv(uc, buff, sizeof(buff), 0);
				if (nread < 1) {
					close(uc);
				}
				else {
					trans = atoi(buff);
					if (trans == 0) {
						printf("Client Disconnected\n");
						trans = 1;
						done--;
					}
					else {
						total += trans;
						printf("%d\n", total);
						
					}
				}
			}
		}
