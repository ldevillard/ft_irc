#include "../includes/command.hpp"

Command::Command(std::string name, std::string desc) : _name(name), _desc(desc)
{
	_server = NULL;
}

Command::~Command()
{
}