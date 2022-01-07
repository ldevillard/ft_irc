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

bool msgFiltering(std::string msg, Client *user, Channel *chan)
{
	std::vector<std::string> *banWords = user->getServer()->getBanWordsList();

	for (std::vector<std::string>::iterator it = banWords->begin(); it != banWords->end(); it++)
	{
		if (msg.find(*it) != std::string::npos)
		{
			if (BOTCFG_FILTERKICK)
			{
				chan->broadcastMsg(":" + std::string(BOTCFG_NAME) + " KICK " + chan->getName() + " " + user->getNickName() + " Sending ban words");
				chan->leave(user);
			}
			else
				user->sendMsg(":" + std::string(BOTCFG_NAME) + " PRIVMSG " + chan->getName() + " :<only visible by you> Your message contain ban word and have been blocked");
			return true;
		}
	}
	return false;
}

bool boating(std::string msg, Client *user, Channel *chan)
{
	if (!BOTCFG_ENABLE)
		return true;

	if (BOTCFG_ENABLEFILTER && (!chan->isOp(user) || !BOTCFG_OPBYPASSFILTER))
	{
		if (msgFiltering(msg, user, chan))
			return false;
	}

	if (msg[0] && msg[0] == '!')
	{
		msg.erase(msg.begin());
		if (msg.empty())
		{
			user->sendMsg(":" + std::string(BOTCFG_NAME) + " PRIVMSG " + chan->getName() + " :invalid command");
		}
		else
		{

			std::vector<std::string> args = Parser::split(msg);
			if (args[0] == "help")
			{
				chan->broadcastMsgExept(":" + user->getNickName() + " PRIVMSG " + chan->getName() + " :!" + msg, user);
				chan->broadcastMsg(":" + std::string(BOTCFG_NAME) + " PRIVMSG " + chan->getName() + " :!cat, !dice, !help, !source, !time");
			}
			else if (args[0] == "dice")
			{
				chan->broadcastMsgExept(":" + user->getNickName() + " PRIVMSG " + chan->getName() + " :!" + msg, user);
				std::srand(time(NULL));
				chan->broadcastMsg(":" + std::string(BOTCFG_NAME) + " PRIVMSG " + chan->getName() + " :" + Message::c_itoa((std::rand() % 6) + 1));
			}
			else if (args[0] == "source")
			{
				chan->broadcastMsgExept(":" + user->getNickName() + " PRIVMSG " + chan->getName() + " :!" + msg, user);
				chan->broadcastMsg(":" + std::string(BOTCFG_NAME) + " PRIVMSG " + chan->getName() + " :https://github.com/ldevillard/ft_irc");
			}
			else if (args[0] == "cat")
			{
				chan->broadcastMsgExept(":" + user->getNickName() + " PRIVMSG " + chan->getName() + " :!" + msg, user);
				CURLplusplus curlpp;
				std::string result = curlpp.Get("https://api.thecatapi.com/v1/images/search");
				size_t pos = result.find("http");
				result = result.substr(pos, result.size() - pos);
				pos = result.find("\"");
				result = result.substr(0, result.size() - (result.size() - pos));
				chan->broadcastMsg(":" + std::string(BOTCFG_NAME) + " PRIVMSG " + chan->getName() + " :" + result);
			}
			else if (args[0] == "time")
			{
				chan->broadcastMsgExept(":" + user->getNickName() + " PRIVMSG " + chan->getName() + " :!" + msg, user);
				time_t now = time(NULL);
				chan->broadcastMsg(":" + std::string(BOTCFG_NAME) + " PRIVMSG " + chan->getName() + " :" + ctime(&now));
			}
			else
			{
				user->sendMsg(":" + std::string(BOTCFG_NAME) + " PRIVMSG " + chan->getName() + " :Command not found (" + args[0] + ")");
			}
		}
		return false;
	}
	else
		return true;
}

/*
lè banwourd

!cat
!time
!source
!dice (et pas battlefield ahah)
*/
