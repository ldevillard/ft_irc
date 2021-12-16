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
	~Channel() {}

	void join(Client *user);

	void leave(Client *user);

	void keepOp(void);

	bool isEmpty(void);

	std::string &getName(void);
	std::vector<Client *> getMembers(void);

	void broadcastMsg(std::string msg);

	void broadcastMsgExept(std::string msg, Client *sender);

	bool isUserInChannel(Client *user);

	void sendChannelInfos(Client *user);

	bool isOp(Client *user);
	void setOp(Client *user, bool state);
};
