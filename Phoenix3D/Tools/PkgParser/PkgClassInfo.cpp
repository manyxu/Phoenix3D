// PkgClassInfo.cpp

#include "PkgClassInfo.hpp"
#include "PkgEnumInfo.hpp"
using namespace PkgParse;

//----------------------------------------------------------------------------
PkgClassInfo::PkgClassInfo() :
IsSingleton(false),
IsTemplate(false),
TemplateFloat(false),
TemplateDouble(false),
TemplateInt(false),
IsVirtual(false),
IsValueType(false),
ParentClassInfo(0),
mDestructorFunction(0),
IsHasReference(false)
{
}
//----------------------------------------------------------------------------
PkgClassInfo::~PkgClassInfo()
{
	for (int i = 0; i < (int)mFunctions.size(); i++)
	{
		delete mFunctions[i];
	};
}
//----------------------------------------------------------------------------
std::string PkgClassInfo::GetClassNameAS()
{
	if (IsTemplate)
		return ClassName + "f";

	return ClassName;
}
//----------------------------------------------------------------------------
bool PkgClassInfo::IsContainEnumType(const std::string &type)
{
	for (int i = 0; i < (int)EnumInfos.size(); i++)
	{
		if (EnumInfos[i]->IsContainEnumType(type))
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void PkgClassInfo::AddFunInfo(PkgFunInfo *info)
{
	if (PkgFunInfo::FT_CLASS_CONSTRUCTOR == info->TheFunType)
	{
		mConstructorFunctions.push_back(info);
	}
	else if (PkgFunInfo::FT_CLASS_DESTRUCTOR == info->TheFunType)
	{
		mDestructorFunction = info;
	}

	info->ClassInfo = this;
	
	mFunctions.push_back(info);
}
//----------------------------------------------------------------------------
std::vector<PkgFunInfo*> &PkgClassInfo::GetFunctions()
{
	return mFunctions;
}
//----------------------------------------------------------------------------
std::vector<PkgFunInfo*> &PkgClassInfo::GetConstructorFunctions()
{
	return mConstructorFunctions;
}
//----------------------------------------------------------------------------
bool PkgClassInfo::IsHasConstruct() const
{
	return (int)mConstructorFunctions.size() > 0;
}
//----------------------------------------------------------------------------
bool PkgClassInfo::IsHasDestruct() const
{
	return 0 != mDestructorFunction;
}
//----------------------------------------------------------------------------
PkgFunInfo *PkgClassInfo::GetDestructorFunction()
{
	return mDestructorFunction;
}
//----------------------------------------------------------------------------
void PkgClassInfo::ClearRegistFun()
{
	mRegistedFuns.clear();
}
//----------------------------------------------------------------------------
bool PkgClassInfo::IsFunRegisted(const std::string &funStrAS) const
{
	return mRegistedFuns.find(funStrAS) != mRegistedFuns.end();
}
//----------------------------------------------------------------------------
void PkgClassInfo::AddRegistFun(const std::string &funStrAS)
{
	mRegistedFuns[funStrAS] = 1;
}
//----------------------------------------------------------------------------