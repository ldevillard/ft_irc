#include "../includes/client.hpp"

Client::Client(ServData *server) : _server(server)
{
	_isRegistered = false;
	_correctPsswd = false;
	_getKilled = false;
	_user.clear();
	_nick.clear();
	_realName.clear();
}

const std::string &Client::getUserName() const { return _user; }
const std::string &Client::getNickName() const { return _nick; }
std::string &Client::getRealName() { return _realName; }
const std::string &Client::getAddress() const { return _address; }
const bool &Client::isRegistered() const { return _isRegistered; }

const int &Client::getSd() const { return _sd; }
void Client::setSd(int sd) { _sd = sd; }
std::string &Client::getBufferLine() { return _bufferLine; }

void Client::setUserName(std::string name)
{
	if (name != "none")
		_user = name;
}
void Client::setNickName(std::string name)
{
	if (name != "none")
		_nick = name;
}
void Client::setRealName(std::string name)
{
	if (name != "none")
		_realName = name;
}
void Client::setAddress(std::string address) { _address = address; }
void Client::setRegistered(bool state) { _isRegistered = state; }

void Client::sendMsg(std::string msg)
{
	std::cout << "-> [" << _sd << "] " << msg << std::endl;
	send(_sd, (msg + "\r\n").c_str(), (msg + "\r\n").length(), 0);
}

void Client::disconnect()
{
	std::vector<Channel *> chans = _server->findChannelsOfUser(this);

	for (std::vector<Channel *>::iterator it = chans.begin(); it != chans.end(); it++)
		(*it)->leave(this);
	close(_sd);
}

ServData *Client::getServer()
{
	return _server;
}

void Client::setPsswdState(bool state)
{
	_correctPsswd = state;
}

bool Client::getPsswdState(void)
{
	return _correctPsswd;
}

void Client::setKill(bool state)
{
	_getKilled = state;
}

bool Client::getKill(void)
{
	return _getKilled;
}
