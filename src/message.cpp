#include "../includes/message.hpp"

std::string Message::c_itoa(int n)
{
	std::stringstream str;
	str << n;
	std::string res;
	str >> res;
	return res;
}

void Message::sendMsgToUser(Client *user, std::string str)
{
	user->sendMsg(str);
}
