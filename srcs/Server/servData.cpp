#include "servData.hpp"
#include <csignal>

ServData::~ServData()
{
}

ServData::ServData() : _msg("Salut les gars\n"), _addlen(sizeof(_address)), _opt(1), _port(8080)
{
	init();
}

void ServData::init()
{
	// std::signal(SIGPIPE, SIG_IGN);
	if (!(_server_fd = socket(AF_INET, SOCK_STREAM, 0)))
		throw ServerException::socket_creation();
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_opt, sizeof(_opt)))
		throw ServerException::socket_config();
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(_port);
	if (bind(_server_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
		throw ServerException::binding();
	if (listen(_server_fd, 3) < 0)
		throw ServerException::listening();
	if ((_new_socket = accept(_server_fd, (struct sockaddr *)&_address, (socklen_t *)&_addlen)) < 0)
		throw ServerException::receiving();
	std::cout << "Init done" << std::endl;
}


int	ServData::connect()
{
	for (;;)
	{
		bzero(_buffer, sizeof(_buffer));
		_valread = recv(_new_socket, _buffer, sizeof(_buffer), 0);
		if (_valread > 0)
		{
			std::cout << _buffer << std::endl;
			bzero(_buffer, sizeof(_buffer));
			send(_new_socket, _buffer, sizeof(_buffer), 0);
			// send(_new_socket, _msg.c_str(), _msg.length(), 0);
			// recv(_new_socket, _buffer, sizeof(_buffer), 0);
			std::cout << "Message sent!" << std::endl;
		}
	}
	close(_new_socket);
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
