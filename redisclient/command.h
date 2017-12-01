

// add by mardyu 2017-11-21
// this file is used to create command for send to redis


#ifndef _REDIS_CLIENT_COMMAND_H_
#define _REDIS_CLIENT_COMMAND_H_


#include <vector>
#include <string>
#include <map>


namespace redis_client
{
	class Command
	{
	public:
		explicit Command();
		Command(Command &cmd);
		explicit Command(const std::string &cmd);
		explicit Command(const char *cmd);

		Command operator=(Command &cmd);

		operator std::string();
		std::string ToString();


		Command& operator<< (int8_t c);
		Command& operator<< (uint8_t c);
		Command& operator<< (int16_t s);
		Command& operator<< (uint16_t s);
		Command& operator<< (int32_t i);
		Command& operator<< (uint32_t i);
		Command& operator<< (int64_t l);
		Command& operator<< (uint64_t l);
		Command& operator<< (float f);
		Command& operator<< (double d);
		Command& operator<< (const std::string &s);
		Command& operator<< (const char *str);
		Command& operator<< (const std::vector<std::string> &v);
		Command& operator<< (const std::vector<double> &v);
		Command& operator<< (const std::map<std::string, std::string> &m);
		Command& operator<< (const std::map<double, std::string> &m);

	private:
		std::vector<std::string> mArgs;

		const static std::string kRedisCommand_Array_Prefix;
		const static std::string kRedisCommand_String_Prefix;
		const static std::string kRedisCommand_Split;
	};
}


#endif // !_REDIS_CLIENT_COMMAND_H_
