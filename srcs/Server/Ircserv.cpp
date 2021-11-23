#include "Ircserv.hpp"

Server::Server(std::string host, int port, std::string password) : _host(host), _port(port), _password(password)
{
}

void	Server::address_init(sockaddr_in &addr, int port)
{
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
}

void	Server::server_init(ServData &new_server)
{
	if (!(new_server.server_fd = socket(AF_INET, SOCK_STREAM, 0)))
		throw std::runtime_error("Error during fd init");
	if (setsockopt(new_server.server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &new_server.opt, sizeof(new_server.opt)))
		throw std::runtime_error("Setsockpot");
	address_init(new_server.address, new_server.port);
	if (bind(new_server.server_fd, (struct sockaddr *)&new_server.address, sizeof(new_server.address)) < 0)
		throw std::runtime_error("Binding failed");
	if (listen(new_server.server_fd, 3) < 0)
		throw std::runtime_error("listen failure");
	if ((new_server.new_socket = accept(new_server.server_fd, (struct sockaddr *)&new_server.address, (socklen_t *)&new_server.addlen)) < 0)
		throw std::runtime_error("connection accept failure");
}

int	Server::connect()
{
	try
	{
		server_init(*_data);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
	_data->valread = read(_data->new_socket, _data->buffer, 1024);
	std::cout << _data->buffer << std::endl;
	send(_data->new_socket, _data->msg.c_str(), _data->msg.length(), 0);
	std::cout << "Message sent!" << std::endl;
	return (0);
}