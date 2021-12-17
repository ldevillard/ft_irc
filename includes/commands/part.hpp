#pragma once

#include "../command.hpp"
#include "../client.hpp"
#include <vector>

class Command;

class Part : public Command
{
public:
	Part(Client *user);
	void execute();
};
