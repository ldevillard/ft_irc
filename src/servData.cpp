#include "../includes/servData.hpp"
#include "../includes/Parser.hpp"

void handleLine(int sd, int uid, std::string line);

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

void ServData::onInteraction()
{
	for (int i = 0; i < _max_clients; i++)
	{
		_sd = _client_sockets[i];
		if (FD_ISSET(_sd, &_read_fds))
		{
			std::string actualLine;
			bool read = true;
			_buffer = 0;
			while (read && _buffer != '\n')
			{
				_buffer = 0;
				_valread = recv(_sd, &_buffer, 1, 0);
				if (_valread == -1)
				{
					std::cerr << "Error inr recv(). Quiting" << std::endl;
					read = false;
					break;
				}
				else if (_valread == 0)
				{
					std::cout << "Client disconnected!" << std::endl;
					read = false;
					close(_sd);
					_client_sockets[i] = 0;
					break;
				}
				actualLine.push_back(_buffer);
			}
			if (!read)
				break;
			else if (actualLine.c_str()[actualLine.length() - 1] == '\r')
				actualLine.erase(actualLine.length() - 1, 1);

			handleLine(_sd, i, actualLine);
			/*PARSING COMMANDS
				
				need to pass User that execute the command
				and servData(this) to send the result
				*/
			Parser parser(actualLine, this); //if there's a cmd it'll execute it
			send(_sd, actualLine.c_str(), actualLine.length(), 0);
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
