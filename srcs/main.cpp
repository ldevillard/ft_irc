#include "Server/Ircserv.hpp"

int main(int ac, char **av)
{
	try
	{
		if (ac != 3)
			throw ServerException::argument();
		for (int i = 0; i != NULL; i++)
			if (!isdigit(av[1][i]))
				throw ServerException::argument();
	}
	catch (std::exception const &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}