#pragma once

#include "Ircserv.hpp"

struct	ServData
{
	ServData(); //Init values

	std::string	msg;
	struct	sockaddr_in address;
	char	buffer[1024];
	size_t	addlen;
	int	_port;
	int	server_fd;
	int	new_socket;
	int	valread;
	int	opt;
	int	port;
};