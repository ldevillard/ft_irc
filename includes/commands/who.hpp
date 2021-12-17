#pragma once

#include "../command.hpp"
#include "../client.hpp"

class Command;

class Who : public Command
{
public:
	Who(Client *user);
	void execute();
};
