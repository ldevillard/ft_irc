#pragma once

#include <exception>
#include <string>
#include <iostream>
#include "../Utils/ServerException.hpp"
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include "servData.hpp"

class Server
{
	public:
		Server(std::string host, int port, std::string password);
		void	server_init(ServData &new_server);
		void	address_init(sockaddr_in &addr, int port);
		int		connect();
	private:
		std::string _host;
		std::string	_password;
		id_t		_port;
		ServData	*_data;
};