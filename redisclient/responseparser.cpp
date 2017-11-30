

#include "responseparser.h"


const char redis_client::ResponseParser::kRedisProtocolPrefixSimpleString = '+';
const char redis_client::ResponseParser::kRedisProtocolPrefixError = '-';
const char redis_client::ResponseParser::kRedisProtocolPrefixIntegers = ':';
const char redis_client::ResponseParser::kRedisProtocolPrefixBulkString = '$';
const char redis_client::ResponseParser::kRedisProtocolPrefixArrays = '*';
const char* redis_client::ResponseParser::kRedisProtocolSplit = "\r\n";
const char* redis_client::ResponseParser::kRedisProtocolNilLength = "-1";


using namespace redis_client;


inline string * redis_client::ResponseData::GetContent()
{
	if (nullptr != mData) {
		return mData->content;
	}
	return nullptr;
}

inline int redis_client::ResponseData::GetIndex()
{
	return mStartIndex;
}

inline size_t redis_client::ResponseData::GetLen()
{
	return mLen;
}

redis_client::ResponseData::ResponseData(const string & content) : ResponseData ( content, 0, content.size())
{
}

redis_client::ResponseData::ResponseData(const string & content, int startindex, size_t len)
{
	mData = new _Data();
	mData->refcount = 1;
	mData->content = new string(content);
	mStartIndex = startindex;
	mLen = len;
}

redis_client::ResponseData::ResponseData(const ResponseData & data) : ResponseData(data, data.mStartIndex, data.mLen)
{
}

redis_client::ResponseData::ResponseData(const ResponseData & data, int startindex, int len)
{
	mData = data.mData;
	if (nullptr != mData) {
		mData->refcount++;
	}
	mStartIndex = startindex;
	mLen = len;
}

redis_client::ResponseData & redis_client::ResponseData::operator=(const ResponseData & data)
{
	if (this == &data)
		return *this;
	if (mData != nullptr && mData == data.mData) {
		mStartIndex = data.mStartIndex;
		mLen = data.mLen;
		return *this;
	}
		
	if (mData != nullptr) {
		mData->refcount--;
		if (0 == mData->refcount) {
			if (mData->content != nullptr) {
				delete mData->content;
				mData->content = nullptr;
			}
			delete mData;
			mData = nullptr;
		}
	}

	mData = data.mData;
	if (mData != nullptr) {
		mData->refcount++;
	}
	mStartIndex = data.mStartIndex;
	mLen = data.mLen;

	return *this;
}

redis_client::ResponseData::~ResponseData()
{
	if (mData != nullptr) {
		mData->refcount--;
		if (0 == mData->refcount) {
			if (mData->content != nullptr) {
				delete mData->content;
				mData->content = nullptr;
			}
			delete mData;
			mData = nullptr;
		}
	}
}

redis_client::ResponseParser::Type redis_client::ResponseParser::GetType() throw()
{
	if (!mTypeParsed)
		DoTypeParse();
	return mType;
}

int64_t redis_client::ResponseParser::GetValueInteger() throw()
{
	if (!mParsed)
		DoParse();
	if (Type::kType_Integer == mType) {
		return mData.i;
	}
	else {
		throw RedisErrorTypeValueException();
	}
}

std::string& redis_client::ResponseParser::GetValueString() throw()
{
	if (!mParsed)
		DoParse();
	switch (mType)
	{
	case redis_client::ResponseParser::kType_SimpleString:
	case redis_client::ResponseParser::kType_BulkString:
		return *mData.str;
	case redis_client::ResponseParser::kType_Integer:
		return std::to_string(mData.i);
	case redis_client::ResponseParser::kType_Error:
		return *mData.str;
	default:
		throw RedisErrorTypeValueException();
		break;
	}
}

std::string & redis_client::ResponseParser::GetValueError() throw()
{
	return GetValueString();
}

redis_client::ResponseParser redis_client::ResponseParser::GetValueArrayObjectAt(size_t index) throw()
{
	return operator[](index);
}

size_t redis_client::ResponseParser::GetValueArrayLength() throw()
{
	if (!mParsed)
		DoParse();
	if (Type::kType_Array == mType) {
		return mData.array->size();
	}
	else {
		throw RedisErrorTypeValueException();
	}
}

redis_client::ResponseParser redis_client::ResponseParser::operator[](size_t index) throw()
{
	if (!mParsed)
		DoParse();
	if (Type::kType_Array == mType) {
		if (index > mData.array->size())
			throw RedisIndexOutOfBoundException();
		return ResponseParser((*mData.array)[index]);
	}
	else {
		throw RedisErrorTypeValueException();
	}
}

redis_client::ResponseParser redis_client::ResponseParser::ParseResponse(const string & resp)
{
	return ResponseParser(resp);
}

int redis_client::ResponseParser::CheckContentIsAvailableForParse(const string & content)
{
	const char *str = content.data();
	size_t len = content.size();
	size_t parsedlen;
	ParseResult ret = ParseLength(str, len, &parsedlen);
	if (ret != ParseResult::ParseResult_Done)
		return 0;
	else
		return parsedlen;
}

void redis_client::ResponseParser::DoTypeParse() throw()
{
	string *content = mResponseData.GetContent();
	int startindex = mResponseData.GetIndex();
	size_t len = mResponseData.GetLen();
	const char *str = content->data();

	if (len < 1) {
		throw RedisInvalidFormatException();
	}
	switch (str[startindex])
	{
	case kRedisProtocolPrefixSimpleString:
		mType = kType_SimpleString;
		break;
	case kRedisProtocolPrefixError:
		mType = kType_Error;
		break;
	case kRedisProtocolPrefixIntegers:
		mType = kType_Integer;
		break;
	case kRedisProtocolPrefixBulkString:
		if (len < 3)
			throw RedisInvalidFormatException();
		if ((*(short *)kRedisProtocolNilLength) == (*(short *)&str[startindex + 1])) {
			mType = kType_Nil;
		}
		else {
			mType = kType_BulkString;
		}
		break;
	case kRedisProtocolPrefixArrays:
		if (len < 3)
			throw RedisInvalidFormatException();
		if ((*(short *)kRedisProtocolNilLength) == (*(short *)&str[startindex + 1])) {
			mType = kType_Nil;
		}
		else {
			mType = kType_Array;
		}
		break;
	default:
		throw RedisInvalidFormatException();
	}
}

void redis_client::ResponseParser::DoParse() throw()
{
	if (!mTypeParsed)
		DoTypeParse();

	if (mParsed)
		return;
	
	switch (mType)
	{
	case redis_client::ResponseParser::kType_SimpleString:
		DoParseSimpleString();
		break;
	case redis_client::ResponseParser::kType_Integer:
		DoParseInteger();
		break;
	case redis_client::ResponseParser::kType_Array:
		DoParseArray();
		break;
	case redis_client::ResponseParser::kType_Error:
		DoParseError();
		break;
	case redis_client::ResponseParser::kType_Nil:
		break;
	case redis_client::ResponseParser::kType_BulkString:
		DoParseBulkString();
		break;
	default:
		throw RedisInvalidFormatException();
		break;
	}

	mParsed = true;
}

void redis_client::ResponseParser::DoParseSimpleString() throw()
{
	// format: "+OK\r\n"
	string *content = mResponseData.GetContent();
	int startindex = mResponseData.GetIndex();
	size_t len = mResponseData.GetLen();

	size_t parsedlen;
	ParseResult ret = ParseSingleLineLength(content->data() + startindex, len, &parsedlen);


	if (ret != ParseResult::ParseResult_Done) {
		throw RedisInvalidFormatException();
	}

	mData.str = new string(content->data() + startindex + 1, parsedlen - 3);
}

void redis_client::ResponseParser::DoParseError() throw()
{
	// format: "-Error message\r\n"
	string *content = mResponseData.GetContent();
	int startindex = mResponseData.GetIndex();
	size_t len = mResponseData.GetLen();

	size_t parsedlen = 0;
	ParseResult ret = ParseSingleLineLength(content->data() + startindex, len, &parsedlen);


	if (ret != ParseResult::ParseResult_Done) {
		throw RedisInvalidFormatException();
	}

	mData.str = new string(content->data() + startindex + 1, parsedlen - 3);
}

void redis_client::ResponseParser::DoParseInteger() throw()
{
	// format: ":1000\r\n"
	string *content = mResponseData.GetContent();
	int startindex = mResponseData.GetIndex();
	size_t len = mResponseData.GetLen();

	size_t parsedlen;
	ParseResult ret = ParseSingleLineLength(content->data() + startindex, len, &parsedlen);


	if (ret != ParseResult::ParseResult_Done) {
		throw RedisInvalidFormatException();
	}

	string str(content->data() + startindex + 1, parsedlen - 3);
	int64_t result;
	try {
		 result = std::stoll(str);
	}
	catch (invalid_argument) {
		throw RedisInvalidFormatException();
	}

	mData.i = result;
}

void redis_client::ResponseParser::DoParseBulkString() throw()
{
	// format: "$6\r\nfoobar\r\n", empty: "$0\r\n\r\n", nil: "$-1\r\n"
	string *content = mResponseData.GetContent();
	int startindex = mResponseData.GetIndex();
	size_t len = mResponseData.GetLen();
	const char *str = content->data();

	size_t parsedlen;
	int contentlen;
	ParseResult ret = ParseBulkStringLength(str + startindex, len, &parsedlen, &contentlen);

	if (ret != ParseResult::ParseResult_Done) {
		throw RedisInvalidFormatException();
	}

	if (-1 == contentlen) {
		mType = kType_Nil;
		return;
	}
	
	mData.str = new string(str + startindex + parsedlen - (contentlen + 2), contentlen);
}

void redis_client::ResponseParser::DoParseArray() throw()
{
	// format: "*2\r\n$3\r\nfoo\r\n$3\r\nbar\r\n", empty: "*0\r\n", nil: "*-1\r\n"
	string *content = mResponseData.GetContent();
	int startindex = mResponseData.GetIndex();
	size_t len = mResponseData.GetLen();
	const char *str = content->data();

	size_t parsedlen;
	size_t contentlen;
	int arraylen;
	std::vector<int> indexs;
	ParseResult ret = ParseArrayLength(str + startindex, len, &parsedlen, &arraylen, &contentlen, &indexs);

	if (ret != ParseResult::ParseResult_Done) {
		throw RedisInvalidFormatException();
	}

	if (-1 == arraylen) {
		mType = kType_Nil;
		return;
	}

	mData.array = new vector<ResponseData>();

	if (0 != arraylen) {
		int index = indexs[0];
		for (int i = 1; i < indexs.size(); i++) {
			mData.array->push_back(ResponseData(mResponseData, startindex + index, indexs[i] - index));
			index = indexs[i];
		}
		mData.array->push_back(ResponseData(mResponseData, startindex + index, parsedlen - index));
	}
}

redis_client::ResponseParser::ParseResult redis_client::ResponseParser::ParseLength(const char * str, size_t len, size_t * parsedlen)
{
	*parsedlen = 0;

	if (nullptr == str || len < 3) {
		return ParseResult::ParseResult_Unfinish;
	}

	switch (str[0])
	{
	case kRedisProtocolPrefixSimpleString:
	case kRedisProtocolPrefixError:
	case kRedisProtocolPrefixIntegers:
		return ParseSingleLineLength(str, len, parsedlen);
	case kRedisProtocolPrefixBulkString:
		if (len < 5) {
			return ParseResult::ParseResult_Unfinish;
		}
		return ParseBulkStringLength(str, len, parsedlen, nullptr);
	case kRedisProtocolPrefixArrays:
		if (len < 4) {
			return ParseResult::ParseResult_Unfinish;
		}
		return ParseArrayLength(str, len, parsedlen, nullptr, nullptr, nullptr);
	default:
		return ParseResult::ParseResult_Error;
	}
}

redis_client::ResponseParser::ParseResult redis_client::ResponseParser::ParseSingleLineLength(const char * str, size_t len, size_t * parsedlen)
{
	short delimiter = *(short *)kRedisProtocolSplit;
	int index;
	short v;
	for (index = 1; index < len - 1; index++) {
		v = *(short *)&str[index];
		if (v == delimiter) {
			if (parsedlen != nullptr)
				*parsedlen = index + 2;
			return ParseResult::ParseResult_Done;
		}
	}

	return ParseResult::ParseResult_Unfinish;
}

redis_client::ResponseParser::ParseResult redis_client::ResponseParser::ParseBulkStringLength(const char * str, size_t len, size_t * parsedlen
	, int *contentlen)
{
	short delimiter = *(short *)kRedisProtocolSplit;
	short nil = *(short *)kRedisProtocolNilLength;
	short v;
	int index;

	v = *(short *)&str[1];
	if (nil == v) {
		if (parsedlen != nullptr)
			*parsedlen = 5;
		if (contentlen != nullptr)
			*contentlen = -1;
		return ParseResult::ParseResult_Done;
	}

	for (index = 1; index < len - 1; index++) {
		v = *(short *)&str[index];
		if (v == delimiter) {
			break;
		}
	}
	if (index == len - 1) {
		return ParseResult::ParseResult_Unfinish;
	}
	int64_t bulkstringlen;
	try {
		bulkstringlen = std::stoll(string(str + 1, index - 1));
	}
	catch (invalid_argument) {
		return ParseResult::ParseResult_Error;
	}

	if (len < index + 2 + bulkstringlen + 2) {
		return ParseResult::ParseResult_Unfinish;
	}
	else {
		v = *(short *)&str[index + 2 + bulkstringlen];
		if (v != delimiter) {
			return ParseResult::ParseResult_Error;
		}
		else {
			if (parsedlen != nullptr)
				*parsedlen = index + 2 + bulkstringlen + 2;
			if (contentlen != nullptr)
				*contentlen = bulkstringlen;
			return ParseResult::ParseResult_Done;
		}
	}
}

redis_client::ResponseParser::ParseResult redis_client::ResponseParser::ParseArrayLength(const char * str, size_t len, size_t * parsedlen
	, int *arraylen, size_t *contentlen, std::vector<int> *indexs)
{
	short delimiter = *(short *)kRedisProtocolSplit;
	short nil = *(short *)kRedisProtocolNilLength;
	short v;
	int index;
	size_t elementstartindex;
	size_t elementlen;
	int arrayindex;
	ParseResult ret;

	v = *(short *)&str[1];
	if (nil == v) {
		if (len < 5)
			return ParseResult::ParseResult_Unfinish;
		if (parsedlen != nullptr)
			*parsedlen = 5;
		if (arraylen != nullptr)
			*arraylen = -1;
		if (contentlen != nullptr)
			*contentlen = 0;
		return ParseResult::ParseResult_Done;
	}

	for (index = 1; index < len - 1; index++) {
		v = *(short *)&str[index];
		if (v == delimiter) {
			break;
		}
	}
	if (index == len - 1) {
		return ParseResult::ParseResult_Unfinish;
	}
	int64_t arraycount;
	try {
		arraycount = std::stoll(string(str + 1, index - 1));
	}
	catch (invalid_argument) {
		return ParseResult::ParseResult_Error;
	}
	if (0 == arraycount) {
		if (parsedlen != nullptr)
			*parsedlen = 4;
		if (contentlen != nullptr)
			*contentlen = 0;
		if (arraylen != nullptr)
			*arraylen = 0;
		return ParseResult::ParseResult_Done;
	}

	index = index + 2;
	elementstartindex = index;
	for (arrayindex = 0; arrayindex < arraycount; arrayindex++) {
		ret = ParseLength(str + index, len - index, &elementlen);
		if (ret != ParseResult::ParseResult_Done) {
			if (indexs != nullptr) {
				indexs->clear();
			}
			return ret;
		}

		if (indexs != nullptr) {
			indexs->push_back(index);
		}
		index = index + elementlen;
	}

	if (parsedlen != nullptr)
		*parsedlen = index;
	if (arraylen != nullptr)
		*arraylen = arraycount;
	if (contentlen != nullptr)
		*contentlen = index - elementstartindex;
	return ParseResult::ParseResult_Done;
}

redis_client::ResponseParser::ResponseParser(const string &resp) : mResponseData(resp), mType(kType_Unknown), mParsed(false), mTypeParsed(false)
{
	memset(&mData, 0, sizeof(mData));
	mData.str = nullptr;
	mData.array = nullptr;
}

redis_client::ResponseParser::ResponseParser(const ResponseData &resp) : mResponseData(resp)
, mType(kType_Unknown), mParsed(false), mTypeParsed(false)
{
	memset(&mData, 0, sizeof(mData));
	mData.str = nullptr;
	mData.array = nullptr;
}

redis_client::ResponseParser::ResponseParser(const ResponseData &resp, int startindex, size_t len) : mResponseData(resp, startindex, len)
	, mType(kType_Unknown), mParsed(false), mTypeParsed(false)
{
	memset(&mData, 0, sizeof(mData));
	mData.str = nullptr;
	mData.array = nullptr;
}

redis_client::ResponseParser::~ResponseParser()
{
	if (kType_SimpleString == mType || kType_BulkString == mType) {
		delete mData.str;
		mData.str = nullptr;
	}
	else if (kType_Array == mType) {
		delete mData.array;
		mData.array = nullptr;
	}
}

ResponseParser & redis_client::ResponseParser::operator=(const ResponseParser & parser)
{
	mData = parser.mData;
	mResponseData = parser.mResponseData;
	mType = parser.mType;
	mParsed = parser.mParsed;
	mTypeParsed = parser.mTypeParsed;
	return (*this);
}
