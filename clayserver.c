#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
 
//thread function
void *connection_handler(void *);

int i;              		// for loops
int clientCount = 0;		// keep track of clients
int activeClients = 0;  	// live tally of connected clients
int connectedOnceFlag = 0;  // prevents server from shutting down until at least one client has connected and disconnected
int ticketNumber= 0;
int total = 0;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
 
 int addTotal() { // checks to see if ticket is sold or not
	total = total + 1;
 }
 

 
int main(int argc , char *argv[])
{
     int socket_desc , client_sock , c , *new_sock;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, client;
     int n;
	 
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
	 
     socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	 
     if (socket_desc < 0) 
        error("ERROR opening socket");
	
     bzero((char *) &serv_addr, sizeof(serv_addr));
     client_sock = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(client_sock);
     if (bind(socket_desc, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     
        //Listen
     listen(socket_desc , 3);    
     c = sizeof(struct sockaddr_in);
		
    printf("\n[SERVER]: Waiting for incoming connections...\n");
    
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
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("Couldn't create new thread");
            return 1;
        }
         
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
 

void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char message[20000], client_message[2000]; // buffer for client messages
	int uniqueClient = clientCount; // Assigns a name to the client to keep track
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
				sprintf(client_message,"\nClient[%d%s%s%d%s",uniqueClient," "," New Total: ",total,"\n");
				printf("\n[SERVER]: CLIENT %d%s%s%d%s",uniqueClient," "," New Total: ",total,"\n");
				write(sock , client_message , strlen(client_message));

    }
     
    if(read_size == 0)
    {
		//Notify that a client has disconnected, if there aren't any clients left - shut down the server.
		printf("\n[SERVER]: CLIENT[%d%s",uniqueClient,"] Disconnected...\n");
		loopCheck = 1;
        activeClients = activeClients -1;
        if(activeClients == 0){
         printf("\n[SERVER]: All clients have disconnected. Shutting down...\n");
         close(sock);
         exit(0); 
        }
    }
    else if(read_size == -1)
    {
        perror("[SERVER]: recv failed");
		loopCheck = 1;
    }
	}      
   free(socket_desc);    
}
