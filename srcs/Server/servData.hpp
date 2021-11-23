#pragma once

#include "Ircserv.hpp"

struct	ServData {
	std::string	_msg;
	struct	sockaddr_in _address;
	char	_buffer[1024];
	size_t	_addlen;
	int	_port;
	int	_server_fd;
	int	_new_socket;
	int	_valread;
	int	_opt;
};