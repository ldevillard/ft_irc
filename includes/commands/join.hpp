#pragma once

#include "../command.hpp"
#include "../client.hpp"

class Command;

class Join : public Command
{
public:
	Join(Client *user);
	void execute();
};
