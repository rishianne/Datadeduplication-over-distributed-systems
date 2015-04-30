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
#define ip_addr "127.0.0.1"
using namespace std;

void *probe_connection_handler(void *);
struct demo
{
	string a;
	int b;
		
};

struct demo *d;
int data_id,probe_id,option;

void siginthandler(int param)
{
	printf("\nEntering signal interrupt\n");
	char *str3;
	str3 = (char *) malloc(sizeof(char )*512);
	memset(str3,0,512);
	strcpy(str3,"I am done");
			
	if (write(probe_id, str3, strlen(str3)) < 0)
	{
		printf("Error: Send failed");
		//return 1;
	}
	/*if(read(sock_id, recv_buffer, 1024) < 0)
	{
		printf("Error: Received failed");
		return;
	}
	printf("%s",recv_buffer);*/
	printf("\nConnection closed\n\n");
	
	//Closing connection with Centralized Server
	close(probe_id);
	
	//Closing Upload Server
	close(data_id);
	exit(0);
}

int main()
{
	signal(SIGINT, siginthandler);
	
	char* password_reply = (char *)malloc(sizeof (char)*200);
	memset(password_reply, 0, 200);
	char* password = (char *)malloc(sizeof (char)*200);
	memset(password, 0, 20);
	char* w_password = (char *)malloc(sizeof (char)*200);
	memset(w_password, 0, 20);
	char* str1 = (char *)malloc(sizeof (char)*50);
	memset(str1, 0, 50);
	
	struct sockaddr_in data_addr,probe_addr;
	strcpy(w_password,"arpit");
	if ((data_id = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{

		 printf("\n Error : Could not create socket \n");
     		 return 1;

	}
	
	data_addr.sin_family = AF_INET;
  	data_addr.sin_port = htons(7734);
  	data_addr.sin_addr.s_addr = inet_addr(ip_addr);

	if(connect(data_id, (struct sockaddr *)&data_addr, sizeof(data_addr)) < 0)
	{

		printf("\n Error : Connect Failed \n");
		return 1;
	}
	
	if(read(data_id,str1,100) < 0)
	{
		printf("Error: Received failed");
		return 0;
	}

	printf("\n%s\n",str1);
	scanf("%s",password);
	if (write(data_id,password,strlen(password)) < 0)
	{
		printf("Error: Send failed");
		return 0;
	}
	
	if(read(data_id,password_reply, 200) < 0)
	{
		printf("Error: Received failed");
		return 0;
	}
	printf("Password reply is %s\n",password_reply);
	if(strcmp(password_reply,"Wrong Password")==0)
	{
		printf("OOPS!\n");
		close(data_id);
		exit(0);
		fflush(stdout);	
	}
	if (write(data_id,w_password,strlen(w_password)) < 0)
	{
		printf("Error: Send failed");
		return 0;
	}
	//printf("\nWaste Sent\n");
	printf("Inside COrrectness");
	char* port_probe;
	port_probe = (char*)malloc(sizeof(char)*1024);
	memset(port_probe,0,1024);
	if(read(data_id,port_probe, 1024) < 0)
	{
		printf("Error: Received failed");
		return 0;
	}

	int port_proben = atoi(port_probe);
	printf("\n Port no: %d",port_proben);	

	if ((probe_id = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{

		 printf("\n Error : Could not create socket \n");
     		 return 1;

	}
	printf("\nSocket probe created\n");
	probe_addr.sin_family = AF_INET;
  	probe_addr.sin_port = htons(port_proben);
  	probe_addr.sin_addr.s_addr = inet_addr(ip_addr);

	if(connect(probe_id, (struct sockaddr *)&probe_addr, sizeof(probe_addr)) < 0)
	{

		printf("\n Error : Connect Failed \n");
		return 1;
	}

	//Thread creation
	pthread_t p1;

	if (pthread_create(&p1, NULL, probe_connection_handler, (void*) &probe_id) < 0)
	{
		perror("Error: Could not create thread");
		return 1;
	}
	//printf ("Handler assigned");
	char *data_str;
	data_str = (char *)malloc(sizeof (char)*1024);
	memset(data_str, 0, 1024);	

	while(1)
	{
		////////////////////////////////////////
		///       Code to accept from the main thread             ///
		int kk;
		if(read(data_id,(char *)&kk, sizeof(kk)) < 0)
		{
			printf("Error: Received failed");
			return 0;
		}

		printf("received kk is %d \n",kk);
		
		if (strcmp(data_str,"\0") == 0)
		{	
			printf("Null Entry");
			//exit (0);
		}
		
		int dummy;
		//sscanf(data_str,"%d|%s|%d",&option,d->a.c_str(),&(d->b));
		//d->b = dummy;
		if(option ==1)
		{
			//store the blocks
		}
		if(option ==2)
		{
			//read the blocks
		}
	}
	

		
return 0;
}

void *probe_connection_handler(void *sock_desc_v)
{	
	printf("\nInside Thread\n");
	int sock_desc = *(int *)sock_desc_v;
	//string str1("I am active");
	char str1[50];
	char *reply_str;
	reply_str = (char *)malloc(sizeof (char)*1024);
	memset(reply_str, 0, 1024);	
	int n =5000;
	while(1)
	{
		//put timer here
		
		if(read(sock_desc, reply_str, 255) < 0)
		{
			printf("Error: Received failed");
			return 0;
		}
		printf("\nReceived\n");

		if (strcmp(reply_str,"\0") == 0)
		{	
			printf("Null Entry");
			//exit (0);
		}
		
		printf("\nEnter the reply\n");
		scanf("%s",str1);
		if (write(sock_desc,str1,strlen(str1)) < 0)
		{
			printf("Error: Send failed");
			return 0;
		}
		
		printf("\nSent\n");
		n--;
		fflush(stdout);
	}
}

