#pragma once

#include <string>
#include <iostream>
#include "servData.hpp"
#include "serverException.hpp"

void sendMsgToUser(User *user, std::string str);
std::string response(int response_code, std::string name, std::string command, std::string message);

class Command
{
	public:
		Command(std::string name, std::string desc, User *user);
		virtual ~Command();

		std::string getDesc() const { return _desc; }
		std::string getName() const { return _name; }

		void setServer(ServData *server) { _server = server; }
		void setArgs(std::vector<std::string> &args) { _args = args; }

		virtual void execute() = 0;

	protected:
		std::string _name;
		std::string _desc;

		ServData *_server;
		User	*_user;
		std::vector<std::string> _args;
};