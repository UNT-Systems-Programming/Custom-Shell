// Group 12
// CSCE 3600 Fall 2016
// 12/7/2016
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <pthread.h>

int i;              		// for loops
int clientCount = 0;		// keep track of clients
int activeClients = 0;  	// live tally of connected clients
int connectedOnceFlag = 0;  // prevents server from shutting down until at least one client has connected and disconnected
int ticketNumber= 0;
int total = 0;
int prefSocket = 11345;
int connection = 0;
int clientDC = 0;
pthread_t prod_tid;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void clientError(const char *msg){
	perror(msg);
	prefSocket = prefSocket + 1;
}
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


void *listenForOtherClient(){
	int socket_desc , client_sock , c , *new_sock;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, client;
     int n;
	 
     socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	 
     if (socket_desc < 0) 
        error("ERROR opening socket");
	while (connection == 0){
     bzero((char *) &serv_addr, sizeof(serv_addr));
     client_sock = prefSocket;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(client_sock);
     if (bind(socket_desc, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) {
              clientError("ERROR on binding, trying next socket");
			  }else{
				  connection = 1;
			  }
}
        //Listen
     listen(socket_desc , 3);    
     c = sizeof(struct sockaddr_in);
		 
    // Wait for client to connect
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
		clientCount = clientCount + 1;
        activeClients = activeClients +1;
        
		printf("CLIENT[%d%s",clientCount,"] Connected...\n");
        connectedOnceFlag = 1;
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
    }
}

void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char message[20000], client_message[2000]; // buffer for client messages
	int loopCheck = 0;  // enable the while loop
	bzero(client_message,2000);
    
    while(loopCheck == 0){  // Continue operations until client exits. 
	bzero(message,20000);

        sprintf(message,"\nCurrent Total %d%s",total,"] \n");
        write(sock , message , strlen(message));
        bzero(message,20000);
        
    //Receive a message from client and execute the proper command
   if (read_size = recv(sock , client_message , 2000 , 0) > 0 )
    {
		total = total + atoi(client_message);
		bzero(client_message,2000);
				sprintf(client_message,"Other Client Disconnected, transfering their total...\n");
    }
    
    else if(read_size == -1)
    {
        perror("[SERVER]: recv failed");
    }
	}
}

int main(int argc, char *argv[])
{
	pthread_create(&prod_tid, NULL, listenForOtherClient, (void*) NULL);

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
	int cpu_load;
	
    char buffer[200];
	char message[200];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname("192.168.122.112");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
	
	while(1){
		sleep(2);
	    cpu_load = getCpuUsage();
		if(cpu_load > atoi(argv[5])){
			printf("CPU utilization: %d Threshold %d exceeded\n",cpu_load, atoi(argv[5]));
			clientDC = 1;
			// Pass total to to other client 
			exit(1);
		}
		//Receive a reply from the server
        if( recv(sockfd , buffer , 200 , 0) < 0)
        {
            puts("server message retrieval failed");
			close(sockfd);
            exit(1);
        }

	
	printf("%s%s",buffer,"\n");
	bzero(buffer,200);
    bzero(message,200);	
	fgets(message,200,stdin);
	
    n = write(sockfd,message,strlen(message));
	if ((atoi(message)) == 0) {
		printf("Disconnecting...\n");
		break;
	}
	if(clientDC > 0){
		bzero(message,200);
		sprintf(message,"Adding Total From Disconnected Client\n");
		n = write(sockfd,message,strlen(message));
		bzero(message,200);
		sprintf(message,"%d%s",clientDC,"\n");	
		n = write(sockfd,message,strlen(message));
		bzero(message,200);
	}
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(message,200);
    
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",message);
    
	}
	
	
	close(sockfd);
    return 0;
}
