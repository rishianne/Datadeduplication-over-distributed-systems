#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ifaddrs.h>
#include <signal.h>
#include <time.h> 
using namespace std;

//create a new socket inside the thread and connect it with the probe socket of client. The main socket which was the listner will send data to client listner socket. put a while loop in the thread. 


int i = -1;
int socket_list[3];
int thread_port = 10200;
int port_array[3];
char password[20] = "ESA";
struct demo
{
	string a;
	int b;
		
};

struct demo *d;
void *probe_connection_handler(void *);
//void struct_sender();

int main()
{
	 
	int sock_id,new_socket;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	
	// Main Socket which will be listening for connections.
	//Listening Socket creation
	sock_id = socket (AF_INET, SOCK_STREAM, 0);
	
	if (sock_id == -1)
	{
		printf("\nError: Opening socket\n");
		perror("");
		return 1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	//Socket address assignment
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(7734);
	

	//Bind socket with port
	if (bind(sock_id, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
	{
		printf("\nError: Binding not successful\n");
		perror("");		
		return 1;
	}
	printf("Bind successful\n");

	//Listening to connections
	if(listen(sock_id, 5) == -1)
	{
		printf("\nFailed to listen\n");
		return 1;
	}
	printf("Listening to port 7734\n"); // this line exec whenever a new client tries to communicate.

	//Accepting connections from all the clients.
	clilen = sizeof(cli_addr);

	int random =1;	
	
	while(1)
	{
		
		new_socket = accept(sock_id, (struct sockaddr *) &cli_addr, &clilen);
		//printf("\n%s",str2);	

		if (new_socket<0)
		{
			perror ("accept failed");
			return 1;
		}

		//Thread creation
		pthread_t p1;

		if (pthread_create(&p1, NULL, probe_connection_handler, (void*) &new_socket) < 0)
		{
			perror("Error: Could not create thread");
			return 1;
		}
		printf ("\nHandler assigned");	
		
		
	
			
		
	}
}


void *probe_connection_handler(void *sock_desc_v)
{	
	int sock_number;	
	int sock_desc = *(int *)sock_desc_v;
	int sock,new_socket_t;
	if (sock_desc_v==NULL)
	{
		printf("Error: Thread not assigned");
		//exit (0);
	}
	
	//password check

	char* str2p = (char *)malloc(sizeof (char)*100);
	memset(str2p, 0, 100);
	strcpy(str2p,"Please enter the Password");
	char* reply_str2p = (char *)malloc(sizeof (char)*100);
	memset(reply_str2p, 0, 100);
	char* freply_str2p = (char *)malloc(sizeof (char)*100);
	memset(freply_str2p, 0, 100);
	char* str3p = (char *)malloc(sizeof (char)*100);
	memset(str3p, 0, 100);
	strcpy(str3p,"Correct Passwordsss");
	char* ftr3p = (char *)malloc(sizeof (char)*100);
	memset(ftr3p, 0, 100);
	strcpy(ftr3p,"Correct Password");

	if (write(sock_desc,str2p,strlen(str2p)) < 0)
	{
		printf("Error: Send failed");
		return 0;
	}		

	if(read(sock_desc, reply_str2p, 100) < 0)
	{
		printf("Error: Received failed");
		return 0;
	}	
	if(strcmp(password,reply_str2p)!=0)
	{
		char* str21pp = (char *)malloc(sizeof (char)*100);
		memset(str21pp, 0, 100);
		strcpy(str21pp,"Wrong Password");
		if (write(sock_desc,str21pp,strlen(str21pp)) < 0)
		{
			printf("Error: Send failed");
			return 0;
		}
		close(sock_desc);
		printf("\nWrong Password");
		fflush(stdout);		
	}
	
	if (write(sock_desc,ftr3p,strlen(ftr3p)) < 0)
	{
		printf("Error: Send failed");
		return 0;
	}	
	
	if(read(sock_desc, freply_str2p, 100) < 0)
	{
		printf("Error: Received failed");
		return 0;
	}
	//printf("\nWaste received\n");
	socket_list[++i] = sock_desc;	// global variable //
	printf ("\nConnection Accepted\n");
	socklen_t server_len_t,client_len;
	struct sockaddr_in serv_addr_t,client_addr_t;
	
	sock = socket (AF_INET, SOCK_STREAM, 0);
	
	if (sock == -1)
	{
		printf("\nError: Opening socket\n");
		perror("");
		return 0;
	}

	memset(&serv_addr_t, '0', sizeof(serv_addr_t));

	//Socket address assignment
	serv_addr_t.sin_family = AF_INET;
	serv_addr_t.sin_addr.s_addr = INADDR_ANY;
	serv_addr_t.sin_port = htons(thread_port);
	
	port_array[i] = thread_port; 
	char *str;
	str = (char*)malloc(sizeof(char)*25);
	sprintf(str,"%d",thread_port);
	if (write(sock_desc,str,strlen(str)) < 0)
	{
		printf("Error: Send failed");
		return 0;
	}
	printf("\nPort No Sent\n");	

	//Bind socket with port
	if (bind(sock, (struct sockaddr *)&serv_addr_t, sizeof(serv_addr_t)) == -1)
	{
		printf("\nError: Binding not successful inside socket\n");
		perror("");		
		return 0;
	}
	printf("Bind successful\n");


	//Listening to connections
	if(listen(sock, 5) == -1)
	{
		printf("\nFailed to listen\n");
		return 0;
	}
	printf("Listening to port"); // this line exec whenever a new client tries to communicate.
	
	//main socket sending new socket info to client
	/*char *str;
	str = (char*)malloc(sizeof(char)*25);
	sprintf(str,"%d",thread_port);
	if (write(sock_desc,str,strlen(str)) < 0)
	{
		printf("Error: Send failed");
		return 0;
	}*/

	//Accepting connections from client.
	client_len = sizeof(client_addr_t);
	new_socket_t = accept(sock, (struct sockaddr *) &client_addr_t, &client_len);
	
	//sending and receiving probes
	char* reply_str = (char *)malloc(sizeof (char)*20);
	memset(reply_str, 0, 20);
	char* str1 = (char *)malloc(sizeof (char)*30);
	memset(str1, 0, 30);
	strcpy(str1,"Are you active?");
	thread_port++;
	int n =5000;
	while(1)
	{
		//put timer here
		
		
		if (write(new_socket_t,str1,strlen(str1)) < 0)
		{
			printf("Error: Send failed");
			return 0;
		}
		printf("\nSent\n");
		/////////////receiving reply from client////////////////////
		
		if(read(new_socket_t, reply_str, 10) < 0)
		{
			printf("Error: Received failed");
			return 0;
		}
		printf("\nReceived\n");
		
		if (strcmp(reply_str,"\0") == 0)
		{	
			printf("Client Disconnected.");
			break;
			//exit (0);
		}
		else if (strcmp(reply_str,"I am done") == 0)
		{
			close(new_socket_t);
			close(sock_desc); 
			printf("Client Disconnected by interrupt");
						
			//close(stdout);
			printf("\nSocket Closed");
			fflush(stdout);	
			break;
		}
		else
			printf("Client Connected");
		n--;
	}
	
	
}

/*void struct_sender()
{	
	int option =1;
	char* buffer = (char*)malloc(sizeof(char)*256);
	memset(buffer,0,256);

	d = (struct demo*)malloc (sizeof(struct demo)); //creating struct object
	sprintf(buffer,"%d|%s|%d",&option,d->a,&(d->b));

	if (write(socket_list[/**enter number here**],buffer,strlen(buffer)) < 0)
/*	{
		printf("Error: Send failed");
		return;
	}
}*/





























