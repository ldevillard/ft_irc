#pragma once

#include "../command.hpp"
#include "../client.hpp"

class Command;

class Notice : public Command
{
private:
	std::string makeMessage();
public:
	Notice(Client *user);
	void execute();
};
