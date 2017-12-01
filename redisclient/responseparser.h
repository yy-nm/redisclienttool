

// add by mardyu 2017-11-27
// this file is used to parse result 


#ifndef _REDIS_CLIENT_RESPONSEPARSER_H_
#define _REDIS_CLIENT_RESPONSEPARSER_H_


#include <vector>
#include <string>

#include "exception.h"


using namespace std;


namespace redis_client
{
	class ResponseParser;

	class ResponseData
	{
	private:
		struct _Data {
			int refcount;
			string *content;
		};
		_Data *mData;
		int mStartIndex;
		size_t mLen;

	public:
		inline string * GetContent();
		inline int GetIndex();
		inline size_t GetLen();

		explicit ResponseData(const string &content);
		explicit ResponseData(const string &content, int startindex, size_t len);
		explicit ResponseData(const ResponseData &data);
		explicit ResponseData(const ResponseData &data, int startindex, int len);

		ResponseData& operator=(const ResponseData &data);

		virtual ~ResponseData();
	};

	union _Data
	{
		int64_t i;
		string *str;
		vector<ResponseData> *array;
	};

	class ResponseParser
	{
	public:
		enum Type {
			kType_Unknown,
			kType_SimpleString,
			kType_BulkString,
			kType_Integer,
			kType_Array,
			kType_Error,
			kType_Nil,
		};
		enum ParseResult {
			ParseResult_Done,
			ParseResult_Unfinish,
			ParseResult_Error,
		};
	private:

		_Data mData;

		ResponseData mResponseData;
		Type mType;
		bool mParsed;
		bool mTypeParsed;
	public:
		
		ResponseParser::Type GetType() throw();
		int64_t GetValueInteger() throw();
		std::string GetValueString() throw();
		std::string GetValueError() throw();
		ResponseParser GetValueArrayObjectAt(size_t index) throw();
		size_t GetValueArrayLength() throw();

		ResponseParser operator[](size_t index) throw();

		static ResponseParser ParseResponse(const string &resp);
		// if return 0, means content not available, or it ready for parse
		static int CheckContentIsAvailableForParse(const string &content);

		ResponseParser(const string &resp);
		ResponseParser(const ResponseData &resp);
		ResponseParser(const ResponseData &resp, int startindex, size_t len);
		virtual ~ResponseParser();
		ResponseParser& operator=(const ResponseParser &parser);

	private:
		void DoTypeParse() throw();
		void DoParse() throw();
		void DoParseSimpleString() throw();
		void DoParseError() throw();
		void DoParseInteger() throw();
		void DoParseBulkString() throw();
		void DoParseArray() throw();

		static ParseResult ParseLength(const char *str, size_t len, size_t *parsedlen);
		static ParseResult ParseSingleLineLength(const char *str, size_t len, size_t *parsedlen);
		static ParseResult ParseBulkStringLength(const char *str, size_t len, size_t *parsedlen, int *contentlen);
		static ParseResult ParseArrayLength(const char *str, size_t len, size_t *parsedlen, int *arraylen, size_t *contentlen, std::vector<int> *indexs);


		static const char kRedisProtocolPrefixSimpleString;
		static const char kRedisProtocolPrefixError;
		static const char kRedisProtocolPrefixIntegers;
		static const char kRedisProtocolPrefixBulkString;
		static const char kRedisProtocolPrefixArrays;
		static const char* kRedisProtocolSplit;
		static const char* kRedisProtocolNilLength;
	};
}


#endif //!_REDIS_CLIENT_RESPONSEPARSER_H_
