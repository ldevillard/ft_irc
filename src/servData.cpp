#include "../includes/servData.hpp"
#include "../includes/Parser.hpp"

ServData::~ServData()
{
}

ServData::ServData()
{
}

ServData::ServData(id_t port, std::string password) : _msg("IRC better than ever before!\n"), _addrlen(sizeof(_address)), _max_clients(5), _opt(1), _port(port), _password(password)
{
	std::cout << "Port : " << _port << std::endl;
}

void ServData::setup()
{
	// Create a master socket
	std::signal(SIGPIPE, SIG_IGN);
	for (int i = 0; i < _max_clients; i++)
		_client_sockets[i] = 0;
	if ((_master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		throw ServerException::socket_creation();
	// It should work without this part, it just allows the socket to handle multiple connections
	// it seems to be a good behavior to have that's all
	if (setsockopt(_master_socket, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt)) < 0)
		throw ServerException::socket_config();
	if (setsockopt(_master_socket, SOL_SOCKET, SO_REUSEPORT, &_opt, sizeof(_opt)) < 0)
		throw ServerException::socket_config();
	// Define the kind of socket created
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(_port);
	// Bind the socket to the desired port
	if (bind(_master_socket, (struct sockaddr *)&_address, sizeof(_address)) < 0)
		throw ServerException::binding();
	if (listen(_master_socket, 3) < 0)
		throw ServerException::listening();
	_addrlen = sizeof(_address);
	std::cout << "Waiting for connections..." << std::endl;
}

int readLine(int fd, std::string &line)
{
	char buff = 0;
	int read;
	while (buff != '\n')
	{
		buff = 0;
		std::cout << "start read" << std::endl;
		read = recv(fd, &buff, 1, 0);
		std::cout << "stop read " << (int)buff << std::endl;
		if (read <= 0)
			return read - 1;
		else if (buff)
			line.push_back(buff);
	}
	if (line.c_str()[line.length() - 1] == '\r')
		line.erase(line.length() - 1, 1);
	return line.length();
}

void ServData::onInteraction()
{
	for (int i = 0; i < _max_clients; i++)
	{
		_sd = _client_sockets[i];
		if (FD_ISSET(_sd, &_read_fds))
		{
			std::string line;

			int status = readLine(_sd, line);
			if (status == -2)
			{
				std::cerr << "Error inr recv(). Quiting" << std::endl;
			}
			else if (status == -1)
			{
				std::cout << "Client disconnected!" << std::endl;
				close(_sd);
				_client_sockets[i] = 0;
			}
			else
			{
				std::cout << "* <" << i << "> " << line << std::endl;

				// if (line == "JOIN #salut")
				// {
				// std::string msg = response(RPL_INFO, "gapoulain", "", ":pouet");
				// std::cout << "send \"" << msg << "\"" << std::endl;
				// send(sd, msg.c_str(), msg.length(), 0);
				// }

				send(_sd, line.c_str(), line.length(), 0);
				/*PARSING COMMANDS
				
				need to pass User that execute the command
				and servData(this) to send the result
				*/
				Parser parser(line, this); //if there's a cmd it'll execute it
				send(_sd, line.c_str(), line.length(), 0);
			}
		}
	}
}

void ServData::onConnection()
{
	if (FD_ISSET(_master_socket, &_read_fds))
	{
		if ((_new_socket = accept(_master_socket, (struct sockaddr *)&_address, (socklen_t *)&_addrlen)) < 0)
			throw ServerException::receiving();
		// gives infos that we'll use in send and recv
		std::cout << "New connection : socket fd [" << _new_socket << "] ip [" << inet_ntoa(_address.sin_addr) << "] port [" << ntohs(_address.sin_port) << "]" << std::endl;
		// add new socket to sockets array
		for (int i = 0; i < _max_clients; i++)
		{
			if (!_client_sockets[i])
			{
				_client_sockets[i] = _new_socket;
				std::cout << "Adding to sockets array as : " << i << std::endl;
				break;
			}
		}
	}
}

void ServData::setupFD()
{
	// reset the socket set
	FD_ZERO(&_read_fds);
	// add master to the set
	FD_SET(_master_socket, &_read_fds);
	_max_sd = _master_socket;
	// add child sockets to set
	for (int i = 0; i < _max_clients; i++)
	{
		_sd = _client_sockets[i];
		// if valid socket you can add it to the read list
		if (_sd > 0)
			FD_SET(_sd, &_read_fds);
		// highest fd number, needed for poll()
		if (_sd > _max_sd)
			_max_sd = _sd;
	}
}

int ServData::connect()
{
	setup();
	while (true)
	{
		setupFD();
		// Wait for an activity on one of the socket
		// Timeout set to null so infinite waiting
		// migh change later to poll
		_activity = select(_max_sd + 1, &_read_fds, NULL, NULL, NULL);
		if ((_activity < 0) && (errno != EINTR))
			throw ServerException::select();
		// If something happens on the master socket
		// then it's an incoming connection
		onConnection();
		// Else it's some IO interaction on another socket
		onInteraction();
	}
	return (0);
}
