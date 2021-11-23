#pragma once

#include "Ircserv.hpp"

struct	ServData {
	std::string	msg = "uwu les copains!\n";
	struct	sockaddr_in address;
	char	buffer[1024] = { 0 };
	size_t	addlen = sizeof(address);
	int	_port;
	int	server_fd;
	int	new_socket;
	int	valread;
	int	opt = 1;
	int	port = 8080;
};