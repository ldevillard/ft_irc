#include "ServData.hpp"


ServData::ServData()
{
	msg = "uwu les copains!\n";
	memset(buffer, 0, 1024);
	addlen = sizeof(address);

	opt = 1;
	port = 8080;
}