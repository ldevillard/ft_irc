#include <stdio.h> 
#include <string.h>   //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>   //close 
#include <arpa/inet.h>    //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <iostream>
#include <cstring>
#include "../includes/ServerException.hpp"

int main(void)
{
	int	opt = true;
	int	master_socket;
	int	addrlen;
	int	new_socket;
	int	client_socket[30]; // depends on max clients
	int	max_clients = 30;
	int	activity;
	int	i;
	int	valread;
	int	sd;
	int	max_sd;
	struct	sockaddr_in address;

	char buffer[1024];

	fd_set read_fds;

	std::string	msg = "Yo les potes\n";

	// Create a master socket
	for (int i = 0; i < max_clients; i++)
		client_socket[i] = 0;
	if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		throw ServerException::socket_creation();
	// It should work without this part, it just allows the socket to handle multiple connections
	// it seems to be a good behavior to have that's all
	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw ServerException::socket_config();
	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
		throw ServerException::socket_config();
	// Define the kind of socket created
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(8080);
	// Bind the socket to the desired port
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw ServerException::binding();
	if (listen(master_socket, 3) < 0)
		throw ServerException::listening();
	addrlen = sizeof(address);
	std::cout << "Waiting for connections..." << std::endl;
}