#pragma once

#include <exception>
#include <string>
#include <iostream>
#include "../Utils/ServerException.hpp"

class Server
{
	public:
		Server(std::string host, int port, std::string password);

	private:
		std::string _host;
		std::string _password;
		int _port;
};