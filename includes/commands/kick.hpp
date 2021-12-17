#pragma once

#include "../command.hpp"
#include "../client.hpp"

class Command;

class Kick : public Command
{
public:
	Kick(Client *user);
	void execute();
};
