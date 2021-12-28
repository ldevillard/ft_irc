#include "../includes/servData.hpp"
#include "../includes/parser.hpp"
#include "../includes/rpl_codes.hpp"
#include "../includes/client.hpp"
#include "../includes/channel.hpp"
#include "../includes/message.hpp"

ServData::~ServData()
{
	std::map<std::string, Channel *>::iterator it;

	for (it = _chan_list.begin(); it != _chan_list.end(); it++)
		delete (*it).second;

	for (int i = 0; i < CFG_MAXCLIENTS; i++)
	{
		if (_clients[i])
			delete _clients[i];
	}
}

ServData::ServData()
{
}

ServData::ServData(id_t port, std::string password) : _msg("IRC better than ever before!\n"), _addrlen(sizeof(_address)), _opt(1), _port(port), _password(password)
{
	if (_password.empty())
		_needPsswd = false;
	else
		_needPsswd = true;
	std::cout << "Port : " << _port << std::endl;
}

void initBanWords(std::vector<std::string> &list)
{
	list.push_back("owo");
	list.push_back("sad");
}

void ServData::setup()
{
	shutdown = false;
	// Create a master socket
	std::signal(SIGPIPE, SIG_IGN);
	for (int i = 0; i < CFG_MAXCLIENTS; i++)
		_clients[i] = NULL;
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
	initBanWords(_banWordsList);
	std::cout << "Waiting for connections..." << std::endl;
}

int readLine(Client &user)
{
	char buff = 0;
	int read;
	std::string &line = user.getBufferLine();
	read = recv(user.getSd(), &buff, 1, 0);
	if (read <= 0)
		return read;
	else if (buff == '\n')
	{
		if (line.length() > 0 && line.at(line.length() - 1) == '\r')
			line.erase(line.length() - 1, 1);
		return 2;
	}
	else if (buff != 0)
		line.push_back(buff);
	return 1;
}

void ServData::onInteraction()
{
	for (int i = 0; i < CFG_MAXCLIENTS; i++)
	{

		if (_clients[i] && FD_ISSET(_clients[i]->getSd(), &_read_fds))
		{
			_sd = _clients[i]->getSd();

			int status = readLine(*_clients[i]);
			if (status == -1)
			{
				std::cerr << "Error inr recv(). Quiting" << std::endl;
				_clients[i]->disconnect();
				delete _clients[i];
				_clients[i] = NULL;
			}
			else if (status == 0)
			{
				std::cout << "Client disconnected!" << std::endl;
				_clients[i]->disconnect();
				delete _clients[i];
				_clients[i] = NULL;
			}
			else if (status == 2)
			{
				std::string line = std::string(_clients[i]->getBufferLine());
				_clients[i]->getBufferLine().erase();

				if (line.length() > 0)
				{
					std::cout << "<- [" << _clients[i]->getSd() << "] " << line << std::endl;
					Parser parser(line, this, _clients[i]);
				}

				if (_clients[i]->getKill())
				{
					std::cout << "Client disconnected!" << std::endl;
					close(_sd);
					delete _clients[i];
					_clients[i] = NULL;
				}
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
		for (int i = 0; i < CFG_MAXCLIENTS; i++)
		{
			if (!_clients[i])
			{
				_clients[i] = new Client(this);
				_clients[i]->setSd(_new_socket);

				_clients[i]->setAddress(inet_ntoa(_address.sin_addr));
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
	for (int i = 0; i < CFG_MAXCLIENTS; i++)
	{
		if (_clients[i])
		{
			_sd = _clients[i]->getSd();
			// if valid socket you can add it to the read list
			if (_sd > 0)
				FD_SET(_sd, &_read_fds);
			if (_sd > _max_sd)
				_max_sd = _sd;
		}
	}
}

int ServData::connect()
{
	setup();
	while (true)
	{
		if (shutdown)
			break;

		setupFD();
		// Wait for an activity on one of the socket
		// Timeout set to null so infinite waiting
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

std::string str_to_lower(std::string str)
{
	std::string res;

	for (std::string::iterator it = str.begin(); it != str.end(); it++)
		res.push_back(std::tolower(*it));
	return res;
}

Client *ServData::getUserFromUsername(std::string name)
{
	for (size_t i = 0; i < CFG_MAXCLIENTS; i++)
	{
		if (_clients[i] && str_to_lower(name) == str_to_lower(_clients[i]->getUserName()))
			return _clients[i];
	}
	return NULL;
}

Client *ServData::getUser(std::string name)
{
	for (size_t i = 0; i < CFG_MAXCLIENTS; i++)
	{
		if (_clients[i] && str_to_lower(name) == str_to_lower(_clients[i]->getNickName()))
			return _clients[i];
	}
	return NULL;
}

std::map<std::string, Channel *> &ServData::getChannels() { return _chan_list; }

Channel *ServData::findChannel(std::string name)
{
	std::map<std::string, Channel *>::iterator it;

	for (it = _chan_list.begin(); it != _chan_list.end(); it++)
	{
		if (str_to_lower(name) == str_to_lower((*it).first))
			return (*it).second;
	}
	return (NULL);
}

void ServData::shutdownServer()
{
	shutdown = true;
}

std::vector<Channel *> ServData::findChannelsOfUser(Client *user)
{
	std::vector<Channel *> tab;

	for (std::map<std::string, Channel *>::iterator it = _chan_list.begin(); it != _chan_list.end(); it++)
	{
		if ((*it).second->isUserInChannel(user) == true)
			tab.push_back((*it).second);
	}
	return tab;
}

std::vector<Client *> ServData::getVectorUser()
{
	std::vector<Client *> users;

	for (int i = 0; i < CFG_MAXCLIENTS; i++)
	{
		if (_clients[i] != NULL)
			users.push_back(_clients[i]);
	}

	return users;
}

std::vector<std::string> *ServData::getBanWordsList()
{
	return &_banWordsList;
}

bool ServData::passwordAuth(std::string check)
{
	return _password == check;
}

bool ServData::needPsswd(void)
{
	return _needPsswd;
}
