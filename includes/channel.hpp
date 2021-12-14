#pragma once
#include <vector>
#include <string>
#include "message.hpp"
#include "client.hpp"
#include "../includes/rpl_codes.hpp"

class Channel
{
private:
	std::vector<Client *> _members;
	std::vector<Client *> _ops;
	std::string _channelName;

public:
	Channel(std::string name) : _channelName(name) {}
	~Channel();

	void join(Client *user)
	{
		message::sendMsgToUser(user, ":" + user->getNickName() + "!" + user->getNickName() + "@" + user->getAddress() + " JOIN " + _channelName);
		_members.push_back(user);
		updateUsersList();
	}

	void leave(Client *user);

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
		std::vector<Client *>::iterator it;

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

	void updateUsersList(void)
	{
		std::string usersList = "";
		for (std::vector<Client *>::iterator user = _members.begin(); user != _members.end(); user++)
		{
			usersList += (*user)->getNickName() + " ";
		}

		for (std::vector<Client *>::iterator user = _members.begin(); user != _members.end(); user++)
		{
			message::sendMsgToUser(*user, (":127.0.0.1 " + std::string(RPL_NAMREPLY) + " " + (*user)->getNickName() + " = " + _channelName + " :" + usersList));
			message::sendMsgToUser(*user, (":127.0.0.1 " + std::string(RPL_ENDOFNAMES) + " " + (*user)->getNickName() + " = " + _channelName + " :End of NAMES list"));
		}
	}
};
