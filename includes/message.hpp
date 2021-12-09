#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include "../includes/user.hpp"

class message
{
private:
	static std::string c_itoa(int n)
	{
		std::stringstream str;
		str << n;
		std::string res;
		str >> res;
		return res;
	}

public:
	static std::string response(int response_code, std::string name, std::string command, std::string message)
	{
		std::string code = c_itoa(response_code);
		command = command.size() ? command + " " : command;

		std::string res = ":127.0.0.1/6667 " + code + " " + name + " " + command + message;
		return res;
	}
	static void sendMsgToUser(User *user, std::string str)
	{
		user->sendMsg(str);
	}
};
