#pragma once
#include <vector>
#include <string>
#include "user.hpp"

std::string response(int response_code, std::string name, std::string command, std::string message);
void sendMsgToUser(User *user, std::string str);
// void sendMsgToChannel(channel *chan, std::string str);

class channel
{
private:
	std::vector<User *> _members;
	std::vector<User *> _ops;
	std::string _channelName;

public:
	channel(std::string name) : _channelName(name) {}
	~channel();

	void addUser(User *user)
	{
		_members.push_back(user);
	}

	void broadcastMsg(std::string msg)
	{
		for (std::vector<User *>::iterator user = _members.begin(); user != _members.end(); user++)
			sendMsgToUser(*user, msg);
	}
};
