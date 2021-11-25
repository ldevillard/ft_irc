#include <csignal>
#include <netdb.h>
#include <string>
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
	// std::signal(SIGPIPE, SIG_IGN);
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

int ServData::connect()
{
	init();
	User user;
	for (;;)
	{
		bzero(_buffer, sizeof(_buffer));
		_valread = recv(_client_socket, _buffer, sizeof(_buffer), 0);
		if (_valread == -1)
		{
			std::cerr << "Error inr recv(). Quiting" << std::endl;
			break;
		}
		if (_valread == 0)
		{
			std::cout << "Client disconnected!" << std::endl;
			break;
		}
		if (_valread > 0)
		{
			int i = user.recoverData(_buffer);
			if (i) // Parsing data if needed
			{
				if (i == 1)
					std::cout << "Connection refused: username invalid!" << std::endl;
				else if (i == 2)
					std::cout << "Connection refused: nickname invalid!" << std::endl;

				break;
			}

			std::cout << _buffer;
			bzero(_buffer, sizeof(_buffer));
			send(_client_socket, _buffer, _valread + 1, 0);
			// send(_new_socket, _msg.c_str(), _msg.length(), 0);
			// recv(_new_socket, _buffer, sizeof(_buffer), 0);
		}

		std::cout << "Nickname : " + user.getNick() + " and Username : " + user.getUser() << std::endl;
	}
	close(_server_fd);
	// while (_new_socket > 0)
	// {
	// 	bzero(_buffer, sizeof(_buffer));
	// 	// send(_new_socket, _buffer, sizeof(_buffer), 0);
	// 	// send(_new_socket, _msg.c_str(), _msg.length(), 0);
	// 	recv(_new_socket, _buffer, sizeof(_buffer), 0);
	// 	std::cout << _buffer << std::endl;
	// }
	return (0);
}
