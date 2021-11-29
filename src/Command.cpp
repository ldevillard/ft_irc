#include "../includes/Command.hpp"

Command::Command(std::string name, std::string desc) : _name(name), _desc(desc)
{
}

Command::~Command()
{
}