#pragma once

#include "../command.hpp"
#include "../client.hpp"

class Command;

class List : public Command
{
public:
	List(Client *user);
	void execute();
};
