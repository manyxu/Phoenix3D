// PX2Object_Function.cpp

#include "PX2Object.hpp"
#include "PX2FunObject.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
FunObject *Object::RegistClassFunctions()
{
	FunObject *funObj = new0 FunObject();
	funObj->IsClassCatalogue = true;
	funObj->ClassName = "Object";
	FunObjectManager::GetSingleton().SetObjectFunObject(funObj);

	FunObject *thisFunObj = funObj->GetAddClass("Object");

	{
		FunObjectPtr funObj = new0 FunObject;
		funObj->FunName = "SetName";
		funObj->AddInput("handler", FPT_POINTER_THIS, (Object*)0);
		funObj->AddInput("in_name", FPT_STRING, std::string("NoName"));
		thisFunObj->AddFunObject(funObj);
	}

	{
		FunObjectPtr funObj = new0 FunObject;
		funObj->FunName = "GetName";
		funObj->AddInput("handler", FPT_POINTER_THIS, (Object*)0);
		funObj->AddOutput("out_name", FPT_STRING, std::string("notvalied"));
		thisFunObj->AddFunObject(funObj);
	}

	{
		FunObjectPtr funObj = new0 FunObject;
		funObj->FunName = "GetObjectByName";
		funObj->AddInput("handler", FPT_POINTER_THIS, (Object*)0);
		funObj->AddInput("in_name", FPT_STRING, std::string("NoName"));
		funObj->AddOutput("out_object", FPT_POINTER, (Object*)0);
		thisFunObj->AddFunObject(funObj);
	}

	return thisFunObj;
}
//----------------------------------------------------------------------------