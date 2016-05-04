// PX2FunObject.cpp

#include "PX2FunObject.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
// FunParam
//----------------------------------------------------------------------------
FunParam::FunParam() :
Type(FPT_INT)
{
}
//----------------------------------------------------------------------------
FunParam::~FunParam()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FunObject
//----------------------------------------------------------------------------
FunObject::FunObject() :
IsClassCatalogue(false)
{
}
//----------------------------------------------------------------------------
FunObject::~FunObject()
{
}
//----------------------------------------------------------------------------
void FunObject::AddInput(const std::string &paramName,
	FunParamType type, const Any &paramValue)
{
	FunParam funParam;
	funParam.Name = paramName;
	funParam.Type = type;
	funParam.Value = paramValue;
	mInParams.push_back(funParam);
}
//----------------------------------------------------------------------------
void FunObject::AddOutput(const std::string &paramName, FunParamType type,
	const Any &paramValue)
{
	FunParam funParam;
	funParam.Name = paramName;
	funParam.Type = type;
	funParam.Value = paramValue;
	mOutParams.push_back(funParam);
}
//----------------------------------------------------------------------------
const std::vector<FunParam> &FunObject::GetInParams() const
{
	return mInParams;
}
//----------------------------------------------------------------------------
int FunObject::GetNumInParams() const
{
	return (int)mInParams.size();
}
//----------------------------------------------------------------------------
const FunParam &FunObject::GetInParam(int i) const
{
	return mInParams[i];
}
//----------------------------------------------------------------------------
void FunObject::ClearInParams()
{
	mInParams.clear();
}
//----------------------------------------------------------------------------
const std::vector<FunParam> &FunObject::GetOutParams() const
{
	return mOutParams;
}
//----------------------------------------------------------------------------
int FunObject::GetNumOutParams() const
{
	return (int)mOutParams.size();
}
//----------------------------------------------------------------------------
const FunParam &FunObject::GetOutParam(int i) const
{
	return mOutParams[i];
}
//----------------------------------------------------------------------------
void FunObject::ClearOutParams()
{
	mOutParams.clear();
}
//----------------------------------------------------------------------------
FunObject *FunObject::GetAddClass(const std::string &className)
{
	if (IsClassCatalogue && ClassName == className)
		return this;

	std::vector<Pointer0<FunObject> >::iterator it = mChildFunObjectVec_Class.begin();
	for (; it != mChildFunObjectVec_Class.end(); it++)
	{
		FunObject *funObj = (*it)->GetAddClass(className);
		if (funObj)
		{
			return funObj;
		}
	}

	FunObjectPtr funObj = new0 FunObject();
	funObj->IsClassCatalogue = true;
	funObj->ClassName = className;

	AddFunObject(funObj);

	return funObj;
}
//----------------------------------------------------------------------------
FunObject *FunObject::GetFunObject(const std::string &className,
	const std::string &funName)
{
	if (!IsClassCatalogue && ClassName == className && FunName == funName)
		return this;

	std::vector<Pointer0<FunObject> >::iterator it = mChildFunObjectVec.begin();
	for (; it != mChildFunObjectVec.end(); it++)
	{
		FunObject *funObj = (*it)->GetFunObject(className, funName);
		if (funObj)
		{
			return funObj;
		}
	}

	std::vector<Pointer0<FunObject> >::iterator it1 = mChildFunObjectVec_Class.begin();
	for (; it1 != mChildFunObjectVec_Class.end(); it1++)
	{
		FunObject *funObj = (*it1)->GetFunObject(className, funName);
		if (funObj)
		{
			return funObj;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
void FunObject::AddFunObject(FunObject *funObj)
{
	if (IsHasFunObject(funObj))
		return;

	if (funObj->IsClassCatalogue)
	{
		mChildFunObjectVec_Class.push_back(funObj);
	}
	else
	{
		funObj->ClassName = ClassName;
		mChildFunObjectVec.push_back(funObj);
	}

}
//----------------------------------------------------------------------------
bool FunObject::IsHasFunObject(FunObject *funObj)
{
	for (int i = 0; i<(int)mChildFunObjectVec.size(); i++)
	{
		if (funObj->FunName == mChildFunObjectVec[i]->FunName)
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FunObjectManager
//----------------------------------------------------------------------------
FunObjectManager::FunObjectManager()
{
}
//----------------------------------------------------------------------------
FunObjectManager::~FunObjectManager()
{
}
//----------------------------------------------------------------------------
bool FunObjectManager::Initlize()
{
	mObjectFunObject = new0 FunObject();
	mObjectFunObject->IsClassCatalogue = true;
	mObjectFunObject->ClassName = "Object";

	return true;
}
//----------------------------------------------------------------------------
bool FunObjectManager::Terminate()
{
	mObjectFunObject = 0;
	mFunMap.clear();

	return true;
}
//----------------------------------------------------------------------------
FunObject *FunObjectManager::GetFunObject()
{
	return mObjectFunObject;
}
//----------------------------------------------------------------------------
std::map<std::string, FunObjectPtr> FunObjectManager::GetFunMap()
{
	return mFunMap;
}
//----------------------------------------------------------------------------
FunObject *FunObjectManager::GetAddFunMap(const std::string &name)
{
	std::map<std::string, FunObjectPtr>::iterator it = mFunMap.find(name);
	if (it != mFunMap.end())
	{
		return it->second;
	}

	FunObject *funObj = new0 FunObject();
	mFunMap[name] = funObj;

	return funObj;
}
//----------------------------------------------------------------------------