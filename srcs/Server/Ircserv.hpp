#pragma once

#include <exception>
#include <string>
#include <iostream>
#include "../Utils/ServerException.hpp"
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include "ServData.hpp"

struct ServData;

class Server
{
	public:
		Server(std::string host, id_t port, std::string password);
		void	server_init(ServData &new_server);
		void	address_init(sockaddr_in &addr, int port);
		int		connect();
	private:
		std::string _host;
		id_t		_port;
		std::string	_password;
		ServData	*_data;
};