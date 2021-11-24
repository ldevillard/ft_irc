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
		Server(std::string host, int port, std::string password);
		ServData	_data;
	private:
		std::string _host;
		id_t		_port;
		std::string	_password;
