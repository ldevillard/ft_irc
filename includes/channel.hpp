#pragma once
#include <vector>
#include <string>
#include "message.hpp"
#include "user.hpp"

class Channel
{
private:
	std::vector<User *> _members;
	std::vector<User *> _ops;
	std::string _channelName;

public:
	Channel(std::string name) : _channelName(name) {}
	~Channel();

	void join(User *user);

	std::string &getName() { return _channelName; }

	void broadcastMsg(std::string msg)
	{
		for (std::vector<User *>::iterator user = _members.begin(); user != _members.end(); user++)
			message::sendMsgToUser(*user, msg);
	}

	bool isUserInChannel(User *user)
	{
		std::vector<User*>::iterator it;

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
