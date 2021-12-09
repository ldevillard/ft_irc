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

	void broadcastMsg(std::string msg)
	{
		for (std::vector<User *>::iterator user = _members.begin(); user != _members.end(); user++)
			message::sendMsgToUser(*user, msg);
	}
};
