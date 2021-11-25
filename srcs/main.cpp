#include "Server/Ircserv.hpp"

int main(int ac, char **av)
{
	try
	{
		if (ac != 3)
			throw ServerException::argument();
		for (int i = 0; av[1][i]; i++)
			if (!isdigit(av[1][i]))
				throw ServerException::non_numeric_argument();
		Server serv("127.0.0.1", atoi(av[1]), av[2]);
		serv.connect();
		serv.connect();
		serv.connect();
	}
	catch (std::exception const &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}