#pragma once

#include "../command.hpp"
#include "../client.hpp"

class Command;

class User : public Command
{
public:
	User(Client *user);
	void execute();
};
