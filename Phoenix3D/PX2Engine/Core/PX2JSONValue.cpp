// PX2JSONValue.cpp

#include "PX2JSONValue.hpp"
#include "rapidjson.h"
#include "document.h"
using namespace PX2;

//----------------------------------------------------------------------------
JSONValue::JSONValue():
mValue(0)
{
}
//----------------------------------------------------------------------------
JSONValue::JSONValue(void* jsValue)
{
	mValue = jsValue;
}
//----------------------------------------------------------------------------
JSONValue::JSONValue(const JSONValue &rhs)
{
	mValue = rhs.mValue;
}
//----------------------------------------------------------------------------
JSONValue::~JSONValue()
{
}
//----------------------------------------------------------------------------
bool JSONValue::IsNull()
{
	if (!mValue) return true;
	
	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	return jValue->IsNull();
}
//----------------------------------------------------------------------------
int JSONValue::ToInt()
{
	if (!mValue) return 0;

	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	return jValue->GetInt();
}
//----------------------------------------------------------------------------
float JSONValue::ToFloat()
{
	if (!mValue) return 0.0f;

	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	return jValue->GetFloat();
}
//----------------------------------------------------------------------------
bool JSONValue::ToBool()
{
	if (!mValue) return false;

	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	return jValue->GetBool();
}
//----------------------------------------------------------------------------
std::string JSONValue::ToString()
{
	if (!mValue) return "";

	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	return jValue->GetString();
}
//----------------------------------------------------------------------------
bool JSONValue::ToIntVal(int &value)
{
	if (!mValue) return false;

	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	value = jValue->GetInt();
	return true;
}
//----------------------------------------------------------------------------
bool JSONValue::ToFloatVal(float &value)
{
	if (!mValue) return false;

	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	value = jValue->GetFloat();
	return true;
}
//----------------------------------------------------------------------------
bool JSONValue::ToBoolVal(bool &value)
{
	if (!mValue) return false;

	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	value = jValue->GetBool();
	return true;
}
//----------------------------------------------------------------------------
bool JSONValue::ToStringVal(std::string &value)
{
	if (!mValue) return false;

	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	value = jValue->GetString();
	return true;
}
//----------------------------------------------------------------------------
bool JSONValue::IsArray()
{
	if (!mValue) return false;

	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	return jValue->IsArray();
}
//----------------------------------------------------------------------------
int JSONValue::GetArraySize()
{
	if (!mValue) return 0;

	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	return jValue->GetArray().Size();
}
//----------------------------------------------------------------------------
JSONValue JSONValue::GetArrayElement(int i)
{
	if (!mValue) return JSONValue();

	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	int arraySize = jValue->GetArray().Size();

	if (0 <= i && i < arraySize)
	{
		rapidjson::Value &je = (*jValue).GetArray()[i];
		return JSONValue(&je);
	}
	else
	{
		return JSONValue();
	}
}
//----------------------------------------------------------------------------
bool JSONValue::IsHasMember(const std::string &name)
{
	if (!mValue) return false;

	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	return jValue->HasMember(name.c_str());
}
//----------------------------------------------------------------------------
JSONValue JSONValue::GetMember(const std::string &name)
{
	if (!mValue) return JSONValue();
	if (!IsHasMember(name)) return JSONValue();

	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	rapidjson::Value &jm = (*jValue)[name.c_str()];

	return JSONValue(&jm);
}
//----------------------------------------------------------------------------