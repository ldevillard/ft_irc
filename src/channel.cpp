#include "../includes/channel.hpp"
#include "../includes/message.hpp"

void Channel::join(User *user)
{
	message::sendMsgToUser(user, ":" + user->getNick() + "!" + user->getNick() + "@" + user->getAddress() + " JOIN " + _channelName);
	_members.push_back(user);
}
