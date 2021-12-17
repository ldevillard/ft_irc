#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include "../includes/client.hpp"

class Message
{
private:
public:
	static std::string c_itoa(int n)
	{
		std::stringstream str;
		str << n;
		std::string res;
		str >> res;
		return res;
	}
	static void sendMsgToUser(Client *user, std::string str)
	{
		user->sendMsg(str);
	}
};
