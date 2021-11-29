#include "../includes/servData.hpp"

int main(int ac, char **av)
{
	try
	{
		if (ac != 3)
			throw ServerException::argument();
		for (int i = 0; av[1][i]; i++)
			if (!isdigit(av[1][i]))
				throw ServerException::non_numeric_argument();
		ServData serv(atoi(av[1]), av[2]);
		serv.connect();
	}
	catch (std::exception const &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
} //Je fais un test deqfrwef