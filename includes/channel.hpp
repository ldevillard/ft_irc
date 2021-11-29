#include <vector>
#include <string>
#include "User.hpp"

class channel
{
private:
	std::vector<User *> _members;
	std::vector<User *> _ops;
	std::string _channelName;

public:
	channel();
	~channel();
};

channel::channel()
{
}

channel::~channel()
{
}
