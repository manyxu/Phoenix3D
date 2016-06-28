// PX2JSONValue.hpp

#ifndef PX2JSONVALUE_HPP
#define PX2JSONVALUE_HPP

#include "PX2CorePre.hpp"
#include "PX2Object.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM JSONValue
	{
	public:
		JSONValue();
		JSONValue(void* jsValue);
		JSONValue(const JSONValue &rhs);
		~JSONValue();

		bool IsNull();
		int ToInt();
		float ToFloat();
		bool ToBool();
		std::string ToString();
		bool ToIntVal(int &value);
		bool ToFloatVal(float &value);
		bool ToBoolVal(bool &value);
		bool ToStringVal(std::string &value);

		bool IsArray();
		int GetArraySize();
		JSONValue GetArrayElement(int i);

		bool IsHasMember(const std::string &name);
		JSONValue GetMember(const std::string &name);

	private:
		void *mValue;
	};

}

#endif