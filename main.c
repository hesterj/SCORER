#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
//#include <cio_multiplexer.h>

#define MAX_BUFFER 1024
#define SERVER_PORT 5500

float ScoreFormulaRandomly(char *formula)
{
	float score = (float)rand()/(float)(RAND_MAX);
	//float coefficient = (float)rand()*strlen(formula);
	//score = score/coefficient;
	return score;
}

int main()
{
	int clientSocketDescriptor,socketDescriptor;

	struct sockaddr_in serverAddress,clientAddress;
	socklen_t clientLength;
	

	char recvBuffer[MAX_BUFFER];
	char sendBuffer[MAX_BUFFER];
	pid_t cpid;
	bzero(&serverAddress,sizeof(serverAddress));
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
	serverAddress.sin_port=htons(SERVER_PORT);
	socketDescriptor=socket(AF_INET,SOCK_STREAM,0);
	bind(socketDescriptor,(struct sockaddr*)&serverAddress,sizeof(serverAddress));
	listen(socketDescriptor,5);
	printf("%s\n","Server is running ...");
	clientSocketDescriptor=accept(socketDescriptor,(struct sockaddr*)&clientAddress,&clientLength);
	printf("Connection accepted.  Scoring formulas...\n");
	
	float score;
	long formulascounted = 0;

	receiving:
	printf("\r%ld",formulascounted);
	fflush(stdout);
	while(1)
	{
		bzero(&recvBuffer,sizeof(recvBuffer));
		ssize_t input = recv(clientSocketDescriptor,recvBuffer,sizeof(recvBuffer),0);
		if (input > 0)
		{
			score = ScoreFormulaRandomly(recvBuffer);
			formulascounted++;
			break;
		}
	}
	
	while(1)
	{
		bzero(&sendBuffer,sizeof(sendBuffer));
		//printf("\nType a message here ...  ");
		//fgets(sendBuffer,10000,stdin);
		sprintf(sendBuffer,"%f",score);
		ssize_t output = send(clientSocketDescriptor,sendBuffer,strlen(sendBuffer)+1,0);
		if (output > 0)
		{
			goto receiving;
		}
	}
		
	return 0;
}
