#include "Ircserv.hpp"

Server::Server(std::string host, int port, std::string password) : _host(host), _port(port), _password(password)
{
}

void	Server::address_init(ServData data)
{
	data._address.sin_family = AF_INET;
	data._address.sin_addr.s_addr = INADDR_ANY;
	data._address.sin_port = htons(_port);
}

void	Server::server_init(ServData data)
{
	if (!(data._server_fd = socket(AF_INET, SOCK_STREAM, 0)))
		throw ServerException::socket_creation();
	if (setsockopt(data._server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &data._opt, sizeof(data._opt)))
		throw ServerException::socket_config();
	address_init(data);
	if (bind(data._server_fd, (struct sockaddr *)&data._address, sizeof(data._address)) < 0)
		throw ServerException::binding();
	if (listen(data._server_fd, 3) < 0)
		throw ServerException::listening();
	if ((data._new_socket = accept(data._server_fd, (struct sockaddr *)&data._address, (socklen_t *)&data._addlen)) < 0)
		throw ServerException::receiving();
}

int	Server::connect()
{
	ServData *data;
	data->_msg = "Salut les gars!\n";
	data->_addlen = sizeof(data->_address);
	data->_opt = 1;
	data->_port = 8080;
	try
	{
		server_init(*data);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
	data->_valread = read(data->_new_socket, data->_buffer, 1024);
	std::cout << data->_buffer << std::endl;
	send(data->_new_socket, data->_msg.c_str(), data->_msg.length(), 0);
	std::cout << "Message sent!" << std::endl;
	return (0);
}