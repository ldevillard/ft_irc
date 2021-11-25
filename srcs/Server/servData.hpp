#pragma once

#include <exception>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include "../Utils/ServerException.hpp"

class	ServData {

	public :
		ServData();
		~ServData();
		int	connect();

	private :
		std::string	_msg;
		struct	sockaddr_in _address;
		char	_buffer[1024];
		size_t	_addlen;
		int	_port;
		int	_server_fd;
		int	_new_socket;
		int	_valread;
		int	_opt;
		void	init();
};
