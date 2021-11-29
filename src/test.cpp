#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#define PORT 8080

// int main(int argc, char const *argv[])
// {
//     int sock = 0, valread;
//     struct sockaddr_in serv_addr;
//     char *hello = "Hello from client";
//     char buffer[1024] = {0};
//     if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//     {
//         printf("\n Socket creation error \n");
//         return -1;
//     }

//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(PORT);

//     // Convert IPv4 and IPv6 addresses from text to binary form
//     if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
//     {
//         printf("\nInvalid address/ Address not supported \n");
//         return -1;
//     }

//     if (connect(sock, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
//     {
//         printf("\nConnection Failed \n");
//         return -1;
//     }
//     send(sock , hello , strlen(hello) , 0 );
//     printf("Hello message sent\n");
//     // valread = read( sock , buffer, 1024);
// 	recv(sock, hello, strlen(hello), 0);
//     printf("%s\n",buffer );
//     return 0;
// }

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <cstring>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
void func(int sockfd)
{
	char buff[MAX];
	int n;
	for (;;)
	{
		bzero(buff, sizeof(buff));
		std::cout << "Enter the string : " << std::endl;
		n = 0;
		while ((buff[n++] = getchar()) != '\n')
			;
		send(sockfd, buff, sizeof(buff), 0);
		// bzero(buff, sizeof(buff));
		// recv(sockfd, buff, sizeof(buff), 0);
		// std::cout << "From Server : " << buff << std::endl;
		// if (!strcmp(buff, "exit"))
		// {
		// 	std::cout << "Client Exit..." << std::endl;
		// 	break;
		// }
	}
}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		std::cout << "socket creation failed..." << std::endl;
		exit(0);
	}
	else
		std::cout << "Socket successfully created.." << std::endl;
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
	{
		std::cout << "connection with the server failed..." << std::endl;
		exit(0);
	}
	else
		std::cout << "connected to the server.." << std::endl;

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}