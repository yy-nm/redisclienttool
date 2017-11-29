

// add by mardyu 2017-11-23
// this file is used to create command convenient via Command class


#ifndef _REDIS_CLIENT_COMMANDHELPER_H_
#define _REDIS_CLIENT_COMMANDHELPER_H_


#include <string>
#include <vector>
#include <map>


using namespace std;


namespace redis_client
{
	class CommandHelper
	{
	public:
		enum OverflowType {
			OFT_NONE,
			WRAP,
			SAT,
			FAIL,
		};
		enum BitOPType {
			AND,
			OR,
			XOR,
			NOT,
		};
		enum ClientType {
			CT_NONE,
			NORMAL,
			MASTER,
			SLAVE,
			PUBSUB,
		};
		enum ClientReplyType {
			ON,
			OFF,
			SKIP,
		};
		enum ClusterFailOverType {
			FORCE,
			TAKEOVER,
		};
		enum ClusterResetType {
			HARD,
			SOFT,
		};
		enum GeoDistanceUnit {
			GDU_NONE,
			M,
			KM,
			MI,
			FT,
		};
		enum SortType {
			ST_NONE,
			ASC,
			DESC,
		};
		enum ListInsertPos {
			BEFORE,
			AFTER,
		};
		enum ObjectSubCommandType {
			REFCOUNT,
			ENCODING,
			IDLETIME,
			FREQ,
		};
		enum ScriptDebugMode {
			YES,
			SYNC,
			NO,
		};
		enum Mode {
			M_NONE,
			NX,
			XX,
		};
		enum AggregateType {
			AT_NONE,
			SUM,
			MIN,
			MAX,
		};

		struct GeoSpatialItem {
			float longitude;
			float latitude;
			string name;
		};
	public:

		// keys
		static string Del(const vector<string> &keys);
		static string Dump(const string &key);
		static string Exists(const vector<string> &keys);
		static string Expire(const string &key, int seconds);
		static string ExpireAt(const string &key, int timestamp);
		static string Keys(const string &pattern);
		static string Migrate(const string &host, int port, const string &key, int &db, int timeout, bool copy = false, bool replace = false
			, const vector<string> &keys = kEmptyVector);
		static string Move(const string &key, int db);
		static string Persist(const string &key);
		static string PExpipe(const string &key, int64_t milliseconds);
		static string PExpipeAt(const string &key, int64_t milliseconds_timestamp);
		static string PTTL(const string &key);
		static string RandomKey();
		static string Rename(const string &key, const string &newkey);
		static string RenameNX(const string &key, const string &newkey);
		static string Restore(const string &key, int ttl, const string &serialvalue, bool replace = false);
		static string Sort(const string &key, const string &bypattern = kEmptyString, const string &dst = kEmptyString, int limitoffset = 0, int limitcount = 0
			, const vector<string> &getpatterns = kEmptyVector, CommandHelper::SortType sorttype = CommandHelper::SortType::ST_NONE, bool alpha = false);
		static string Touch(const vector<string> &keys);
		static string TTL(const string &key);
		static string Type(const string &key);
		static string Unlink(const vector<string> &keys);
		static string Scan(int cursor, const string &pattern = kEmptyString, int count = 0);
		static string Wait(int numslaves, int timeout);
		static string Object(CommandHelper::ObjectSubCommandType type, const vector<string> &args);

		// hash
		static string HDel(const string &key, const vector<string> &fields);
		static string HExists(const string &key, const string &field);
		static string HGet(const string &key, const string &field);
		static string HGetAll(const string &key);
		static string HIncrBy(const string &key, const string &field, int increment);
		static string HIncrByFloat(const string &key, const string &field, float increment);
		static string HKeys(const string &key);
		static string HLen(const string &key);
		static string HSet(const string &key, const string &field, const string &value);
		static string HSetNX(const string &key, const string &field, const string &value);
		static string HStrLen(const string &key, const string &field);
		static string HValues(const string &key);
		static string HMGet(const string &key, const vector<string> &fields);
		static string HMSet(const string &key, map<string, string> &fieldvaluepairs);
		static string HScan(int cursor, const string &pattern = kEmptyString, int count = 0);

		// list
		static string BLPop(const vector<string> &keys, int timeout);
		static string BRPop(const vector<string> &keys, int timeout);
		static string BRPopLPush(const string &src, const string &dst, int timeout);
		static string LIndex(const string &key, int index);
		static string LInsert(const string &key, bool before, const string &pivot, const string &value);
		static string LLen(const string &key);
		static string LPop(const string &key);
		static string LPush(const string &key, const vector<string> &values);
		static string LPushX(const string &key, const string &value);
		static string LRange(const string &key, int start, int stop);
		static string LRem(const string &key, int count, const string &value);
		static string LSet(const string &key, int index, const string &value);
		static string LTrim(const string &key, int start, int stop);
		static string RPop(const string &key);
		static string RPopLPush(const string &src, const string &dst);
		static string RPush(const string &key, const vector<string> &values);
		static string RPushHX(const string &key, const string &value);

		// set
		static string SAdd(const string &key, const vector<string> &members);
		static string SCard(const string &key);
		static string SDiff(const vector<string> &keys);
		static string SDiffStore(const string &dst, const vector<string> &keys);
		static string SInter(const vector<string> &keys);
		static string SInterStore(const string &dst, const vector<string> &keys);
		static string SIsMember(const string &key, const string &member);
		static string SMembers(const string &key);
		static string SMove(const string &src, const string &dst, const string &member);
		static string SPop(const string &key, int count = 1);
		static string SRandMember(const string &key, int count = 1);
		static string SRem(const string &key, const vector<string> &members);
		static string SUnion(const vector<string> &keys);
		static string SUnionStore(const string &dst, const vector<string> &keys);
		static string SScan(int cursor, const string &pattern = kEmptyString, int count = 0);

		// sorted set
		static string ZAdd(const string &key, const map<double, string> &scoremembers, Mode mode = Mode::M_NONE, bool ch = false, bool incr = false);
		static string ZCard(const string &key);
		static string ZCount(const string &key, double min, double max);
		static string ZIncreBy(const string &key, double increment, const string &member);
		static string ZInterStore(const string &dst, const vector<string> &keys, const vector<double> &weights
			, AggregateType aggtype = AggregateType::AT_NONE);
		static string ZLexCount(const string &key, const string &min, const string &max);
		static string ZRange(const string &key, int start, int stop, bool withscores = false);
		static string ZRangeByLex(const string &key, const string &min, const string &max, int offset = 0, int count = 0);
		static string ZRevRangeByLex(const string &key, const string &max, const string &min, int offset = 0, int count = 0);
		static string ZRangeByScore(const string &key, double min, double max, bool withscores = false, int offset = 0, int count = 0);
		static string ZRank(const string &key, const string &member);
		static string ZRem(const string &key, const vector<string> &members);
		static string ZRemRangeByLex(const string &key, const string &min, const string &max);
		static string ZRemRangeByRank(const string &key, int start, int stop);
		static string ZRemRangeByScore(const string &key, double min, double max);
		static string ZRevRange(const string &key, int start, int stop, bool withscores = false);
		static string ZRevRangeByScore(const string &key, double max, double min, bool withscores = false, int offset = 0, int count = 0);
		static string ZRevRank(const string &key, const string &member);
		static string ZScore(const string &key, const string &member);
		static string ZUnionStore(const string &dst, const vector<string> &keys, const vector<double> &weights
			, AggregateType aggtype = AggregateType::AT_NONE);
		static string ZScan(int cursor, const string &pattern = kEmptyString, int count = 0);

		// connection
		static string Auth(const string &password);
		static string Quit();
		static string Select(int index);
		static string SwapDB(int index1, int index2);
		static string Ping(const string &msg = kEmptyString);
		static string Echo(const string &msg);

		// string
		static string Append(const string &key, const string &value);
		static string Decr(const string &key);
		static string DecrBy(const string &key, int decrement);
		static string Get(const string &key);
		static string GetRange(const string &key, int start, int end);
		static string GetSet(const string &key, string&value);
		static string Incr(const string &key);
		static string incrBy(const string &key, int increment);
		static string incrByFloat(const string &key, float increment);
		static string MGet(const vector<string> &keys);
		static string MSet(const map<string, string> &kvs);
		static string MSetNX(const map<string, string> &kvs);
		static string PSetEX(const string &key, int64_t milliseconds, const string &value);
		static string Set(const string &key, const string &value, int seconds = 0, int64_t milliseconds = 0, CommandHelper::Mode mode = Mode::M_NONE);
		static string SetEX(const string &key, int seconds, const string &value);
		static string SetNX(const string &key, const string &value);
		static string STRLen(const string &key);
		// string bit
		static string BitCount(const string &key);
		static string BitCount(const string &key, const string &start, const string &end);
		static string BitField_Get(const string &key, const string &type, int offset);
		static string BitField_Set(const string &key, const string &type, int offset, int value);
		static string BitField_IncrBy(const string &key, const string &type, int offset, int increment
			, CommandHelper::OverflowType overflowtype = OverflowType::OFT_NONE);
		static string BitOP(CommandHelper::BitOPType optype, const string &destkey, const vector<string> &keys);
		static string BitPos(const string &key, int bit);
		static string BitPos(const string &key, int bit, int start = 0, int end = 0);
		static string GetBit(const string &key, int offset);
		static string SetBit(const string &key, int offset, int value);
		static string SetRange(const string &key, int offset, int value);

		// cluster
		static string ClusterAddSlots(const vector<string> &slots);
		static string ClusterCountFailureReport(const string &nodeid);
		static string ClusterCountKeysInSlot(const string &slot);
		static string ClusterDelSlots(const vector<string> &slots);
		static string ClusterFailOver(CommandHelper::ClusterFailOverType type);
		static string ClusterForget(const string &nodeid);
		static string ClusterGetKeysInSlot(string slot, int count);
		static string ClusterInfo();
		static string ClusterKeySlot(const string &key);
		static string ClusterMeet(const string &ip, int port);
		static string ClusterNodes();
		static string ClusterReplicate(const string &nodeid);
		static string ClusterReset(CommandHelper::ClusterResetType type);
		static string ClusterSaveConfig();
		static string ClusterSetConfigEpoch(const string &configepoch);
		static string ClusterSetSlot_Migrating(const string &slot, const string &dstnodeid);
		static string ClusterSetSlot_Import(const string &slot, const string &srcnodeid);
		static string ClusterSetSlot_Table(const string &slot);
		static string ClusterSetSlot_Node(const string &slot, const string &nodeid);
		static string ClusterSlaves(const string &nodeid);
		static string CLusterSlots();
		static string ReadOnly();
		static string ReadWrite();
		
		// server
		static string Info(const string &section = kEmptyString);
		static string Monitor();
		static string Role();
		static string Shutdown();
		static string Shutdown(bool save);
		static string Slaveof(const string &host, int port);
		static string Sync();
		static string Time();
		// server save
		static string BgRewriteAOF();
		static string BgSave();
		static string LastSave();
		static string Save();
		// server clientkill
		static string ClientKill(const string &addr);
		static string ClientKill_ClientId(const string &id = kEmptyString, CommandHelper::ClientType clienttype = ClientType::CT_NONE
			, const string &addr = kEmptyString
			, bool skipme = true);
		static string ClientList();
		static string ClientGetName();
		static string ClientPause(int timeout);
		static string ClientReply(CommandHelper::ClientReplyType replytype);
		static string ClientSetName(const string &name);
		// serve command
		static string Command();
		static string CommandCount();
		static string CommandGetKeys();
		static string CommandInfo(const vector<string> &cmds);
		// server config
		static string ConfigGet(const string &parameter);
		static string ConfigRewrite();
		static string ConfigSet(const string &parameter, const string &value);
		static string ConfigResetStat();
		// server debug
		static string DebugObject(const string &key);
		static string DebugSegfault();
		// server db
		static string DBSize();
		static string FlushAll(bool async = false);
		static string FlushDB(bool async = false);
		// server slowlog
		static string SlowlogGet(int count);
		static string SlowlogLen();
		static string SlowlogReset();

		//transaction
		static string Discard();
		static string Exec();
		static string Multi();
		static string Watch(const vector<string> &keys);
		static string Unwatch();

		// geo
		static string GeoAdd(const string &key, const vector<GeoSpatialItem> &items);
		static string GeHash(const string &key, const vector<string> &members);
		static string GePos(const string &key, const vector<string> &members);
		static string GeDist(const string &key, const string &member1, const string &member2
			, CommandHelper::GeoDistanceUnit unit = GeoDistanceUnit::GDU_NONE);
		static string GeoRadius(const string &key, float longitude, float latitude, float radius, CommandHelper::GeoDistanceUnit unit
			, int count = 0, const string &storekey = kEmptyString, const string &storedistkey = kEmptyString
			, CommandHelper::SortType sorttype = CommandHelper::SortType::ST_NONE, bool withcoord = false, bool withdist = false
			, bool withhash = false);
		static string GeoRadiusByMember(const string &key, const string &member, float radius, CommandHelper::GeoDistanceUnit unit
			, int count = 0, const string &storekey = kEmptyString, const string &storedistkey = kEmptyString
			, CommandHelper::SortType sorttype = CommandHelper::SortType::ST_NONE, bool withcoord = false, bool withdist = false
			, bool withhash = false);

		// hyperloglog
		static string PFAdd(const string &key, const vector<string> &elements);
		static string PFCount(const vector<string> &keys);
		static string PFMerge(const string &dstkey, const vector<string> srckeys);

		// channel
		static string PSubscribe(const vector<string> &patterns);
		static string PubSub_Channels(const string &pattern = kEmptyString);
		static string PubSub_NumSub(const vector<string> &channels);
		static string PubSub_NumPat();
		static string Publish(const string &channel, const string &msg);
		static string PUnsubscribe(const vector<string> &patterns);
		static string Subscribe(const vector<string> &channels);
		static string Unsubscribe(const vector<string> &channels);

		// script
		static string Eval(const string &scrpt, const vector<string> &keys, const vector<string> &args);
		static string EvalSHA(const string &sha1, const vector<string> &keys, const vector<string> &args);
		static string ScriptDebug(CommandHelper::ScriptDebugMode mode);
		static string ScriptExists(const vector<string> &sha1s);
		static string ScriptFlush();
		static string ScriptKill();
		static string ScriptLoad(const string &script);


		private:
			static const vector<string> kEmptyVector;
			static const string kEmptyString;
	};
}


#endif // !_REDIS_CLIENT_COMMANDHELPER_H_
