// PX2JSONData.cpp

#include "PX2JSONData.hpp"
#include "PX2Memory.hpp"
#include "rapidjson.h"
#include "document.h"
using namespace PX2;

//----------------------------------------------------------------------------
JSONData::JSONData():
mDocument(0)
{
}
//----------------------------------------------------------------------------
JSONData::~JSONData()
{
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	if (doc)
	{
		delete0(doc);
	}
}
//----------------------------------------------------------------------------
bool JSONData::LoadBuffer(const std::string &strBuffer)
{
	return LoadBuffer(strBuffer.c_str(), strBuffer.length());
}
//----------------------------------------------------------------------------
bool JSONData::LoadBuffer(const char *buffer, int size)
{
	PX2_UNUSED(size);

	rapidjson::Document *doc = new0 rapidjson::Document();
	mDocument = doc;

	if (doc->Parse<0>((char*)buffer).HasParseError())
	{
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------
bool JSONData::IsArray()
{
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	return doc->IsArray();
}
//----------------------------------------------------------------------------
int JSONData::GetArraySize()
{
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	return doc->GetArray().Size();
}
//----------------------------------------------------------------------------
JSONValue JSONData::GetArrayElement(int i)
{
	if (!IsArray())
		return JSONValue(0);

	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	rapidjson::Value& val = (*doc)[i];
	return JSONValue(&val);
}
//----------------------------------------------------------------------------
bool JSONData::IsNull()
{
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	return doc->IsNull();
}
//----------------------------------------------------------------------------
int JSONData::ToInt()
{
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	return doc->GetInt();
}
//----------------------------------------------------------------------------
float JSONData::ToFloat()
{
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	return doc->GetFloat();
}
//----------------------------------------------------------------------------
bool JSONData::ToBool()
{
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	return doc->GetBool();
}
//----------------------------------------------------------------------------
std::string JSONData::ToString()
{
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	return doc->GetString();
}
//----------------------------------------------------------------------------
bool JSONData::ToIntVal(int &value)
{
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	if (doc->IsBool())
	{
		value = doc->GetInt();

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool JSONData::ToFloatVal(float &value)
{
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	if (doc->IsBool())
	{
		value = doc->GetFloat();

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool JSONData::ToBoolVal(bool &value)
{
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	if (doc->IsBool())
	{
		value = doc->GetBool();

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool JSONData::ToStringVal(std::string &value)
{
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	if (doc->IsBool())
	{
		value = doc->GetString();

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool JSONData::IsHasMember(const std::string &name)
{
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	return !doc->HasMember(name.c_str());
}
//----------------------------------------------------------------------------
JSONValue JSONData::GetMember(const std::string &name)
{
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	rapidjson::Value& a = (*doc)[name.c_str()];
	return JSONValue(&a);
}
//----------------------------------------------------------------------------