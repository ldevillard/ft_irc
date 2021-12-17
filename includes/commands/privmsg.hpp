#pragma once

#include "../command.hpp"
#include "../client.hpp"
#include <vector>

class Command;

class Privmsg : public Command
{
private:
	std::string makeMessage();

public:
	Privmsg(Client *user);
	void execute();
};
