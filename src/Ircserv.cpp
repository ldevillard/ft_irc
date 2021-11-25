#include "../includes/Ircserv.hpp"

Server::Server(std::string host, id_t port, std::string password) : _data(port), _host(host), _port(port), _password(password)
{
	(void)_port;
}

void Server::connect()
{
	_data.connect();
}
