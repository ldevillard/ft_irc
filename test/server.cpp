#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <pthread.h>

void *func(void *arg)
{
	int socket = *(int*)arg;

	char str[] = "What's your name ?";
	send(socket, str, sizeof(str) + 1, 0);
	recv(socket, str, 25, 0);

	std::string msg(str);

	std::cout << "Bienvenu sur Teamspeak 4 : " + msg << std::endl;

	close(socket);
	pthread_exit(NULL);
}

int	main()
{
	pthread_t clientThread;

	int socketServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	struct sockaddr_in addrServer;
	addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(30000);

	bind(socketServer, (const struct sockaddr *)&addrServer, sizeof(addrServer));
	std::cout << "bind : " << socketServer << std::endl;

	listen(socketServer, 5);
	std::cout << "listen" << std::endl;

	for (int i = 0; i < 3; i++)
	{
		struct sockaddr_in addrClient;
		socklen_t csize = sizeof(addrClient);
		int socketClient = accept(socketServer, (struct sockaddr *)&addrClient, &csize);
		std::cout << "accept" << std::endl;

		std::cout << "client : " << socketClient << std::endl;

		pthread_create(&clientThread, NULL, func, &socketClient);
	}

	pthread_join(clientThread, NULL);

	std::cout << "close" << std::endl;
	close(socketServer);
}