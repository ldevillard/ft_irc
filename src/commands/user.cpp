#include "../../includes/commands/user.hpp"
#include <iostream>
#include "../../includes/channel.hpp"
#include "../../includes/rpl_codes.hpp"
#include "../../includes/message.hpp"

User::User(Client *user) : Command("USER", "USER - <username> <hostname> <servername> <realname> : Specify identity.", user)
{
}

void User::execute()
{
}