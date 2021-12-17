#pragma once

#include "../command.hpp"
#include "../client.hpp"

class Command;

class Quit : public Command
{
private:
	std::string makeMessage();

public:
	Quit(Client *user);
	void execute();
};
