#include "Ircserv.hpp"

Server::Server(std::string host, int port, std::string password) : _host(host), _port(port), _password(password)
{
}

void	Server::address_init()
{
	_data.address.sin_family = AF_INET;
	_data.address.sin_addr.s_addr = INADDR_ANY;
	_data.address.sin_port = htons(_port);
}

void	Server::server_init()
{
	if (!(_data.server_fd = socket(AF_INET, SOCK_STREAM, 0)))
		throw ServerException::socket_creation();
	if (setsockopt(_data.server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_data.opt, sizeof(_data.opt)))
		throw ServerException::socket_config();
	address_init();
	if (bind(_data.server_fd, (struct sockaddr *)&_data.address, sizeof(_data.address)) < 0)
		throw ServerException::binding();
	if (listen(_data.server_fd, 3) < 0)
		throw ServerException::listening();
	if ((_data.new_socket = accept(_data.server_fd, (struct sockaddr *)&_data.address, (socklen_t *)&_data.addlen)) < 0)
		throw ServerException::receiving();
}

int	Server::connect()
{
	try
	{
		server_init();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
	_data.valread = read(_data.new_socket, _data.buffer, 1024);
	std::cout << _data.buffer << std::endl;
	send(_data.new_socket, _data.msg.c_str(), _data.msg.length(), 0);
	std::cout << "Message sent!" << std::endl;
	return (0);
}