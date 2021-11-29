#pragma once

#include "../Command.hpp"

class Command;

class Help : public Command
{
	public:
		Help();
		void execute() const;
};