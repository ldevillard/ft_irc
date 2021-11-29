#include "../includes/servData.hpp"

ServData::~ServData()
{
}

ServData::ServData()
{
}

ServData::ServData(id_t port) : _msg("IRC better than ever before!\n"), _addrlen(sizeof(_address)), _max_clients(5), _opt(1), _port(port)
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
	_address.sin_port = htons(8080);
	// Bind the socket to the desired port
	if (bind(_master_socket, (struct sockaddr *)&_address, sizeof(_address)) < 0)
		throw ServerException::binding();
	if (listen(_master_socket, 3) < 0)
		throw ServerException::listening();
	_addrlen = sizeof(_address);
	std::cout << "Waiting for connections..." << std::endl;
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

void ServData::onInteraction()
{
	for (int i = 0; i < _max_clients; i++)
	{
		_sd = _client_sockets[i];
		if (FD_ISSET(_sd, &_read_fds))
		{
			std::string userSave = "";
			bool goin = true;

			while (goin || !userSave.empty())
			{
				bool read = true;
				std::string actualLine;
				goin = false;
				bzero(_buffer, SOCKET_BUFFER_SIZE);
				strncpy(_buffer, userSave.c_str(), std::min((size_t)SOCKET_BUFFER_SIZE, userSave.length()));
				userSave.erase();
				actualLine += std::string(_buffer);
				while (read && !std::strchr(_buffer, '\n') && !std::strchr(_buffer, '\r'))
				{
					bzero(_buffer, SOCKET_BUFFER_SIZE);
					_valread = recv(_sd, _buffer, SOCKET_BUFFER_SIZE, 0);
					//crash/error
					if (_valread == -1)
					{
						std::cerr << "Error inr recv(). Quiting" << std::endl;
						userSave.erase();
						read = false;
						break;
					}
					//simple deconnection
					else if (_valread == 0)
					{
						std::cout << "Client disconnected!" << std::endl;
						userSave.erase();
						read = false;
						close(_sd);
						_client_sockets[i] = 0;
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
				}

				std::cout << "* <" << i << "> " << actualLine << std::endl;
				/*PARSING COMMANDS
				
				need to pass User that execute the command
				and servData(this) to send the result
				*/
				Parser parser(actualLine); //if there's a cmd it'll execute it
				send(_sd, actualLine.c_str(), actualLine.length(), 0);
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
