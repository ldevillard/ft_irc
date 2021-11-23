#pragma once

#include <exception>

class ServerException
{
	public:
		class argument : public std::exception
		{
			const char * what () const throw ()
			{
				return "arguments: ./ircserv <port> <password>";
			}
		};

		class non_numeric_argument : public std::exception
		{
			const char * what () const throw ()
			{
				return "non numeric argument!";
			}
		};
};
