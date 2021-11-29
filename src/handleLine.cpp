#include <string>
#include <iostream>
#include <sys/socket.h>
#include "../includes/channel.hpp"

void send_to_chan(channel &channel)
{
	(void)channel;
}


void handleLine(int sd, int uid, std::string line)
{
	std::cout << "* <" << uid << "> " << line << std::endl;




	send(sd, line.c_str(), line.length(), 0);
}
