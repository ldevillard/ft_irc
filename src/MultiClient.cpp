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
#include <poll.h> // Poll() (yeah no shit)
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
	while (true)
	{
		// reset the socket set
		FD_ZERO(&read_fds);
		// add master to the set
		FD_SET(master_socket, &read_fds);
		max_sd = master_socket;
		// add child sockets to set
		for (i = 0; i < max_clients; i++)
		{
			sd = client_socket[i];
			// if valid socket you can add it to the read list
			if (sd > 0)
				FD_SET(sd, &read_fds);
			// highest fd number, needed for poll()
			if (sd > max_sd)
				max_sd = sd;
		}

		// Wait for an activity on one of the socket
		// Timeout set to null so infinite waiting
		activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);
		if ((activity < 0) && (errno != EINTR))
			std::cout << "Select error!" << std::endl;
		
		// If something happens on the master socket
		// then it's an incoming connection
		if (FD_ISSET(master_socket, &read_fds))
		{
			if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
				throw ServerException::receiving();
			// gives infos that we'll use in send and recv
			std::cout << "New connection : socket fd [" << new_socket << "] ip [" << inet_ntoa(address.sin_addr) << "] port [" << ntohs(address.sin_port) << "]" << std::endl;
			//sends a "greeting" message
			if (send(new_socket, msg.c_str(), msg.length(), 0) != msg.length())
				std::cout << "Error during send!" << std::endl;
			std::cout << "Welcome message sent" << std::endl;
			//add new socket to sockets array
			for (i = 0; i < max_clients; i++)
			{
				if (!client_socket[i])
				{
					client_socket[i] = new_socket;
					std::cout << "Adding to sockets array as : " << i << std::endl;
					break ;
				}
			}
		}
		// Else it's some IO interaction on another socket
		for (i = 0; i < max_clients; i++)
		{
			sd = client_socket[i];
			if (FD_ISSET(sd, &read_fds))
			{
				// check if it's a disconnection and read the inc msg
				if ((valread = read(sd, buffer, 1024)) == 0)
				{
					// We might be able to catch some info on the client disconnected if needed
					std::cout << "Client disconnected!" << std::endl;
					// close the socked and mark it available
					close (sd);
					client_socket[i] = 0;
				}
				// Echoes the inc msg
				else
				{
					buffer[valread] = '\0';
					send(sd, buffer, strlen(buffer), 0);
				}
			}
		}
	}
	return (0);
}