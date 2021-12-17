#include "../../includes/commands/notice.hpp"
#include "../../includes/rpl_codes.hpp"

Notice::Notice(Client *user) : Command("NOTICE", "NOTICE - <target> <text to be send>: Send message (without automatic reply) to other client or channel.", user)
{
}

void Notice::execute()
{

}