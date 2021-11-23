#pragma once

#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>

struct server
{
	int	server_fd;
	int	new_socket;
	int	valread;
	int	opt = 1;
	int	port = 8080;
	struct	sockaddr_in address;
	char	buffer[1024] = { 0 };
	size_t	addlen = sizeof(address);
	std::string	msg = "uwu les copains!\n";
};