

#include "commandhelper.h"

#include "command.h"


using namespace redis_client;


const vector<string> CommandHelper::kEmptyVector = vector<string>();
const string CommandHelper::kEmptyString = string();




string CommandHelper::Eval(const string & scrpt, const vector<string>& keys, const vector<string>& args)
{
	redis_client::Command c("EVAL");
	c << scrpt << keys.size();
	for (size_t i = 0; i < keys.size(); i++) {
		c << keys[i];
	}

	for (size_t i = 0; i < args.size(); i++) {
		c << args[i];
	}
	return c;
}

string CommandHelper::EvalSHA(const string & sha1, const vector<string>& keys, const vector<string>& args)
{
	redis_client::Command c("EVALSHA");
	c << sha1 << keys.size();
	for (size_t i = 0; i < keys.size(); i++) {
		c << keys[i];
	}

	for (size_t i = 0; i < args.size(); i++) {
		c << args[i];
	}
	return c;
}

string redis_client::CommandHelper::ScriptDebug(CommandHelper::ScriptDebugMode mode)
{
	redis_client::Command c("SCRIPT");
	c << "DEBUG";
	switch (mode)
	{
	case redis_client::CommandHelper::YES:
		c << "YES";
		break;
	case redis_client::CommandHelper::SYNC:
		c << "SYNC";
		break;
	case redis_client::CommandHelper::NO:
		c << "NO";
		break;
	}

	return c;
}

string redis_client::CommandHelper::ScriptExists(const vector<string>& sha1s)
{
	return redis_client::Command("SCRIPT") << "EXISTS" << sha1s;
}

string redis_client::CommandHelper::ScriptFlush()
{
	return redis_client::Command("SCRIPT") << "FLUSH";
}

string redis_client::CommandHelper::ScriptKill()
{
	return redis_client::Command("SCRIPT") << "KILL";
}

string redis_client::CommandHelper::ScriptLoad(const string & script)
{
	return redis_client::Command("SCRIPT") << "LOAD" << script;
}

string redis_client::CommandHelper::Del(const vector<string>& keys)
{
	redis_client::Command c("DEL");
	return c << keys;
}

string redis_client::CommandHelper::Dump(const string & key)
{
	return redis_client::Command("DUMP") << key;
}

string redis_client::CommandHelper::Exists(const vector<string>& keys)
{
	return redis_client::Command("EXISTS") << keys;
}

string redis_client::CommandHelper::Expire(const string & key, int seconds)
{
	return redis_client::Command("EXPIRE") << key << seconds;
}

string redis_client::CommandHelper::ExpireAt(const string & key, int timestamp)
{
	return redis_client::Command("EXPIREAT") << key << timestamp;
}

string redis_client::CommandHelper::Keys(const string & pattern)
{
	return redis_client::Command("KEYS") << pattern;
}

string redis_client::CommandHelper::Migrate(const string & host, int port, const string & key, int & db, int timeout, bool copy, bool replace, const vector<string>& keys)
{
	redis_client::Command c("MIGRATE");
	c << host << port << key << db << timeout;
	if (copy) {
		c << "COPY";
	}
	if (replace) {
		c << "REPLACE";
	}

	if (keys.size() > 0) {
		c << "KEYS ";
		c << keys;
	}
	return c;
}

string redis_client::CommandHelper::Move(const string & key, int db)
{
	return redis_client::Command("MOVE") << key << db;
}

string redis_client::CommandHelper::Persist(const string & key)
{
	return redis_client::Command("PERSIST") << key;
}

string redis_client::CommandHelper::PExpipe(const string & key, int64_t milliseconds)
{
	return redis_client::Command("PEXPIPE") << key << milliseconds;
}

string redis_client::CommandHelper::PExpipeAt(const string & key, int64_t milliseconds_timestamp)
{
	return redis_client::Command("PEXPIPEAT") << key << milliseconds_timestamp;
}

string redis_client::CommandHelper::PTTL(const string & key)
{
	return redis_client::Command("PTTL") << key;
}

string redis_client::CommandHelper::RandomKey()
{
	return redis_client::Command("RANDOMKEY");
}

string redis_client::CommandHelper::Rename(const string & key, const string & newkey)
{
	return redis_client::Command("RENAME") << key << newkey;
}

string redis_client::CommandHelper::RenameNX(const string & key, const string & newkey)
{
	return redis_client::Command("RENAMENX") << key << newkey;
}

string redis_client::CommandHelper::Restore(const string & key, int ttl, const string & serialvalue, bool replace)
{
	redis_client::Command c("RESTORE");
	c << key << ttl << serialvalue;
	if (replace) {
		c << "REPLACE";
	}
	return c;
}

string redis_client::CommandHelper::Sort(const string & key, const string & bypattern, const string & dst
	, int limitoffset, int limitcount, const vector<string> &getpatterns, CommandHelper::SortType sorttype, bool alpha)
{
	redis_client::Command c("SORT");
	c << key;

	if (bypattern != kEmptyString) {
		c << "BY" << bypattern;
	}

	if (limitcount != 0) {
		c << "LIMIT" << limitoffset << limitcount;
	}

	for (size_t i = 0; i < getpatterns.size(); i++) {
		c << "GET" << getpatterns[i];
	}

	switch (sorttype)
	{
	case redis_client::CommandHelper::ASC:
		c << "ASC";
		break;
	case redis_client::CommandHelper::DESC:
		c << "DESC";
		break;
	}

	if (alpha) {
		c << "ALPHA";
	}

	if (dst != kEmptyString) {
		c << "STORE" << dst;
	}

	return c;
}

string redis_client::CommandHelper::Touch(const vector<string>& keys)
{
	return redis_client::Command("TOUCH") << keys;
}

string redis_client::CommandHelper::TTL(const string & key)
{
	return redis_client::Command("TTL") << key;
}

string redis_client::CommandHelper::Type(const string & key)
{
	return redis_client::Command("TYPE") << key;
}

string redis_client::CommandHelper::Unlink(const vector<string>& keys)
{
	return redis_client::Command("UNLINK") << keys;
}

string redis_client::CommandHelper::Scan(int cursor, const string & pattern, int count)
{
	redis_client::Command c("SCAN");
	c << cursor;
	if (pattern != kEmptyString) {
		c << "MATCH" << pattern;
	}

	if (count != 0)
		c << "COUNT" << count;
	return c;
}

string redis_client::CommandHelper::Wait(int numslaves, int timeout)
{
	return redis_client::Command("WAIT") << numslaves << timeout;
}

string redis_client::CommandHelper::Object(CommandHelper::ObjectSubCommandType type, const vector<string>& args)
{
	redis_client::Command c("OBJECT");
	switch (type)
	{
	case redis_client::CommandHelper::REFCOUNT:
		c << "REFCOUNT";
		break;
	case redis_client::CommandHelper::ENCODING:
		c << "ENCODING";
		break;
	case redis_client::CommandHelper::IDLETIME:
		c << "IDLETIME";
		break;
	case redis_client::CommandHelper::FREQ:
		c << "FREQ";
		break;
	}

	return c << args;
}

string redis_client::CommandHelper::HDel(const string & key, const vector<string>& fields)
{
	return redis_client::Command("HDEL") << key << fields;
}

string redis_client::CommandHelper::HExists(const string & key, const string & field)
{
	return redis_client::Command("HEXISTS") << key << field;
}

string redis_client::CommandHelper::HGet(const string & key, const string & field)
{
	return redis_client::Command("HGET") << key << field;
}

string redis_client::CommandHelper::HGetAll(const string & key)
{
	return redis_client::Command("HGETALL") << key;
}

string redis_client::CommandHelper::HIncrBy(const string & key, const string & field, int increment)
{
	return redis_client::Command("HINCRBY") << key << field << increment;
}

string redis_client::CommandHelper::HIncrByFloat(const string & key, const string & field, float increment)
{
	return redis_client::Command("HINCRBYFLOAT") << key << field << increment;
}

string redis_client::CommandHelper::HKeys(const string & key)
{
	return redis_client::Command("HKEYS") << key;
}

string redis_client::CommandHelper::HLen(const string & key)
{
	return redis_client::Command("HLEN") << key;
}

string redis_client::CommandHelper::HSet(const string & key, const string & field, const string & value)
{
	return redis_client::Command("HSET") << key << field << value;
}

string redis_client::CommandHelper::HSetNX(const string & key, const string & field, const string & value)
{
	return redis_client::Command("HSETNX") << key << field << value;
}

string redis_client::CommandHelper::HStrLen(const string & key, const string & field)
{
	return redis_client::Command("HSTRLEN") << key << field;
}

string CommandHelper::HValues(const string & key)
{
	redis_client::Command c("HVALS");
	return c << key;
}

string redis_client::CommandHelper::HMGet(const string & key, const vector<string>& fields)
{
	return redis_client::Command("HMGET") << key << fields;
}

string redis_client::CommandHelper::HMSet(const string & key, map<string, string>& fieldvaluepairs)
{
	return redis_client::Command("HMSET") << key << fieldvaluepairs;
}

string redis_client::CommandHelper::HScan(int cursor, const string & pattern, int count)
{
	redis_client::Command c("HSCAN");
	c << cursor;
	if (pattern != kEmptyString) {
		c << "MATCH" << pattern;
	}

	if (count != 0)
		c << "COUNT" << count;
	return c;
}

string redis_client::CommandHelper::BLPop(const vector<string>& keys, int timeout)
{
	return redis_client::Command("BLPOP") << keys << timeout;
}

string redis_client::CommandHelper::BRPop(const vector<string>& keys, int timeout)
{
	return redis_client::Command("BRPOP") << keys << timeout;
}

string redis_client::CommandHelper::BRPopLPush(const string & src, const string & dst, int timeout)
{
	return redis_client::Command("BRPOPLPUSH") << src << dst << timeout;
}

string redis_client::CommandHelper::LIndex(const string & key, int index)
{
	return redis_client::Command("LINDEX") << key << index;
}

string redis_client::CommandHelper::LInsert(const string & key, bool before, const string & pivot, const string & value)
{
	redis_client::Command c("LINSERT");
	c << key;
	if (before) {
		c << "BEFORE";
	}
	else {
		c << "AFTER";
	}
	return c << pivot << value;
}

string redis_client::CommandHelper::LLen(const string & key)
{
	return redis_client::Command("LLEN") << key;
}

string redis_client::CommandHelper::LPop(const string & key)
{
	return redis_client::Command("LPOP") << key;
}

string redis_client::CommandHelper::LPush(const string & key, const vector<string>& values)
{
	return redis_client::Command("LPUSH") << key << values;
}

string redis_client::CommandHelper::LPushX(const string & key, const string & value)
{
	return redis_client::Command("LPUSHX") << key << value;
}

string redis_client::CommandHelper::LRange(const string & key, int start, int stop)
{
	return redis_client::Command("LRANGE") << key << start << stop;
}

string redis_client::CommandHelper::LRem(const string & key, int count, const string & value)
{
	return redis_client::Command("LREM") << key << count << value;
}

string redis_client::CommandHelper::LSet(const string & key, int index, const string & value)
{
	return redis_client::Command("LSET") << key << index << value;
}

string redis_client::CommandHelper::LTrim(const string & key, int start, int stop)
{
	return redis_client::Command("LTRIM") << key << start << stop;
}

string redis_client::CommandHelper::RPop(const string & key)
{
	return redis_client::Command("RPOP") << key;
}

string redis_client::CommandHelper::RPopLPush(const string & src, const string & dst)
{
	return redis_client::Command("RPOPLPUSH") << src << dst;
}

string redis_client::CommandHelper::RPush(const string & key, const vector<string>& values)
{
	return redis_client::Command("RPUSH") << key << values;
}

string redis_client::CommandHelper::RPushHX(const string & key, const string & value)
{
	return redis_client::Command("RPUSHHX") << key << value;
}

string redis_client::CommandHelper::SAdd(const string & key, const vector<string>& members)
{
	return redis_client::Command("SADD") << key << members;
}

string redis_client::CommandHelper::SCard(const string & key)
{
	return redis_client::Command("SCARD") << key;
}

string redis_client::CommandHelper::SDiff(const vector<string>& keys)
{
	return redis_client::Command("SDIFF") << keys;
}

string redis_client::CommandHelper::SDiffStore(const string & dst, const vector<string>& keys)
{
	return redis_client::Command("SDIFFSTORE") << dst << keys;
}

string redis_client::CommandHelper::SInter(const vector<string>& keys)
{
	return redis_client::Command("SINTER") << keys;
}

string redis_client::CommandHelper::SInterStore(const string & dst, const vector<string>& keys)
{
	return redis_client::Command("SINTERSTORE") << dst << keys;
}

string redis_client::CommandHelper::SIsMember(const string & key, const string & member)
{
	return redis_client::Command("SISMEMBER") << key << member;
}

string redis_client::CommandHelper::SMembers(const string & key)
{
	return redis_client::Command("SMEMBER") << key;
}

string redis_client::CommandHelper::SMove(const string & src, const string & dst, const string & member)
{
	return redis_client::Command("SMOVE") << src << dst << member;
}

string redis_client::CommandHelper::SPop(const string & key, int count)
{
	redis_client::Command c("SPOP");
	c << key;

	if (count > 1) {
		c << count;
	}

	return c;
}

string redis_client::CommandHelper::SRandMember(const string & key, int count)
{
	redis_client::Command c("SRANDMEMBER");
	c << key;

	if (count > 1) {
		c << count;
	}

	return c;
}

string redis_client::CommandHelper::SRem(const string & key, const vector<string>& members)
{
	return redis_client::Command("SREM") << key << members;
}

string redis_client::CommandHelper::SUnion(const vector<string>& keys)
{
	return redis_client::Command("SUNION") << keys;
}

string redis_client::CommandHelper::SUnionStore(const string & dst, const vector<string>& keys)
{
	return redis_client::Command("SUNION") << dst << keys;
}

string redis_client::CommandHelper::SScan(int cursor, const string & pattern, int count)
{
	redis_client::Command c("SSCAN");
	c << cursor;
	if (pattern != kEmptyString) {
		c << "MATCH" << pattern;
	}

	if (count != 0)
		c << "COUNT" << count;
	return c;
}

string redis_client::CommandHelper::ZAdd(const string & key, const map<double, string>& scoremembers, Mode mode, bool ch, bool incr)
{
	redis_client::Command c("ZADD");
	c << key;
	switch (mode)
	{
	case redis_client::CommandHelper::NX:
		c << "NX";
		break;
	case redis_client::CommandHelper::XX:
		c << "XX";
		break;
	}
	if (ch) {
		c << "CH";
	}
	if (incr) {
		c << "INCR";
	}

	return c << scoremembers;
}

string redis_client::CommandHelper::ZCard(const string & key)
{
	return redis_client::Command("ZCARD") << key;
}

string redis_client::CommandHelper::ZCount(const string & key, double min, double max)
{
	return redis_client::Command("ZCOUNT") << key << min << max;
}

string redis_client::CommandHelper::ZIncreBy(const string & key, double increment, const string & member)
{
	return redis_client::Command("ZINCRBY") << key << increment << member;
}

string redis_client::CommandHelper::ZInterStore(const string & dst, const vector<string>& keys, const vector<double>& weights, AggregateType aggtype)
{
	redis_client::Command c("ZINTERSTORE");
	c << dst << keys.size() << keys;
	if (weights.size() > 0) {
		c << "WEIGHTS" << weights;
	}

	switch (aggtype)
	{
	case redis_client::CommandHelper::SUM:
		c << "AGGREGATE" << "SUM";
		break;
	case redis_client::CommandHelper::MIN:
		c << "AGGREGATE" << "MIN";
		break;
	case redis_client::CommandHelper::MAX:
		c << "AGGREGATE" << "MAX";
		break;
	}

	return c;
}

string redis_client::CommandHelper::ZLexCount(const string & key, const string & min, const string & max)
{
	return redis_client::Command("ZLEXCOUNT") << key << min << max;
}

string redis_client::CommandHelper::ZRange(const string & key, int start, int stop, bool withscores)
{
	redis_client::Command c("ZRANGE");
	c << key << start << stop;
	if (withscores) {
		c << "WITHSCORES";
	}
	return c;
}

string redis_client::CommandHelper::ZRangeByLex(const string & key, const string & min, const string & max, int offset, int count)
{
	redis_client::Command c("ZRANGEBYLEX");
	c << key << min << max;
	if (count != 0) {
		c << "LIMIT" << offset << count;
	}

	return c;
}

string redis_client::CommandHelper::ZRevRangeByLex(const string & key, const string & max, const string & min, int offset, int count)
{
	redis_client::Command c("ZREVRANGEBYLEX");
	c << key << min << max;
	if (count != 0) {
		c << "LIMIT" << offset << count;
	}

	return c;
}

string redis_client::CommandHelper::ZRangeByScore(const string & key, double min, double max, bool withscores, int offset, int count)
{
	redis_client::Command c("ZRANGEBYSCORE");
	c << key << min << max;
	if (withscores) {
		c << "WITHSCORES";
	}
	if (count != 0) {
		c << "LIMIT" << offset << count;
	}

	return c;
}

string redis_client::CommandHelper::ZRank(const string & key, const string & member)
{
	return redis_client::Command("ZRANK") << key << member;
}

string redis_client::CommandHelper::ZRem(const string & key, const vector<string>& members)
{
	return redis_client::Command("ZREM") << key << members;
}

string redis_client::CommandHelper::ZRemRangeByLex(const string & key, const string & min, const string & max)
{
	return redis_client::Command("ZREMRANGEBYLEX") << key << min << max;
}

string redis_client::CommandHelper::ZRemRangeByRank(const string & key, int start, int stop)
{
	return redis_client::Command("ZREMRANGEBYRANK") << key << start<< stop;
}

string redis_client::CommandHelper::ZRemRangeByScore(const string & key, double min, double max)
{
	return redis_client::Command("ZREMRANGEBYSCORE") << key << min << max;
}

string redis_client::CommandHelper::ZRevRange(const string & key, int start, int stop, bool withscores)
{
	redis_client::Command c("ZREVRANGE");
	c << key << start << stop;
	if (withscores) {
		c << "WITHSCORES";
	}

	return c;
}

string redis_client::CommandHelper::ZRevRangeByScore(const string & key, double max, double min, bool withscores, int offset, int count)
{
	redis_client::Command c("ZREVRANGEBYSCORE");
	c << key << max << min;
	if (withscores) {
		c << "WITHSCORES";
	}
	if (count != 0) {
		c << "LIMIT" << offset << count;
	}

	return c;
}

string redis_client::CommandHelper::ZRevRank(const string & key, const string & member)
{
	return redis_client::Command("ZREVRANK") << key << member;
}

string redis_client::CommandHelper::ZScore(const string & key, const string & member)
{
	return redis_client::Command("ZSCORE") << key << member;
}

string redis_client::CommandHelper::ZUnionStore(const string & dst, const vector<string>& keys, const vector<double>& weights
	, AggregateType aggtype)
{
	redis_client::Command c("ZUNIONSTORE");
	c << dst << keys.size() << keys;
	if (weights.size() > 0) {
		c << "WEIGHTS" << weights;
	}

	switch (aggtype)
	{
	case redis_client::CommandHelper::SUM:
		c << "AGGREGATE" << "SUM";
		break;
	case redis_client::CommandHelper::MIN:
		c << "AGGREGATE" << "MIN";
		break;
	case redis_client::CommandHelper::MAX:
		c << "AGGREGATE" << "MAX";
		break;
	}

	return c;
}

string redis_client::CommandHelper::ZScan(int cursor, const string & pattern, int count)
{
	redis_client::Command c("ZSCAN");
	c << cursor;
	if (pattern != kEmptyString) {
		c << "MATCH" << pattern;
	}

	if (count != 0)
		c << "COUNT" << count;
	return c;
}

string redis_client::CommandHelper::Auth(const string & password)
{
	return redis_client::Command("AUTH") << password;
}

string redis_client::CommandHelper::Quit()
{
	return redis_client::Command("QUIT");
}

string redis_client::CommandHelper::Select(int index)
{
	return redis_client::Command("SELECT") << index;
}

string redis_client::CommandHelper::SwapDB(int index1, int index2)
{
	return redis_client::Command("SWAPDB") << index1 << index2;
}

string redis_client::CommandHelper::Ping(const string & msg)
{
	redis_client::Command c("PING");
	if (msg != kEmptyString)
		c << msg;

	return c;
}

string redis_client::CommandHelper::Echo(const string & msg)
{
	return redis_client::Command("ECHO") << msg;
}

string redis_client::CommandHelper::Append(const string & key, const string & value)
{
	return redis_client::Command("APPEND") << key << value;
}

string redis_client::CommandHelper::Decr(const string & key)
{
	return redis_client::Command("DECR") << key;
}

string redis_client::CommandHelper::DecrBy(const string & key, int decrement)
{
	return redis_client::Command("DECRBY") << key << decrement;
}

string redis_client::CommandHelper::Get(const string & key)
{
	return redis_client::Command("GET") << key;
}

string redis_client::CommandHelper::GetRange(const string & key, int start, int end)
{
	return redis_client::Command("GETRANGE") << key << start << end;
}

string redis_client::CommandHelper::GetSet(const string & key, string & value)
{
	return redis_client::Command("GETSET") << key << value;
}

string redis_client::CommandHelper::Incr(const string & key)
{
	return redis_client::Command("INCR") << key;
}

string redis_client::CommandHelper::incrBy(const string & key, int increment)
{
	return redis_client::Command("INCRBY") << key << increment;
}

string redis_client::CommandHelper::incrByFloat(const string & key, float increment)
{
	return redis_client::Command("INCRBYFLOAT") << key << increment;
}

string redis_client::CommandHelper::MGet(const vector<string>& keys)
{
	return redis_client::Command("MGET") << keys;
}

string redis_client::CommandHelper::MSet(const map<string, string>& kvs)
{
	return redis_client::Command("MSET") << kvs;
}

string redis_client::CommandHelper::MSetNX(const map<string, string>& kvs)
{
	return redis_client::Command("MSETNX") << kvs;
}

string redis_client::CommandHelper::PSetEX(const string & key, int64_t milliseconds, const string & value)
{
	return redis_client::Command("PSETEX") << key << milliseconds << value;
}

string redis_client::CommandHelper::Set(const string & key, const string & value, int seconds, int64_t milliseconds
	, CommandHelper::Mode mode)
{
	redis_client::Command c("SET");
	c << key << value;
	if (seconds != 0) {
		c << "EX" << seconds;
	}
	if (milliseconds != 0) {
		c << "PX" << milliseconds;
	}

	switch (mode)
	{

	case redis_client::CommandHelper::NX:
		c << "NX";
		break;
	case redis_client::CommandHelper::XX:
		c << "XX";
		break;
	}

	return c;
}

string redis_client::CommandHelper::SetEX(const string & key, int seconds, const string & value)
{
	return redis_client::Command("SETEX") << key << seconds << value;
}

string redis_client::CommandHelper::SetNX(const string & key, const string & value)
{
	return redis_client::Command("SETNX") << key << value;
}

string redis_client::CommandHelper::STRLen(const string & key)
{
	return redis_client::Command("STRLEN") << key;
}

string redis_client::CommandHelper::BitCount(const string & key)
{
	return redis_client::Command("BITCOUNT") << key;
}

string redis_client::CommandHelper::BitCount(const string & key, const string & start, const string & end)
{
	return redis_client::Command("BITCOUNT") << key << start << end;
}

string redis_client::CommandHelper::BitField_Get(const string & key, const string & type, int offset)
{
	redis_client::Command c("BITFIELD");
	c << key << "GET" << type << offset;
	return c;
}

string redis_client::CommandHelper::BitField_Set(const string & key, const string & type, int offset, int value)
{
	redis_client::Command c("BITFIELD");
	c << key << "SET" << type << offset << value;
	return c;
}

string redis_client::CommandHelper::BitField_IncrBy(const string & key, const string & type, int offset, int increment
	, CommandHelper::OverflowType overflowtype)
{
	redis_client::Command c("BITFIELD");
	c << key << "INCRBY" << type << offset << increment;
	switch (overflowtype)
	{
	case redis_client::CommandHelper::WRAP:
		c << "OVERFLOW" << "WRAP";
		break;
	case redis_client::CommandHelper::SAT:
		c << "OVERFLOW" << "SAT";
		break;
	case redis_client::CommandHelper::FAIL:
		c << "OVERFLOW" << "FAIL";
		break;
	}
	return c;
}

string redis_client::CommandHelper::BitOP(CommandHelper::BitOPType optype, const string & destkey, const vector<string>& keys)
{
	redis_client::Command c("BITOP");
	switch (optype)
	{
	case redis_client::CommandHelper::AND:
		c << "AND";
		break;
	case redis_client::CommandHelper::OR:
		c << "OR";
		break;
	case redis_client::CommandHelper::XOR:
		c << "XOR";
		break;
	case redis_client::CommandHelper::NOT:
		c << "NOT";
		break;
	}
	c << destkey << keys;
	return c;
}

string redis_client::CommandHelper::BitPos(const string & key, int bit)
{
	return redis_client::Command("BITPOS") << key << bit;
}

string redis_client::CommandHelper::BitPos(const string & key, int bit, int start, int end)
{
	return redis_client::Command("BITPOS") << key << bit << start << end;
}

string redis_client::CommandHelper::GetBit(const string & key, int offset)
{
	return redis_client::Command("GETBIT") << key << offset;
}

string redis_client::CommandHelper::SetBit(const string & key, int offset, int value)
{
	return redis_client::Command("SETBIT") << key << offset << value;
}

string redis_client::CommandHelper::SetRange(const string & key, int offset, int value)
{
	return redis_client::Command("SETRANGE") << key << offset << value;
}

string redis_client::CommandHelper::ClusterAddSlots(const vector<string>& slots)
{
	return redis_client::Command("CLUSTER") << "ADDSLOTS" << slots;
}

string redis_client::CommandHelper::ClusterCountFailureReport(const string & nodeid)
{
	return redis_client::Command("CLUSTER") << "COUNT-FAILURE-REPORTS" << nodeid;
}

string redis_client::CommandHelper::ClusterCountKeysInSlot(const string & slot)
{
	return redis_client::Command("CLUSTER") << "COUNTKEYSINSLOT" << slot;
}

string redis_client::CommandHelper::ClusterDelSlots(const vector<string>& slots)
{
	return redis_client::Command("CLUSTER") << "DELSLOTS" << slots;
}

string redis_client::CommandHelper::ClusterFailOver(CommandHelper::ClusterFailOverType type)
{
	redis_client::Command c("CLUSTER");
	c << "FAILOVER";
	switch (type)
	{
	case redis_client::CommandHelper::FORCE:
		c << "FORCE";
		break;
	case redis_client::CommandHelper::TAKEOVER:
		c << "TAKEOVER";
		break;
	}

	return c;
}

string redis_client::CommandHelper::ClusterForget(const string & nodeid)
{
	return redis_client::Command("CLUSTER") << "FORGET" << nodeid;
}

string redis_client::CommandHelper::ClusterGetKeysInSlot(string slot, int count)
{
	return redis_client::Command("CLUSTER") << "GETKEYSINSLOT" << slot << count;
}

string redis_client::CommandHelper::ClusterInfo()
{
	return redis_client::Command("CLUSTER") << "INFO";
}

string redis_client::CommandHelper::ClusterKeySlot(const string & key)
{
	return redis_client::Command("CLUSTER") << "KEYSLOT" << key;
}

string redis_client::CommandHelper::ClusterMeet(const string & ip, int port)
{
	return redis_client::Command("CLUSTER") << "MEET" << ip << port;
}

string redis_client::CommandHelper::ClusterNodes()
{
	return redis_client::Command("CLUSTER") << "NODES";
}

string redis_client::CommandHelper::ClusterReplicate(const string & nodeid)
{
	return redis_client::Command("CLUSTER") << "REPLICATE" << nodeid;
}

string redis_client::CommandHelper::ClusterReset(CommandHelper::ClusterResetType type)
{
	redis_client::Command c("CLUSTER");
	c << "RESET";
	switch (type)
	{
	case redis_client::CommandHelper::HARD:
		c << "HARD";
		break;
	case redis_client::CommandHelper::SOFT:
		c << "SOFT";
		break;
	}
	return c;
}

string redis_client::CommandHelper::ClusterSaveConfig()
{
	return redis_client::Command("CLUSTER") << "SAVECONFIG";
}

string redis_client::CommandHelper::ClusterSetConfigEpoch(const string & configepoch)
{
	return redis_client::Command("CLUSTER") << "SET-CONFIG-EPOCH" << configepoch;
}

string redis_client::CommandHelper::ClusterSetSlot_Migrating(const string & slot, const string & dstnodeid)
{
	return redis_client::Command("CLUSTER") << "SETSLOT" << slot << "MIGRATING" << dstnodeid;
}

string redis_client::CommandHelper::ClusterSetSlot_Import(const string & slot, const string & srcnodeid)
{
	return redis_client::Command("CLUSTER") << "SETSLOT" << slot << "IMPORTING" << srcnodeid;
}

string redis_client::CommandHelper::ClusterSetSlot_Table(const string & slot)
{
	return redis_client::Command("CLUSTER") << "SETSLOT" << slot << "STABLE";
}

string redis_client::CommandHelper::ClusterSetSlot_Node(const string & slot, const string & nodeid)
{
	return redis_client::Command("CLUSTER") << "SETSLOT" << slot << "NODE" << nodeid;
}

string redis_client::CommandHelper::ClusterSlaves(const string & nodeid)
{
	return redis_client::Command("CLUSTER") << "SLAVES" << nodeid;
}

string redis_client::CommandHelper::CLusterSlots()
{
	return redis_client::Command("CLUSTER") << "SLOTS";
}

string redis_client::CommandHelper::ReadOnly()
{
	return redis_client::Command("READONLY");
}

string redis_client::CommandHelper::ReadWrite()
{
	return redis_client::Command("READWRITE");
}

string redis_client::CommandHelper::Info(const string & section)
{
	redis_client::Command c("INFO");

	if (section != kEmptyString)
		c << section;

	return c;
}

string redis_client::CommandHelper::Monitor()
{
	return redis_client::Command("MONITOR");
}

string redis_client::CommandHelper::Role()
{
	return redis_client::Command("ROLE");
}

string redis_client::CommandHelper::Shutdown()
{
	return redis_client::Command("SHUTDOWN");
}

string redis_client::CommandHelper::Shutdown(bool save)
{
	redis_client::Command c("SHUTDOWN");
	if (save) {
		c << "SAVE";
	}
	else {
		c << "NOSAVE";
	}
	return c;
}

string redis_client::CommandHelper::Slaveof(const string & host, int port)
{
	return redis_client::Command("SLAVEOF") << host << port;
}

string redis_client::CommandHelper::Sync()
{
	return redis_client::Command("SYNC");
}

string redis_client::CommandHelper::Time()
{
	return redis_client::Command("TIME");
}

string redis_client::CommandHelper::BgRewriteAOF()
{
	return redis_client::Command("BGREWRITEAOF");
}

string redis_client::CommandHelper::BgSave()
{
	return redis_client::Command("BGSAVE");
}

string redis_client::CommandHelper::LastSave()
{
	return redis_client::Command("LASTSAVE");
}

string redis_client::CommandHelper::Save()
{
	return redis_client::Command("SAVE");
}

string redis_client::CommandHelper::ClientKill(const string & addr)
{
	return redis_client::Command("CLIENT") << "KILL" << addr;
}

string redis_client::CommandHelper::ClientKill_ClientId(const string & id, CommandHelper::ClientType clienttype, const string & addr
	, bool skipme)
{
	redis_client::Command c("CLIENT");
	if (id != kEmptyString) {
		c << "ID" << id;
	}

	switch (clienttype)
	{
	case redis_client::CommandHelper::NORMAL:
		c << "TYPE" << "normal";
		break;
	case redis_client::CommandHelper::MASTER:
		c << "TYPE" << "master";
		break;
	case redis_client::CommandHelper::SLAVE:
		c << "TYPE" << "slave";
		break;
	case redis_client::CommandHelper::PUBSUB:
		c << "TYPE" << "pubsub";
		break;
	default:
		break;
	}

	if (addr != kEmptyString) {
		c << "ADDR" << addr;
	}

	if (skipme) {
		c << "SKIPME" << "yes";
	}
	else {
		c << "SKIPME" << "no";
	}

	return c;
}

string redis_client::CommandHelper::ClientList()
{
	return redis_client::Command("CLIENT") << "LIST";
}

string redis_client::CommandHelper::ClientGetName()
{
	return redis_client::Command("CLIENT") << "GETNAME";
}

string redis_client::CommandHelper::ClientPause(int timeout)
{
	return redis_client::Command("CLIENT") << "PAUSE" << timeout;
}

string redis_client::CommandHelper::ClientReply(CommandHelper::ClientReplyType replytype)
{
	redis_client::Command c("CLIENT");

	switch (replytype)
	{
	case redis_client::CommandHelper::ON:
		c << "ON";
		break;
	case redis_client::CommandHelper::OFF:
		c << "OFF";
		break;
	case redis_client::CommandHelper::SKIP:
		c << "SKIP";
		break;
	}

	return c;
}

string redis_client::CommandHelper::ClientSetName(const string & name)
{
	return redis_client::Command("CLIENT") << "SETNAME" << name;
}

string redis_client::CommandHelper::Command()
{
	return redis_client::Command("COMMAND");
}

string redis_client::CommandHelper::CommandCount()
{
	return redis_client::Command("COMMAND") << "COUNT";
}

string redis_client::CommandHelper::CommandGetKeys()
{
	return redis_client::Command("COMMAND") << "GETKEYS";
}

string redis_client::CommandHelper::CommandInfo(const vector<string>& cmds)
{
	return redis_client::Command("COMMAND") << "INFO" << cmds;
}

string redis_client::CommandHelper::ConfigGet(const string & parameter)
{
	return redis_client::Command("CONFIG") << "GET" << parameter;
}

string redis_client::CommandHelper::ConfigRewrite()
{
	return redis_client::Command("CONFIG") << "REWRITE";
}

string redis_client::CommandHelper::ConfigSet(const string & parameter, const string & value)
{
	return redis_client::Command("CONFIG") << "SET" << parameter << value;
}

string redis_client::CommandHelper::ConfigResetStat()
{
	return redis_client::Command("CONFIG") << "RESETSTAT";
}

string redis_client::CommandHelper::DebugObject(const string & key)
{
	return redis_client::Command("DEBUG") << "OBJECT" << key;
}

string redis_client::CommandHelper::DebugSegfault()
{
	return redis_client::Command("DEBUG") << "SEGFAULT";
}

string redis_client::CommandHelper::DBSize()
{
	return redis_client::Command("DBSIZE");
}

string redis_client::CommandHelper::FlushAll(bool async)
{
	redis_client::Command c("FLUSHALL");

	if (async) {
		c << "ASYNC";
	}
	return c;
}

string redis_client::CommandHelper::FlushDB(bool async)
{
	redis_client::Command c("FLUSHDB");

	if (async) {
		c << "ASYNC";
	}
	return c;
}

string redis_client::CommandHelper::SlowlogGet(int count)
{
	return redis_client::Command("SLOWLOG") << "GET" << count;
}

string redis_client::CommandHelper::SlowlogLen()
{
	return redis_client::Command("SLOWLOG") << "LEN";
}

string redis_client::CommandHelper::SlowlogReset()
{
	return redis_client::Command("SLOWLOG") << "RESET";
}

string redis_client::CommandHelper::Discard()
{
	return redis_client::Command("DISCARD");
}

string redis_client::CommandHelper::Exec()
{
	return redis_client::Command("EXEC");
}

string redis_client::CommandHelper::Multi()
{
	return redis_client::Command("MULTI");
}

string redis_client::CommandHelper::Watch(const vector<string>& keys)
{
	return redis_client::Command("WATCH") << keys;
}

string redis_client::CommandHelper::Unwatch()
{
	return redis_client::Command("UNWATCH");
}

string redis_client::CommandHelper::GeoAdd(const string & key, const vector<GeoSpatialItem>& items)
{
	redis_client::Command c("GEOADD");
	c << key;

	for (size_t i = 0; i < items.size(); i++) {
		c << items[i].longitude << items[i].latitude << items[i].name;
	}

	return c;
}

string redis_client::CommandHelper::GeHash(const string & key, const vector<string>& members)
{
	return redis_client::Command("GEOHASH") << key << members;
}

string redis_client::CommandHelper::GePos(const string & key, const vector<string>& members)
{
	return redis_client::Command("GEOPOS") << key << members;
}

string redis_client::CommandHelper::GeDist(const string & key, const string & member1, const string & member2, CommandHelper::GeoDistanceUnit unit)
{
	redis_client::Command c("GEODIST");
	c << key << member1 << member2;
	switch (unit)
	{
	case redis_client::CommandHelper::M:
		c << "m";
		break;
	case redis_client::CommandHelper::KM:
		c << "km";
		break;
	case redis_client::CommandHelper::MI:
		c << "mi";
		break;
	case redis_client::CommandHelper::FT:
		c << "ft";
		break;
	}

	return c;
}

string redis_client::CommandHelper::GeoRadius(const string & key, float longitude, float latitude, float radius
	, CommandHelper::GeoDistanceUnit unit, int count, const string & storekey, const string & storedistkey
	, CommandHelper::SortType sorttype, bool withcoord, bool withdist, bool withhash)
{
	redis_client::Command c("GEORADIUS");
	c << key << longitude << latitude << radius;
	switch (unit)
	{
	case redis_client::CommandHelper::M:
		c << "m";
		break;
	case redis_client::CommandHelper::KM:
		c << "km";
		break;
	case redis_client::CommandHelper::MI:
		c << "mi";
		break;
	case redis_client::CommandHelper::FT:
		c << "ft";
		break;
	}
	if (withcoord) {
		c << "WITHCOORD";
	}
	if (withdist) {
		c << "WITHDIST";
	}
	if (withhash) {
		c << "WITHHASH";
	}
	if (count != 0) {
		c << "COUNT" << count;
	}

	switch (sorttype)
	{
	case redis_client::CommandHelper::ASC:
		c << "ASC";
		break;
	case redis_client::CommandHelper::DESC:
		c << "DESC";
		break;
	}

	if (storekey != kEmptyString) {
		c << "STORE" << storekey;
	}
	if (storedistkey != kEmptyString) {
		c << "STOREDIST" << storedistkey;
	}
	return c;
}

string redis_client::CommandHelper::GeoRadiusByMember(const string & key, const string & member, float radius
	, CommandHelper::GeoDistanceUnit unit, int count, const string & storekey, const string & storedistkey
	, CommandHelper::SortType sorttype, bool withcoord, bool withdist, bool withhash)
{
	redis_client::Command c("GEORADIUSBYMEMBER");
	c << key << member << radius;
	switch (unit)
	{
	case redis_client::CommandHelper::M:
		c << "m";
		break;
	case redis_client::CommandHelper::KM:
		c << "km";
		break;
	case redis_client::CommandHelper::MI:
		c << "mi";
		break;
	case redis_client::CommandHelper::FT:
		c << "ft";
		break;
	}
	if (withcoord) {
		c << "WITHCOORD";
	}
	if (withdist) {
		c << "WITHDIST";
	}
	if (withhash) {
		c << "WITHHASH";
	}
	if (count != 0) {
		c << "COUNT" << count;
	}

	switch (sorttype)
	{
	case redis_client::CommandHelper::ASC:
		c << "ASC";
		break;
	case redis_client::CommandHelper::DESC:
		c << "DESC";
		break;
	}

	if (storekey != kEmptyString) {
		c << "STORE" << storekey;
	}
	if (storedistkey != kEmptyString) {
		c << "STOREDIST" << storedistkey;
	}
	return c;
}

string redis_client::CommandHelper::PFAdd(const string & key, const vector<string>& elements)
{
	return redis_client::Command("PFADD") << key << elements;
}

string redis_client::CommandHelper::PFCount(const vector<string>& keys)
{
	return redis_client::Command("PFCOUNT") << keys;
}

string redis_client::CommandHelper::PFMerge(const string & dstkey, const vector<string> srckeys)
{
	return redis_client::Command("PFMERGE") << dstkey << srckeys;
}

string redis_client::CommandHelper::PSubscribe(const vector<string>& patterns)
{
	return redis_client::Command("PSUBSCRIBE") << patterns;
}

string redis_client::CommandHelper::PubSub_Channels(const string & pattern)
{
	redis_client::Command c("PUBSUB");
	c << "CHANNELS";
	if (pattern != kEmptyString) {
		c << pattern;
	}

	return c;
}

string redis_client::CommandHelper::PubSub_NumSub(const vector<string>& channels)
{
	return redis_client::Command("PUBSUB") << "NUMSUB" << channels;
}

string redis_client::CommandHelper::PubSub_NumPat()
{
	return redis_client::Command("PUBSUB") << "NUMPAT";
}

string redis_client::CommandHelper::Publish(const string & channel, const string & msg)
{
	return redis_client::Command("PUBLISH") << channel << msg;
}

string redis_client::CommandHelper::PUnsubscribe(const vector<string>& patterns)
{
	return redis_client::Command("PUNSUBSCRIBE") << patterns;
}

string redis_client::CommandHelper::Subscribe(const vector<string>& channels)
{
	return redis_client::Command("SUBSCRIBE") << channels;
}

string redis_client::CommandHelper::Unsubscribe(const vector<string>& channels)
{
	return redis_client::Command("UNSUBSCRIBE") << channels;
}

