


#include <responseparser.h>

#include <vector>
#include <string>
#include <set>
#include <map>


using namespace std;
using namespace redis_client;


int main(void)
{
	
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

	return 0;
}