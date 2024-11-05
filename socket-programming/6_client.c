#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5400
#define CLIENT_IP "127.0.0.1"
#define CLIENT_PORT 5401
int main(){
	struct sockaddr_in client,server;
	int sd;
	char str[512];
	bzero((char *)&client,sizeof(client));
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=inet_addr(CLIENT_IP);
	client.sin_port=htons(CLIENT_PORT);
	bzero((char *)&server,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr(SERVER_IP);
	server.sin_port=htons(SERVER_PORT);
	sd=socket(AF_INET,SOCK_STREAM,0);
	connect(sd,(struct sockaddr *)&server,sizeof(server));
//	listen(sd,5);
	do{
		printf("\nEnter a message: ");
		scanf("%s",str);
		send(sd,str,strlen(str)+1,0);
	//	memset(str,0x0,512);
	//	recv(sd,str,512,0);
	//	printf("\nReceived message: %s",str);
	}while(strcmp(str,"stop"));
	close(sd);
	return 0;
}

