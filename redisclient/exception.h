

// add by mardyu 2017-11-27
// this file is used to define exception classes

#ifndef _REDIS_CLIENT_EXCEPTION_H_
#define _REDIS_CLIENT_EXCEPTION_H_


#include <string>


namespace redis_client
{
	class RedisException : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return "RedisException";
		}
	};

	class RedisErrorTypeValueException : public RedisException
	{
	public:
		virtual const char* what() const throw()
		{
			return "Get Value From Error Type";
		}
	};

	class RedisIndexOutOfBoundException : public RedisException
	{
	public:
		virtual const char* what() const throw()
		{
			return "Index Out Of Array Value Size";
		}
	};

	class RedisInvalidFormatException : public RedisException
	{
	public:
		virtual const char* what() const throw()
		{
			return "Invalid Format for Redis";
		}
	};
}

#endif // !_REDIS_CLIENT_EXCEPTION_H_

