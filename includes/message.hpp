#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include "../includes/client.hpp"

class message
{
private:
public:
	static void sendMsgToUser(Client *user, std::string str)
	{
		user->sendMsg(str);
	}
};
