#pragma once

#include "../command.hpp"
#include "../user.hpp"

class Command;

class Join : public Command
{
	public:
		Join(User *user);
		void execute();
};