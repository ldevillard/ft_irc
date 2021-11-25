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
#include "servData.hpp"

class Server
{
	public:
		Server(std::string host, id_t port, std::string password);
		
		void connect();
	private:
		ServData	_data;
		std::string _host;
		id_t		_port;
		std::string	_password;
};