#include "../includes/ircserv.hpp"

int main(int ac, char **av)
{
	try
	{
		if (ac != 3)
			throw std::invalid_argument("arguments: ./ircserv <port> <password>");
	}
	catch (std::exception const &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}

	return 0;
}