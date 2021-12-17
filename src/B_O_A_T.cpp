#include "../includes/B_O_A_T.hpp"
#include <random>
#include "../includes/message.hpp"
#include <curl/curl.h>
#include <sstream>
#include <string>

class CURLplusplus
{
private:
	CURL *curl;
	std::stringstream ss;
	long http_code;

public:
	CURLplusplus()
		: curl(curl_easy_init()), http_code(0)
	{
	}
	~CURLplusplus()
	{
		if (curl)
			curl_easy_cleanup(curl);
	}
	std::string Get(const std::string &url)
	{
		CURLcode res;
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

		ss.str("");
		http_code = 0;
		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			throw std::runtime_error(curl_easy_strerror(res));
		}
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
		return ss.str();
	}
	long GetHttpCode()
	{
		return http_code;
	}

private:
	static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
	{
		return static_cast<CURLplusplus *>(userp)->Write(buffer, size, nmemb);
	}
	size_t Write(void *buffer, size_t size, size_t nmemb)
	{
		ss.write((const char *)buffer, size * nmemb);
		return size * nmemb;
	}
};

void boating(std::string msg, Client *user, Channel *chan)
{
	(void)user;
	(void)chan;
	std::cout << "bot = " << msg << std::endl;
	std::string botNickName = "b.o.a.t";

	std::vector<std::string> args = Parser::split(msg);

	if (args[0] == "help")
	{
		chan->broadcastMsg(":" + botNickName + " PRIVMSG " + chan->getName() + " : !help, !dice, !source");
	}
	else if (args[0] == "dice")
	{
		std::srand(time(NULL));
		chan->broadcastMsg(":" + botNickName + " PRIVMSG " + chan->getName() + " :" + message::c_itoa((std::rand() % 6) + 1));
	}
	else if (args[0] == "source")
	{
		chan->broadcastMsg(":" + botNickName + " PRIVMSG " + chan->getName() + " :https://github.com/ldevillard/ft_irc");
	}
	else if (args[0] == "cat")
	{
		CURLplusplus curlpp;

		std::string result = curlpp.Get("https://api.thecatapi.com/v1/images/search");

		std::cout << result << std::endl;

		size_t pos = result.find("http");

		result = result.substr(pos, result.size() - pos);

		pos = result.find("\"");

		result = result.substr(0, result.size() - (result.size() - pos));

		chan->broadcastMsg(":" + botNickName + " PRIVMSG " + chan->getName() + " :" + result);
	}
	else
	{
		chan->broadcastMsg(":" + botNickName + " PRIVMSG " + chan->getName() + " :Command not found (" + args[0] + ")");
	}
}

/*
lè banwourd

!cat
!time
!source
!dice (et pas battlefield ahah)
*/
