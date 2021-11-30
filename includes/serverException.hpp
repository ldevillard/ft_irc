#pragma once

#include <exception>

class ServerException
{
public:
	class argument : public std::exception
	{
		const char *what() const throw()
		{
			return "arguments: ./ircserv <port> <password>";
		}
	};

	class non_numeric_argument : public std::exception
	{
		const char *what() const throw()
		{
			return "non numeric argument!";
		}
	};

	class socket_creation : public std::exception
	{
		const char *what() const throw()
		{
			return "can't create socket!";
		}
	};

	class socket_config : public std::exception
	{
		const char *what() const throw()
		{
			return "can't configure socket!";
		}
	};

	class binding : public std::exception
	{
		const char *what() const throw()
		{
			return "binding failed!";
		}
	};

	class listening : public std::exception
	{
		const char *what() const throw()
		{
			return "listening failed!";
		}
	};

	class receiving : public std::exception
	{
		const char *what() const throw()
		{
			return "receiving failed!";
		}
	};

	class select : public std::exception
	{
		const char *what() const throw()
		{
			return "select error!";
		}
	};

	class send : public std::exception
	{
		const char *what() const throw()
		{
			return "send error!";
		}
	};

	class nickAlreadyUsed : public std::exception
	{
		const char *what() const throw()
		{
			return "nickname already used!";
		}
	};

	class invalidNick : public std::exception
	{
		const char *what() const throw()
		{
			return "invalid nickname!";
		}
	};
};
