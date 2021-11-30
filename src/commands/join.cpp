#include "../../includes/commands/join.hpp"
#include <iostream>
#include "../../includes/channel.hpp"
#include "../../includes/rpl_codes.hpp"

Join::Join(User *user) : Command("JOIN", "<channel> <password> : Join wanted channel.", user)
{
}

void Join::execute()
{
	if (_args[1] != "#")
	{
		channel *chan = new channel(_args[1]);

		chan->broadcastMsg(_args[1]);
		sendMsgToUser(_user, response(RPL_INFO, _user->getNick(), "", ":joining " + _args[1] + " channel"));
		sendMsgToUser(_user, _user->getNick() + "!" + _user->getNick() + "@127.0.0.1 JOIN" + _args[1]);

		//sendMsgToUser(_user, ":127.0.0.1 332 " + _user->getNick() + _args[1] + " :generic channel");
		sendMsgToUser(_user, ":127.0.0.1 353 "+ _user->getNick() + " = " + _args[1] + " :@" + _user->getNick());
		//sendMsgToUser(_user, ":127.0.0.1 366 gauthier #salut :End of NAMES list");
	}
}