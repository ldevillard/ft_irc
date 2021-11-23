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
		throw ServerException::socket_creation();
	if (setsockopt(new_server.server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &new_server.opt, sizeof(new_server.opt)))
		throw ServerException::socket_config();
	address_init(new_server.address, new_server.port);
	if (bind(new_server.server_fd, (struct sockaddr *)&new_server.address, sizeof(new_server.address)) < 0)
		throw ServerException::binding();
	if (listen(new_server.server_fd, 3) < 0)
		throw ServerException::listening();
	if ((new_server.new_socket = accept(new_server.server_fd, (struct sockaddr *)&new_server.address, (socklen_t *)&new_server.addlen)) < 0)
		throw ServerException::receiving();
}

int	connect()
{
	try
	{
		server_init(_Serv);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
	new_server.valread = read(new_server.new_socket, new_server.buffer, 1024);
	std::cout << new_server.buffer << std::endl;
	send(new_server.new_socket, new_server.msg.c_str(), new_server.msg.length(), 0);
	std::cout << "Message sent!" << std::endl;
	return (0);
}