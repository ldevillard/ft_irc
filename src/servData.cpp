#include <csignal>
#include <netdb.h>
#include <string>
#include <cstring>
#include <map>
#include "../includes/servData.hpp"
#include "../includes/User.hpp"

ServData::~ServData()
{
}

ServData::ServData() : _msg("Salut les gars\n"), _addlen(sizeof(_address)), _port(8080), _opt(1)
{
}

ServData::ServData(id_t port) : _msg("Salut les gars\n"), _addlen(sizeof(_address)), _port(port), _opt(1)
{
	std::cout << "Port : " << _port << std::endl;
}

void ServData::init()
{
	std::signal(SIGPIPE, SIG_IGN);
	if (!(_server_fd = socket(AF_INET, SOCK_STREAM, 0)))
		throw ServerException::socket_creation();
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt)))
		throw ServerException::socket_config();
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEPORT, &_opt, sizeof(_opt)))
		throw ServerException::socket_config();
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(_port);
	if (bind(_server_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
		throw ServerException::binding();
	if (listen(_server_fd, 3) < 0)
		throw ServerException::listening();
	if ((_client_socket = accept(_server_fd, (struct sockaddr *)&_address, (socklen_t *)&_addlen)) < 0)
		throw ServerException::receiving();
	std::cout << "Init done" << std::endl;
	close(_server_fd);
}

void printBuffer(char buffer[SOCKET_BUFFER_SIZE])
{
	for (size_t i = 0; i < SOCKET_BUFFER_SIZE; i++)
	{
		if ((int)buffer[i] == 13)
			std::cout << "[" << (int)buffer[i] << "="
					  << "\\r"
					  << "]";
		else if ((int)buffer[i] == 10)

			std::cout << "[" << (int)buffer[i] << "="
					  << "\\n"
					  << "]";
		else
			std::cout << "[" << (int)buffer[i] << "=" << buffer[i] << "]";
	}
	std::cout << std::endl
			  << std::endl;
}

int ServData::connect()
{
	init();
	User user;
	std::map<int, std::string> save;

	int port = 1; // user socket port

	save.insert(std::make_pair<int, std::string>(port, ""));
	for (;;)
	{
		std::string actualLine;
		std::string &userSave = save.at(port);
		bool read = true;

		_valread = 1;
		bzero(_buffer, SOCKET_BUFFER_SIZE);
		strncpy(_buffer, userSave.c_str(), std::min((size_t)SOCKET_BUFFER_SIZE, userSave.length()));
		userSave.erase();
		if (_buffer[0] == '\n' && !_buffer[1])
			_buffer[0] = 0;
		actualLine += std::string(_buffer);
		while (read && !std::strchr(_buffer, '\n') && !std::strchr(_buffer, '\r'))
		{
			bzero(_buffer, SOCKET_BUFFER_SIZE);
			_valread = recv(_client_socket, _buffer, SOCKET_BUFFER_SIZE, 0);
			if (_valread == -1)
			{
				std::cerr << "Error inr recv(). Quiting" << std::endl;
				userSave.erase();
				read = false;
				break;
			}
			else if (_valread == 0)
			{
				std::cout << "Client disconnected!" << std::endl;
				userSave.erase();
				read = false;
				break;
			}
			if ((_buffer[0] == '\n' && !_buffer[1] && actualLine.empty()) ||
				(_buffer[0] == '\r' && _buffer[1] == '\n' && !_buffer[2] && actualLine.empty()))
				_buffer[0] = 0;
			actualLine += std::string(_buffer);
		}
		if (!read)
			break;
		{
			while (actualLine.find("\r") != std::string::npos)
				actualLine.replace(actualLine.find("\r"), 1, "\n");
			while (actualLine.find("\n\n") != std::string::npos)
				actualLine.replace(actualLine.find("\n\n"), 2, "\n");
			std::size_t pos = actualLine.find("\n");
			if (pos != std::string::npos)
			{
				userSave += actualLine.substr(pos + 1, (actualLine.length()) - ((std::size_t)pos + 1));
				actualLine.erase(pos, (actualLine.length()) - ((std::size_t)pos));
			}
			else
				userSave.erase();
		}
		std::cout << "* line = " << actualLine << std::endl;

		// bzero(_buffer, sizeof(_buffer));
		// _valread = recv(_client_socket, _buffer, sizeof(_buffer), 0);
		// if (_valread == -1)
		// {
		// 	std::cerr << "Error inr recv(). Quiting" << std::endl;
		// 	break;
		// }
		// if (_valread == 0)
		// {
		// 	std::cout << "Client disconnected!" << std::endl;
		// 	break;
		// }
		// if (_valread > 0)
		// {
		// 	if (std::strchr(_buffer, '\n'))
		// 	{
		// 	}

		// 	std::cout << "* buffer [" << _buffer << "]" << std::endl;

		// int i = user.recoverData(actualLine);
		// if (i) // Parsing data if needed
		// {
		// 	if (i == 1)
		// 		std::cout << "Connection refused: username invalid!" << std::endl;
		// 	else if (i == 2)
		// 		std::cout << "Connection refused: nickname invalid!" << std::endl;
		// 	break;
		// }

		// 	bzero(_buffer, sizeof(_buffer));
		send(_client_socket, actualLine.c_str(), actualLine.length(), 0);
		// }

		// std::cout << "Nickname : " + user.getNick() + " and Username : " + user.getUser() << std::endl;
	}
	close(_client_socket);
	return (0);
}
