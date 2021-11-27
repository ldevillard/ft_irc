#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>	   //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <csignal>
#include <map>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <poll.h>	  // Poll() (yeah no shit)
#include <iostream>
#include <cstring>
#include "../includes/ServerException.hpp"
#define SOCKET_BUFFER_SIZE 2048

int main(void)
{
	int opt = true;
	int master_socket;
	int addrlen;
	int new_socket;
	int client_socket[30]; // depends on max clients
	int max_clients = 30;
	int activity;
	int i;
	int valread;
	int sd;
	int max_sd;
	struct sockaddr_in address;

	char buffer[SOCKET_BUFFER_SIZE];

	fd_set read_fds;

	std::string msg = "Yo les potes\n";

	// Create a master socket
	std::signal(SIGPIPE, SIG_IGN);
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
		// migh change later to poll
		activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);
		if ((activity < 0) && (errno != EINTR))
			throw ServerException::select();
		// If something happens on the master socket
		// then it's an incoming connection
		if (FD_ISSET(master_socket, &read_fds))
		{
			if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
				throw ServerException::receiving();
			// gives infos that we'll use in send and recv
			std::cout << "New connection : socket fd [" << new_socket << "] ip [" << inet_ntoa(address.sin_addr) << "] port [" << ntohs(address.sin_port) << "]" << std::endl;
			// sends a "greeting" message
			if (send(new_socket, msg.c_str(), msg.length(), 0) != (ssize_t)msg.length())
				throw ServerException::send();
			std::cout << "Welcome message sent" << std::endl;
			// add new socket to sockets array
			for (i = 0; i < max_clients; i++)
			{
				if (!client_socket[i])
				{
					client_socket[i] = new_socket;
					std::cout << "Adding to sockets array as : " << i << std::endl;
					break;
				}
			}
		}
		// Else it's some IO interaction on another socket
		for (i = 0; i < max_clients; i++)
		{
			sd = client_socket[i];
			if (FD_ISSET(sd, &read_fds))
			{
				std::map<int, std::string> save;
				std::string actualLine;
				save.insert(std::make_pair<int, std::string>(8080, ""));
				std::string &userSave = save.at(8080);
				bool read = true;

				valread = 1;
				bzero(buffer, SOCKET_BUFFER_SIZE);
				strncpy(buffer, userSave.c_str(), std::min((size_t)SOCKET_BUFFER_SIZE, userSave.length()));
				for (size_t i = 0; i < SOCKET_BUFFER_SIZE && userSave[i]; i++)
				{
					buffer[i] = userSave[0];
					userSave.erase(0, 1);
				}
				actualLine += std::string(buffer);
				while (read && !std::strchr(buffer, '\n') && !std::strchr(buffer, '\r'))
				{
					bzero(buffer, SOCKET_BUFFER_SIZE);
					valread = recv(sd, buffer, SOCKET_BUFFER_SIZE, 0);
					if (buffer[0] == '\n' && buffer[1] == 0)
						buffer[0] = 0;
					std::cout << valread << std::endl;
					if (valread == -1)
					{
						std::cerr << "Error inr recv(). Quiting" << std::endl;
						userSave.erase();
						read = false;
						break;
					}
					else if (valread == 0)
					{
						std::cout << "Client disconnected!" << std::endl;
						userSave.erase();
						read = false;
						close (sd);
						client_socket[i] = 0;
						break;
					}
					actualLine += std::string(buffer);
				}
				if (!read)
					break;
				{
					while (actualLine.find("\r") != std::string::npos)
						actualLine.replace(actualLine.find("\r"), 1, "\n");
					while (actualLine.find("\n\n") != std::string::npos)
						actualLine.replace(actualLine.find("\n\n"), 2, "\n");
					std::size_t pos = actualLine.find("\n");
					if (pos != std::string::npos)
					{
						userSave += actualLine.substr(pos + 1, (actualLine.length()) - ((std::size_t)pos + 1));
						actualLine.erase(pos, (actualLine.length()) - ((std::size_t)pos));
					}
					else
						userSave.erase();
				}
				std::cout << "* line = " << actualLine << std::endl;
				send(sd, actualLine.c_str(), actualLine.length(), 0);
			}
		}
	}
	return (0);
}