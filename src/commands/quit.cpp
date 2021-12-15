#include "../../includes/commands/quit.hpp"
#include "../../includes/rpl_codes.hpp"

Quit::Quit(Client *user) : Command("QUIT", "QUIT - Disconnect from the server.", user)
{
}

void Quit::execute()
{

}