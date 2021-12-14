#pragma once
#include <vector>
#include <string>
#include "message.hpp"
#include "client.hpp"

class Channel
{
private:
	std::vector<Client *> _members;
	std::vector<Client *> _ops;
	std::string _channelName;

public:
	Channel(std::string name) : _channelName(name) {}
	~Channel();

	void join(Client *user);

	std::string &getName() { return _channelName; }

	void broadcastMsg(std::string msg)
	{
		for (std::vector<Client *>::iterator user = _members.begin(); user != _members.end(); user++)
			message::sendMsgToUser(*user, msg);
	}

	void broadcastMsgExept(std::string msg, Client *sender)
	{
		for (std::vector<Client *>::iterator user = _members.begin(); user != _members.end(); user++)
		{
			if (*user != sender)
				message::sendMsgToUser(*user, msg);
		}
	}

	bool isUserInChannel(Client *user)
	{
		std::vector<Client*>::iterator it;

		for (it = _members.begin(); it != _members.end(); it++)
		{
			if (user == (*it))
				return true;
		}

		for (it = _ops.begin(); it != _ops.end(); it++)
		{
			if (user == (*it))
				return true;
		}

		return false;
	}
};
