#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <unistd.h>

int	main()
{
	int socketClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	struct sockaddr_in addrClient;
	addrClient.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(30000);

	connect(socketClient, (const struct sockaddr *)&addrClient, sizeof(addrClient));
	std::cout << "connected" << std::endl;
 
	char str[19];
	recv(socketClient, str, 19, 0);
	std::cout << str << std::endl;

	std::cin >> str;
	send(socketClient, str, sizeof(str) + 1, 0);


	close(socketClient);
}
//