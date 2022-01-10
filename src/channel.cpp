#include "../includes/channel.hpp"
#include "../includes/message.hpp"
#include "../server_config.hpp"

Channel::Channel(std::string name) : _channelName(name) {}

Channel::~Channel() {}

void Channel::join(Client *user)
{
	_members.push_back(user);
	broadcastMsg(":" + user->getNickName() + "!" + user->getUserName() + "@" + user->getAddress() + " JOIN " + _channelName);
	sendChannelInfos(user, true);
}

void Channel::leave(Client *user, bool needBroadcast)
{
	if (needBroadcast)
		broadcastMsg(":" + user->getNickName() + "!" + user->getUserName() + "@" + user->getAddress() + " PART " + _channelName);
	for (std::vector<Client *>::iterator it = _members.begin(); it != _members.end(); it++)
	{
		if ((*it) == user)
		{
			_members.erase(it);
			break;
		}
	}
	if (isOp(user))
	{
		for (std::vector<Client *>::iterator it = _ops.begin(); it != _ops.end(); it++)
		{
			if ((*it) == user)
			{
				_ops.erase(it);
				break;
			}
		}
		if (CFG_KEEPOPINCHANNEL)
			keepOp();
	}
}

void Channel::keepOp(void)
{
	if (_ops.empty() && !_members.empty())
		setOp(*_members.begin(), true);
}

bool Channel::isEmpty(void)
{
	if (_members.size() > 0)
		return false;
	else
		return true;
}

std::string &Channel::getName() { return _channelName; }

void Channel::broadcastMsg(std::string msg)
{
	for (std::vector<Client *>::iterator user = _members.begin(); user != _members.end(); user++)
		Message::sendMsgToUser(*user, msg);
}

void Channel::broadcastMsgExept(std::string msg, Client *sender)
{
	for (std::vector<Client *>::iterator user = _members.begin(); user != _members.end(); user++)
	{
		if (*user != sender)
			Message::sendMsgToUser(*user, msg);
	}
}

std::vector<Client *> Channel::getMembers(void)
{
	return _members;
}

bool Channel::isUserInChannel(Client *user)
{
	std::vector<Client *>::iterator it;

	for (it = _members.begin(); it != _members.end(); it++)
	{
		if (user == (*it))
			return true;
	}

	return false;
}

void Channel::sendChannelInfos(Client *user, bool sendTopic)
{
	std::string usersList = "";
	for (std::vector<Client *>::iterator userIt = _members.begin(); userIt != _members.end(); userIt++)
	{
		if (isOp(*userIt))
			usersList += "@";
		usersList += (*userIt)->getNickName() + " ";
	}

	if (sendTopic)
		Message::sendMsgToUser(user, (":server " + std::string(RPL_TOPIC) + " " + user->getNickName() + " " + _channelName + " :Undefined topic"));
	Message::sendMsgToUser(user, (":server " + std::string(RPL_NAMREPLY) + " " + user->getNickName() + " = " + _channelName + " :" + usersList));
	Message::sendMsgToUser(user, (":server " + std::string(RPL_ENDOFNAMES) + " " + _channelName + " :End of NAMES list"));
}

bool Channel::isOp(Client *user)
{
	for (std::vector<Client *>::iterator opIt = _ops.begin(); opIt != _ops.end(); opIt++)
	{
		if ((*opIt)->getNickName() == user->getNickName())
			return true;
	}
	return false;
}

void Channel::setOp(Client *user, bool state)
{
	if (state == true)
	{
		if (!isOp(user))
		{
			_ops.push_back(user);
			user->sendMsg(":server " + std::string(RPL_YOUREOPER) + ":You are now an IRC operator");
			broadcastMsgExept(":server " + std::string(RPL_WHOISOPERATOR) + " " + user->getNickName() + " :Is now an IRC operator", user);
		}
	}
	else
	{
		if (isOp(user))
		{
			for (std::vector<Client *>::iterator opIt = _ops.begin(); opIt != _ops.end(); opIt++)
			{
				if ((*opIt)->getNickName() == user->getNickName())
				{
					_ops.erase(opIt);
					break;
				}
			}
		}
	}
	for (std::vector<Client *>::iterator it = _members.begin(); it != _members.end(); it++)
		sendChannelInfos(*it, false);
}

Client *Channel::findUserWithName(std::string name)
{
	std::vector<Client *>::iterator it;

	for (it = _members.begin(); it != _members.end(); it++)
	{
		if ((*it)->getNickName() == name)
			return (*it);
	}

	return NULL;
}
