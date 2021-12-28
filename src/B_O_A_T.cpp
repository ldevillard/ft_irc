#include "../includes/B_O_A_T.hpp"
#include "../includes/message.hpp"
#include <curl/curl.h>
#include <sstream>
#include <string>
#include <ctime>
#include "../server_config.hpp"

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

bool boating(std::string msg, Client *user, Channel *chan)
{
	if (!BOTCFG_ENABLE)
		return true;
	std::string botNickName = "b.o.a.t";
	std::vector<std::string> *banWords = user->getServer()->getBanWordsList();

	if (!chan->isOp(user) || !BOTCFG_OPBYPASSFILTER)

		for (std::vector<std::string>::iterator it = banWords->begin(); it != banWords->end(); it++)
		{
			if (msg.find(*it) != std::string::npos)
			{
				if (BOTCFG_FILTERKICK)
				{
					chan->broadcastMsg(":" + botNickName + " KICK " + chan->getName() + " " + user->getNickName() + " Sending ban words");
					chan->leave(user);
				}
				else
					user->sendMsg(":" + botNickName + " PRIVMSG " + chan->getName() + " :<only visible by you> Your message contain ban word and have been blocked");
				return false;
			}
		}

	if (msg[0] && msg[0] == '!')
	{
		msg.erase(msg.begin());
		std::vector<std::string> args = Parser::split(msg);

		if (args[0] == "help")
		{
			chan->broadcastMsg(":" + botNickName + " PRIVMSG " + chan->getName() + " : !help, !dice, !source");
		}
		else if (args[0] == "dice")
		{
			std::srand(time(NULL));
			chan->broadcastMsg(":" + botNickName + " PRIVMSG " + chan->getName() + " :" + Message::c_itoa((std::rand() % 6) + 1));
		}
		else if (args[0] == "source")
		{
			chan->broadcastMsg(":" + botNickName + " PRIVMSG " + chan->getName() + " :https://github.com/ldevillard/ft_irc");
		}
		else if (args[0] == "cat")
		{
			CURLplusplus curlpp;
			std::string result = curlpp.Get("https://api.thecatapi.com/v1/images/search");
			size_t pos = result.find("http");
			result = result.substr(pos, result.size() - pos);
			pos = result.find("\"");
			result = result.substr(0, result.size() - (result.size() - pos));
			chan->broadcastMsg(":" + botNickName + " PRIVMSG " + chan->getName() + " :" + result);
		}
		else if (args[0] == "time")
		{
			time_t now = time(NULL);
			chan->broadcastMsg(":" + botNickName + " PRIVMSG " + chan->getName() + " :" + ctime(&now));
		}
		else
		{
			chan->broadcastMsg(":" + botNickName + " PRIVMSG " + chan->getName() + " :Command not found (" + args[0] + ")");
		}
	}
	return true;
}

/*
lè banwourd

!cat
!time
!source
!dice (et pas battlefield ahah)
*/
