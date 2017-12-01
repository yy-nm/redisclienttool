


#include "responseparser.h"

#include <string.h>

#include <vector>
#include <string>
#include <set>
#include <map>

#include "exception.h"
#include "commandhelper.h"

#include <sys/types.h>

#include <fcntl.h>
#include <errno.h>

#include <signal.h>

#ifndef _WIN32
#include <unistd.h>
#include <sys/mman.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>

#define SOCKET_ERROR (-1)

#else
#include <WinSock2.h>
#include <WS2tcpip.h>
typedef int socklen_t;
#pragma comment(lib, "ws2_32.lib")
#endif // !_WIN32


using namespace std;
using namespace redis_client;


void _initsocket();
void _clearupsocket();
void _close(int socketfd);
void _setkeepalive(int socketfd);


void printResult(ResponseParser &result)
{
	try {
		switch (result.GetType())
		{
		case ResponseParser::kType_SimpleString:
		case ResponseParser::kType_BulkString:
			printf("type: string, value: %s\n", result.GetValueString().c_str());
			break;
		case ResponseParser::kType_Integer:
			printf("type: int, value: %ld\n", result.GetValueInteger());
			break;
		case ResponseParser::kType_Array:
			printf("type: array, count: %ld\n", result.GetValueArrayLength());
			for (int i = 0; i < result.GetValueArrayLength(); i++) {
				ResponseParser element = result[i];
				printf("element %d: ", i);
				printResult(element);
			}
			break;
		case ResponseParser::kType_Nil:
			printf("type: nil\n");
			break;
		case ResponseParser::kType_Error:
			printf("type: error, value: %s\n", result.GetValueError().c_str());
			break;
		default:
			printf("unknown type\n");
			break;
		}
	}
	catch (RedisException) {
		printf("parse fail\n");
	}
	
}

int main(void)
{
	// testvector
	/*
	vector<string> testvector = {
		"+OK\r\n",
		"-Error message\r\n",
		"-ERR unknown command 'foobar'",
		"-WRONGTYPE Operation against a key holding the wrong kind of value",
		":1000\r\n",
		"$6\r\nfoobar\r\n",
		"$0\r\n\r\n",
		"$-1\r\n",
		"*0\r\n",
		"*2\r\n$3\r\nfoo\r\n$3\r\nbar\r\n",
		"*3\r\n:1\r\n:2\r\n:3\r\n",
		"*2\r\n*3\r\n:1\r\n:2\r\n:3\r\n*2\r\n+Foo\r\n-Bar\r\n",
		"*3\r\n$3\r\nfoo\r\n$-1\r\n$3\r\nbar\r\n",
	};

	map<int, bool> testlengthcheckout = {
		{2, true},
		{3, true},
	};


	for (int i = 0; i < testvector.size(); i++) {
		if (0 == ResponseParser::CheckContentIsAvailableForParse(testvector[i]) && 0 == testlengthcheckout.count(i)) {
			printf("check fail in \"%s\"\n", testvector[i].c_str());
		}
	}

	for (int i = 0; i < testvector.size(); i++) {
		ResponseParser &rp = ResponseParser::ParseResponse(testvector[i]);
		printResult(rp);
	}
	*/

	// redis command test
	_initsocket();

	const char *ip = "127.0.0.1";
	short port = 6379;
	
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd == -1) {
		printf("socket err");
		return -1;
	}

	//_setkeepalive(socketfd);

	// get real socket addr
	struct sockaddr_in addrv4;
	memset(&addrv4, 0, sizeof(addrv4));
	
	if (inet_pton(AF_INET, ip, &addrv4.sin_addr) == 0) {
		_close(socketfd);
		_clearupsocket();
		printf("socket convert ip error");
		return -1;
	}

	addrv4.sin_family = AF_INET;
	addrv4.sin_port = htons(port);


	int ret = connect(socketfd, (struct sockaddr *)&addrv4, sizeof(addrv4));
	if (ret == -1) {
		_close(socketfd);
		_clearupsocket();
		printf("socket connect err, %d\n", errno);
		return -1;
	}

	char buf[1024];
	std::string str;
	size_t len;
	//str = CommandHelper::Echo("hello world");
	//str = CommandHelper::Ping();
	//str = CommandHelper::Del({ "12", "123", "1234" });
	str = CommandHelper::Command();

	ret = send(socketfd, str.c_str(), str.size(), 0);
	if (ret == -1) {
		_close(socketfd);
		_clearupsocket();
		printf("socket send err");
		return -1;
	}

	std::string content;
	while (true) {

		ret = recv(socketfd, buf, sizeof(buf), 0);
		if (ret == -1) {
			_close(socketfd);
			_clearupsocket();
			printf("socket recv err");
			return -1;
		}
		content = content + string(buf, ret);
		len = ResponseParser::CheckContentIsAvailableForParse(content);
		if (len > 0) {
			ResponseParser rp = ResponseParser::ParseResponse(content.substr(0, len));
			printResult(rp);
			break;
		}
	}
	

	_close(socketfd);
	_clearupsocket();

	return 0;
}




void _initsocket()
{
#ifdef _WIN32
	do
	{
		WORD wVersionRequested = MAKEWORD(1, 1);
		WSADATA wsaData = { 0 };
		// WinSock init
		int ret = WSAStartup(wVersionRequested, &wsaData);

		if (ret != 0)
		{
			printf("WSAStartup() failed!\n");
			break;
		}

		// Confirm the WinSock DLL version 2.2
		if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
		{
			WSACleanup();
			printf("Invalid WinSock version!\n");
			break;
		}

		printf("WinSock init success!\n");
	} while (0);
#endif
}

void _clearupsocket()
{
#ifdef _WIN32
	WSACleanup();
#endif
}


void _close(int socketfd)
{
#ifdef _WIN32
	closesocket(socketfd);
#else
	close(socketfd);
#endif // _WIN32
}

void _setkeepalive(int socketfd)
{
	unsigned int keepalive = 1;
	int ret = setsockopt(socketfd, IPPROTO_TCP, SO_KEEPALIVE, (char *)&keepalive, sizeof(keepalive));
}