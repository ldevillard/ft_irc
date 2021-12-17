#pragma once

#include "../command.hpp"
#include "../client.hpp"

class Command;

class Die : public Command
{
public:
	Die(Client *user);
	void execute();
};
