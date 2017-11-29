

#include "Command.h"

#include <sstream>

using namespace redis_client;


const std::string Command::kRedisCommand_Array_Prefix = "*";
const std::string Command::kRedisCommand_String_Prefix = "$";
const std::string Command::kRedisCommand_Split = "\r\n";


Command::Command()
{
}

Command::Command(std::string & cmd)
{
	(*this) << cmd;
}

Command::Command(const char * cmd)
{
	Command(std::string(cmd));
}

Command::operator std::string()
{
	return ToString();
}

std::string Command::ToString()
{
	std::stringstream ss;
	int len = mArgs.size();
	ss << kRedisCommand_Array_Prefix << len << kRedisCommand_Split;
	for (int i = 0; i < len; i++) {
		ss << kRedisCommand_String_Prefix << mArgs[i].size() << kRedisCommand_Split << mArgs[i] << kRedisCommand_Split;
	}

	return ss.str();
}

Command & Command::operator<<(int8_t c)
{
	operator<<(std::to_string(c));
	return *this;
}

Command & Command::operator<<(uint8_t c)
{
	operator<<(std::to_string(c));
	return *this;
}

Command & Command::operator<<(int16_t s)
{
	operator<<(std::to_string(s));
	return *this;
}

Command & Command::operator<<(uint16_t s)
{
	operator<<(std::to_string(s));
	return *this;
}

Command & Command::operator<<(int32_t i)
{
	operator<<(std::to_string(i));
	return *this;
}

Command & Command::operator<<(uint32_t i)
{
	operator<<(std::to_string(i));
	return *this;
}

Command & Command::operator<<(int64_t l)
{
	operator<<(std::to_string(l));
	return *this;
}

Command & Command::operator<<(uint64_t l)
{
	operator<<(std::to_string(l));
	return *this;
}

Command & Command::operator<<(float f)
{
	operator<<(std::to_string(f));
	return *this;
}

Command & Command::operator<<(double d)
{
	operator<<(std::to_string(d));
	return *this;
}

Command& Command::operator<<(const std::string &s)
{
	if (s.size() > 0)
		mArgs.push_back(s);
	return *this;
}

Command& Command::operator<<(const char *str)
{
	operator<<(std::string(str));
	return *this;
}

Command & Command::operator<<(const std::vector<std::string>& v)
{
	int sz = v.size();
	for (int i = 0; i < sz; i++)
		(*this) << v[i];
	return *this;
}

Command & Command::operator<<(const std::vector<double>& v)
{
	int sz = v.size();
	for (int i = 0; i < sz; i++)
		(*this) << v[i];
	return *this;
}

Command & redis_client::Command::operator<<(const std::map<std::string, std::string>& m)
{
	for (auto it = m.begin(); it != m.end(); it++) {
		(*this) << it->first << it->second;
	}
	return *this;
}

Command & redis_client::Command::operator<<(const std::map<double, std::string>& m)
{
	for (auto it = m.begin(); it != m.end(); it++) {
		(*this) << it->first << it->second;
	}
	return *this;
}