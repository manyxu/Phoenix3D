// PX2LogicManager.cpp

#include "PX2LogicManager.hpp"
#include "PX2Float3.hpp"
#include "PX2APoint.hpp"
#include "PX2AVector.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2XMLData.hpp"
#include "PX2FunObject.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
LogicManager::LogicManager()
{
}
//----------------------------------------------------------------------------
LogicManager::~LogicManager()
{
}
//----------------------------------------------------------------------------
bool LogicManager::InitlizeManyKitty()
{
	LogicModule *lm = 0;

	// Transform
	lm = new0 LogicModule();
	AddModule("Transform", lm);
	lm->TheFunObject.FunName = "Move";

	lm = new0 LogicModule();
	AddModule("Transform", lm);
	lm->TheFunObject.FunName = "Rotate";

	lm = new0 LogicModule();
	AddModule("Transform", lm);
	lm->TheFunObject.FunName = "FaceTo";

	// events
	lm = new0 LogicModule();
	AddModule("Event", lm);
	lm->TheFunObject.FunName = "Start";

	lm = new0 LogicModule();
	AddModule("Event", lm);
	lm->TheFunObject.FunName = "InitUpdate";

	lm = new0 LogicModule();
	AddModule("Event", lm);
	lm->TheFunObject.FunName = "Update";

	lm = new0 LogicModule();
	AddModule("Event", lm);
	lm->TheFunObject.FunName = "Destory";

	// Look
	lm = new0 LogicModule();
	AddModule("Look", lm);
	lm->TheFunObject.FunName = "Say";

	lm = new0 LogicModule();
	AddModule("Look", lm);
	lm->TheFunObject.FunName = "Think";

	lm = new0 LogicModule();
	AddModule("Look", lm);
	lm->TheFunObject.FunName = "SetColor";

	lm = new0 LogicModule();
	AddModule("Look", lm);
	lm->TheFunObject.FunName = "SetAlpha";

	// FPT_NONE,
	// FPT_INT,
	// FPT_FLOAT,
	// FPT_FLOAT3,
	// FPT_APOINT,
	// FPT_AVECTOR,
	// FPT_BOOL,
	// FPT_STRING,
	// FPT_POINTER,
	// FPT_POINTER_THIS,

	// params
	lm = new0 LogicModule();
	AddModule("Data", lm);
	lm->TheFunObject.FunName = "Param_Int";
	lm->TheFunObject.AddInput("in_param", FPT_INT, 0);
	lm->TheFunObject.AddOutput("ot_param", FPT_INT, 0);

	lm = new0 LogicModule();
	AddModule("Data", lm);
	lm->TheFunObject.FunName = "Param_Float";
	lm->TheFunObject.AddInput("in_param", FPT_FLOAT, 0.0f);
	lm->TheFunObject.AddOutput("ot_param", FPT_FLOAT, 0.0f);

	lm = new0 LogicModule();
	AddModule("Data", lm);
	lm->TheFunObject.FunName = "Param_Float3";
	lm->TheFunObject.AddInput("in_param", FPT_FLOAT3, Float3::ZERO);
	lm->TheFunObject.AddOutput("ot_param", FPT_FLOAT3, Float3::ZERO);

	lm = new0 LogicModule();
	AddModule("Data", lm);
	lm->TheFunObject.FunName = "Param_APoint";
	lm->TheFunObject.AddInput("in_param", FPT_APOINT, APoint::ORIGIN);
	lm->TheFunObject.AddOutput("ot_param", FPT_APOINT, APoint::ORIGIN);

	lm = new0 LogicModule();
	AddModule("Data", lm);
	lm->TheFunObject.FunName = "Param_AVector";
	lm->TheFunObject.AddInput("in_param", FPT_AVECTOR, AVector::ZERO);
	lm->TheFunObject.AddOutput("ot_param", FPT_AVECTOR, AVector::ZERO);

	lm = new0 LogicModule();
	AddModule("Data", lm);
	lm->TheFunObject.FunName = "Param_Bool";
	lm->TheFunObject.AddInput("in_param", FPT_BOOL, true);
	lm->TheFunObject.AddOutput("ot_param", FPT_BOOL, true);

	lm = new0 LogicModule();
	AddModule("Data", lm);
	lm->TheFunObject.FunName = "Param_String";
	lm->TheFunObject.AddInput("in_param", FPT_STRING, std::string(""));
	lm->TheFunObject.AddOutput("ot_param", FPT_STRING, std::string(""));

	lm = new0 LogicModule();
	AddModule("Data", lm);
	lm->TheFunObject.FunName = "Param_Pointer";
	lm->TheFunObject.AddInput("in_param", FPT_POINTER, (Object*)0);
	lm->TheFunObject.AddOutput("ot_param", FPT_POINTER, (Object*)0);

	// Control
	lm = new0 LogicModule();
	AddModule("Control", lm);
	lm->TheFunObject.FunName = "Compare";
	lm->TheFunObject.AddInput("in_param0", FPT_INT, 0);
	lm->TheFunObject.AddInput("in_param1", FPT_INT, 0);
	lm->TheFunObject.AddOutput("ot_true", FPT_BOOL, true);
	lm->TheFunObject.AddOutput("ot_false", FPT_BOOL, false);

	lm = new0 LogicModule();
	AddModule("Control", lm);
	lm->TheFunObject.FunName = "CompareString";
	lm->TheFunObject.AddInput("in_param0", FPT_STRING, std::string("param0"));
	lm->TheFunObject.AddInput("in_param1", FPT_STRING, std::string("param1"));
	lm->TheFunObject.AddOutput("ot_true", FPT_BOOL, true);
	lm->TheFunObject.AddOutput("ot_false", FPT_BOOL, false);

	lm = new0 LogicModule();
	AddModule("Control", lm);
	lm->TheFunObject.FunName = "Switch";
	lm->TheFunObject.AddInput("in_param", FPT_INT, 0);
	lm->TheFunObject.AddInput("in_c0", FPT_INT, 0);
	lm->TheFunObject.AddInput("in_c1", FPT_INT, 1);
	lm->TheFunObject.AddInput("in_c2", FPT_INT, 2);
	lm->TheFunObject.AddInput("in_c3", FPT_INT, 3);
	lm->TheFunObject.AddOutput("empty", FPT_NONE, Any());
	lm->TheFunObject.AddOutput("ot_equal0", FPT_BOOL, true);
	lm->TheFunObject.AddOutput("ot_equal1", FPT_BOOL, true);
	lm->TheFunObject.AddOutput("ot_equal2", FPT_BOOL, true);
	lm->TheFunObject.AddOutput("ot_equal3", FPT_BOOL, true);

	// functions
	lm = new0 LogicModule();
	AddModule("Function", lm);
	lm->TheFunObject.FunName = "FunctionStart";

	lm = new0 LogicModule();
	AddModule("Function", lm);
	lm->TheFunObject.FunName = "GetUI";
	lm->TheFunObject.AddOutput("ot_ui", FPT_POINTER, (Object*)0);

	lm = new0 LogicModule();
	AddModule("Function", lm);
	lm->TheFunObject.FunName = "SetInt";
	lm->TheFunObject.AddInput("in_from", FPT_INT, 0);
	lm->TheFunObject.AddInput("in_param", FPT_INT, 0);

	return true;
}
//----------------------------------------------------------------------------
bool LogicManager::Initlize()
{
	_InitlizeBP();

	return true;
}
//----------------------------------------------------------------------------
bool LogicManager::_InitlizeBP()
{
	LogicModule *lm = new0 LogicModule();
	AddModule("Events", lm);

	// events
	for (int i = 0; i < ProjectES::E_QUANTITY; i++)
	{
		FunObject entFunObj;
		entFunObj.ClassName = "ProjectES";
		//entFunObj.AddInput("in_datastr0", FPT_STRING, std::string(""));
		//entFunObj.AddInput("in_datastr1", FPT_STRING, std::string(""));
		//entFunObj.AddInput("in_datastr2", FPT_STRING, std::string(""));
		entFunObj.FunName = ProjectES::EventStrs[i];
		mEventObjects[entFunObj.ClassName].push_back(entFunObj);
		_AddGenEvent(entFunObj.ClassName, entFunObj.FunName);
	}

	// functions
	mFunStartObject.FunName = "FunctionStart";

	// scene
	FunObject funGetScene;
	funGetScene.FunName = "GetScene";
	funGetScene.AddOutput("ot_scene", FPT_POINTER, (Object*)0);
	mGenFunObjects[funGetScene.FunName] = funGetScene;
	_AddGenFun(funGetScene.FunName, "PX2_PROJ:GetScene");

	// ui
	FunObject funGetUI;
	funGetUI.FunName = "GetUI";
	funGetUI.AddOutput("ot_ui", FPT_POINTER, (Object*)0);
	mGenFunObjects[funGetUI.FunName] = funGetUI;
	_AddGenFun(funGetUI.FunName, "PX2_PROJ:GetUI");

	FunObject funGetLogger;
	funGetLogger.FunName = "LogInfo";
	funGetLogger.AddInput("in_tag", FPT_STRING, std::string("tag"));
	funGetLogger.AddInput("in_info", FPT_STRING, std::string("info"));
	mGenFunObjects[funGetLogger.FunName] = funGetLogger;
	_AddGenFun(funGetLogger.FunName, "PX2_LOGGER:LogInfo");

	// object funs
	// Object::RegistClassFunctions();
	// UIFrame::RegistClassFunctions();
	// UIButton::RegistClassFunctions();

	LoadPkgInfo("Data/engine/pkg/pkg.xml");

	// option
	FunObject fo_SetInt;
	fo_SetInt.FunName = "SetInt";
	fo_SetInt.AddInput("in_from", FPT_INT, 0);
	fo_SetInt.AddInput("in_param", FPT_INT, 0);
	mOptionObjects[fo_SetInt.FunName] = fo_SetInt;
	_AddOption(fo_SetInt.FunName);

	FunObject fo_SetFloat;
	fo_SetFloat.FunName = "SetFloat";
	fo_SetFloat.AddInput("in_from", FPT_FLOAT, 0.0f);
	fo_SetFloat.AddInput("in_param", FPT_FLOAT, 0.0f);
	mOptionObjects[fo_SetFloat.FunName] = fo_SetFloat;
	_AddOption(fo_SetFloat.FunName);

	FunObject fo_Compare;
	fo_Compare.FunName = "Compare";
	fo_Compare.AddInput("in_param0", FPT_INT, 0);
	fo_Compare.AddInput("in_param1", FPT_INT, 0);
	fo_Compare.AddOutput("ot_true", FPT_BOOL, true);
	fo_Compare.AddOutput("ot_false", FPT_BOOL, false);
	mOptionObjects[fo_Compare.FunName] = fo_Compare;
	_AddOption(fo_Compare.FunName);

	FunObject fo_CompareString;
	fo_CompareString.FunName = "CompareString";
	fo_CompareString.AddInput("in_param0", FPT_STRING, std::string("param0"));
	fo_CompareString.AddInput("in_param1", FPT_STRING, std::string("param1"));
	fo_CompareString.AddOutput("ot_true", FPT_BOOL, true);
	fo_CompareString.AddOutput("ot_false", FPT_BOOL, false);
	mOptionObjects[fo_CompareString.FunName] = fo_CompareString;
	_AddOption(fo_CompareString.FunName);

	FunObject fo_Switch;
	fo_Switch.FunName = "Switch4";
	fo_Switch.AddInput("in_param", FPT_INT, 0);
	fo_Switch.AddInput("in_c0", FPT_INT, 0);
	fo_Switch.AddInput("in_c1", FPT_INT, 1);
	fo_Switch.AddInput("in_c2", FPT_INT, 2);
	fo_Switch.AddInput("in_c3", FPT_INT, 3);
	fo_Switch.AddOutput("empty", FPT_NONE, Any());
	fo_Switch.AddOutput("ot_equal0", FPT_BOOL, true);
	fo_Switch.AddOutput("ot_equal1", FPT_BOOL, true);
	fo_Switch.AddOutput("ot_equal2", FPT_BOOL, true);
	fo_Switch.AddOutput("ot_equal3", FPT_BOOL, true);
	mOptionObjects[fo_Switch.FunName] = fo_Switch;
	_AddOption(fo_Switch.FunName);

	FunObject fo_SwitchString4;
	fo_SwitchString4.FunName = "Switch4";
	fo_SwitchString4.AddInput("in_param", FPT_STRING, std::string());
	fo_SwitchString4.AddInput("in_c0", FPT_STRING, std::string());
	fo_SwitchString4.AddInput("in_c1", FPT_STRING, std::string());
	fo_SwitchString4.AddInput("in_c2", FPT_STRING, std::string());
	fo_SwitchString4.AddInput("in_c3", FPT_STRING, std::string());
	fo_SwitchString4.AddOutput("empty", FPT_NONE, Any());
	fo_SwitchString4.AddOutput("ot_equal0", FPT_BOOL, true);
	fo_SwitchString4.AddOutput("ot_equal1", FPT_BOOL, true);
	fo_SwitchString4.AddOutput("ot_equal2", FPT_BOOL, true);
	fo_SwitchString4.AddOutput("ot_equal3", FPT_BOOL, true);
	mOptionObjects[fo_SwitchString4.FunName] = fo_SwitchString4;
	_AddOption(fo_SwitchString4.FunName);

	//FPT_NONE,
	//FPT_INT,
	//FPT_FLOAT,
	//FPT_FLOAT3,
	//FPT_APOINT,
	//FPT_AVECTOR,
	//FPT_BOOL,
	//FPT_STRING,
	//FPT_POINTER,
	//FPT_POINTER_THIS,

	// param
	FunObject p_Int;
	p_Int.FunName = "Param_Int";
	p_Int.AddInput("in_param", FPT_INT, 0);
	p_Int.AddOutput("ot_param", FPT_INT, 0);
	mParamObjects[p_Int.FunName] = p_Int;
	_AddParam(p_Int.FunName);

	FunObject p_Float;
	p_Float.FunName = "Param_Float";
	p_Float.AddInput("in_param", FPT_FLOAT, 0.0f);
	p_Float.AddOutput("ot_param", FPT_FLOAT, 0.0f);
	mParamObjects[p_Float.FunName] = p_Float;
	_AddParam(p_Float.FunName);

	FunObject p_Float3;
	p_Float3.FunName = "Param_Float3";
	p_Float3.AddInput("in_param", FPT_FLOAT3, Float3::ZERO);
	p_Float3.AddOutput("ot_param", FPT_FLOAT3, Float3::ZERO);
	mParamObjects[p_Float3.FunName] = p_Float3;
	_AddParam(p_Float3.FunName);

	FunObject p_APoint;
	p_APoint.FunName = "Param_APoint";
	p_APoint.AddInput("in_param", FPT_APOINT, APoint::ORIGIN);
	p_APoint.AddOutput("ot_param", FPT_APOINT, APoint::ORIGIN);
	mParamObjects[p_APoint.FunName] = p_APoint;
	_AddParam(p_APoint.FunName);

	FunObject p_AVector;
	p_AVector.FunName = "Param_AVector";
	p_AVector.AddInput("in_param", FPT_AVECTOR, AVector::ZERO);
	p_AVector.AddOutput("ot_param", FPT_AVECTOR, AVector::ZERO);
	mParamObjects[p_AVector.FunName] = p_AVector;
	_AddParam(p_AVector.FunName);

	FunObject p_Bool;
	p_Bool.FunName = "Param_Bool";
	p_Bool.AddInput("in_param", FPT_BOOL, true);
	p_Bool.AddOutput("ot_param", FPT_BOOL, true);
	mParamObjects[p_Bool.FunName] = p_Bool;
	_AddParam(p_Bool.FunName);

	FunObject p_String;
	p_String.FunName = "Param_String";
	p_String.AddInput("in_param", FPT_STRING, std::string(""));
	p_String.AddOutput("ot_param", FPT_STRING, std::string(""));
	mParamObjects[p_String.FunName] = p_String;
	_AddParam(p_String.FunName);

	FunObject p_Pointer;
	p_Pointer.FunName = "Param_Pointer";
	p_Pointer.AddInput("in_param", FPT_POINTER, (Object*)0);
	p_Pointer.AddOutput("ot_param", FPT_POINTER, (Object*)0);
	mParamObjects[p_Pointer.FunName] = p_Pointer;
	_AddParam(p_Pointer.FunName);

	// Operators
	FunObject fun_AVector_X_Float;
	fun_AVector_X_Float.FunName = "AVector_X_Float";
	fun_AVector_X_Float.AddInput("in_vec", FPT_AVECTOR, AVector::ZERO);
	fun_AVector_X_Float.AddInput("in_float", FPT_FLOAT, 0.0f);
	fun_AVector_X_Float.AddOutput("ot_vec", FPT_AVECTOR, AVector::ZERO);
	mOperatorsObjects[fun_AVector_X_Float.FunName] = fun_AVector_X_Float;
	_AddOPerator(fun_AVector_X_Float.FunName, "PX2_BPM:AVector_X_Float");

	FunObject fun_Float_X_Float;
	fun_Float_X_Float.FunName = "Float_X_Float";
	fun_Float_X_Float.AddInput("in_param0", FPT_FLOAT, 0.0f);
	fun_Float_X_Float.AddInput("in_param1", FPT_FLOAT, 0.0f);
	fun_Float_X_Float.AddOutput("ot_val", FPT_FLOAT, 0.0f);
	mOperatorsObjects[fun_Float_X_Float.FunName] = fun_Float_X_Float;
	_AddOPerator(fun_Float_X_Float.FunName, "PX2_BPM:Float_X_Float");

	return true;
}
//----------------------------------------------------------------------------
FunParamType GetParamType(const std::string &typeStr,
	Any &getAnyVal)
{
	// FPT_NONE,
	// FPT_INT,
	// FPT_FLOAT,
	// FPT_FLOAT3,
	// FPT_APOINT,
	// FPT_AVECTOR,
	// FPT_BOOL,
	// FPT_STRING,
	// FPT_POINTER,
	// FPT_POINTER_THIS,
	// FPT_POINTER_THIS_STATIC,
	// FPT_MAX_TYPE

	FunParamType fpt = FPT_NONE;
	Any anyVal;

	if ("void" == typeStr)
	{
		fpt = FPT_NONE;
	}
	else if ("int" == typeStr)
	{
		fpt = FPT_INT;
		anyVal = 0;
	}
	else if ("float" == typeStr || "double" == typeStr)
	{
		fpt = FPT_FLOAT;
		anyVal = 1.0f;
	}
	else if ("Float3" == typeStr)
	{
		fpt = FPT_FLOAT3;
		anyVal = Float3::ZERO;
	}
	else if ("APoint" == typeStr)
	{
		fpt = FPT_APOINT;
		anyVal = APoint::ORIGIN;
	}
	else if ("AVector" == typeStr)
	{
		fpt = FPT_AVECTOR;
		anyVal = AVector::ZERO;
	}
	else if ("bool" == typeStr)
	{
		fpt = FPT_BOOL;
		anyVal = true;
	}
	else if ("std::string" == typeStr || "string" == typeStr)
	{
		fpt = FPT_STRING;
		anyVal = std::string();
	}

	getAnyVal = anyVal;

	return fpt;
}
//----------------------------------------------------------------------------
bool LogicManager::LoadPkgInfo(const std::string &filename)
{
	std::map<std::string, FunObjectPtr> funObjectMap;

	int bufSize = 0;
	char *buf = 0;
	if (PX2_RM.LoadBuffer(filename, bufSize, buf))
	{
		XMLData data;
		if (data.LoadBuffer(buf, bufSize))
		{
			XMLNode enumsNode = data.GetNodeByPath("enums");

			XMLNode classesNode = data.GetNodeByPath("classes");
			XMLNode classNode = classesNode.IterateChild();
			while (!classNode.IsNull())
			{
				std::string className = classNode.AttributeToString("name");
				std::string parentName = classNode.AttributeToString("parentname");

				FunObjectPtr funObjectClass = new0 FunObject();
				funObjectMap[className] = funObjectClass;
				funObjectClass->IsClassCatalogue = true;
				funObjectClass->ClassName = className;
				funObjectClass->ParentClassName = parentName;

				XMLNode functionNode = classNode.IterateChild();
				while (!functionNode.IsNull())
				{
					std::string strFunName = functionNode.AttributeToString("name");
					std::string strFunType = functionNode.AttributeToString("funtype");
					bool is_operator = functionNode.AttributeToBool("is_operator");
					bool is_purevirtual = functionNode.AttributeToBool("is_purevirtual");
					bool is_virtual = functionNode.AttributeToBool("is_virtual");
					bool is_inline = functionNode.AttributeToBool("is_inline");
					bool is_const = functionNode.AttributeToBool("is_const");

					bool is_class_constructor = ("class_constructor" == strFunType);
					bool is_class_destructor = ("class_destructor" == strFunType);
					bool is_class_member = ("class_member" == strFunType);
					bool is_static = ("class_static" == strFunType);

					if (is_class_member || is_static)
					{
						FunObjectPtr funObj = new0 FunObject;
						funObj->ClassName = className;
						funObj->FunName = strFunName;
						funObjectClass->AddFunObject(funObj);

						if (is_class_member)
						{
							funObj->AddInput("handler", FPT_POINTER_THIS, (Object*)0);
						}
						else if (is_static)
						{
							funObj->AddInput("static", FPT_POINTER_THIS_STATIC, (Object*)0);
						}

						XMLNode paramNode = functionNode.IterateChild();
						while (!paramNode.IsNull())
						{
							std::string typestr = paramNode.AttributeToString("type_str");
							Any anyData;
							FunParamType fpt = GetParamType(typestr, anyData);

							bool is_return = paramNode.AttributeToBool("is_retrun");
							bool is_returnvoid = (std::string("void") == typestr);
							std::string valuename_str = paramNode.AttributeToString("valuename_str");
							bool is_pointer = paramNode.AttributeToBool("is_pointer");
							bool is_ref = paramNode.AttributeToBool("is_ref");

							if (!is_return)
							{
								if (is_pointer)
								{
									funObj->AddInput(valuename_str, FPT_POINTER, (Object*)0);
								}
								else
								{
									funObj->AddInput(valuename_str, fpt, anyData);
								}
							}
							else
							{
								if (!is_returnvoid)
								{
									if (is_pointer)
									{
										funObj->AddOutput("out_val", FPT_POINTER, (Object*)0);
									}
									else
									{
										funObj->AddOutput("out_val", fpt, anyData);
									}
								}
							} 

							paramNode = paramNode.IterateChild(paramNode);
						}
					}

					functionNode = functionNode.IterateChild(functionNode);
				}

				classNode = classNode.IterateChild(classNode);
			}
		}
	}

	auto it = funObjectMap.begin();
	for (; it != funObjectMap.end(); it++)
	{
		std::string funObjName = it->first;
		FunObject *funObj = it->second;

		if (funObj->IsClassCatalogue)
		{
			const std::string className = funObj->ClassName;
			const std::string &parentClassName = funObj->ParentClassName;

			if ("Object" == className)
			{
				FunObjectManager::GetSingleton().SetObjectFunObject(funObj);
			}

			if (!parentClassName.empty())
			{
				auto itParent = funObjectMap.find(parentClassName);
				if (itParent != funObjectMap.end())
				{
					(*itParent).second->AddFunObject(funObj);
				}
			}
		}
	}

	return true;
}
//----------------------------------------------------------------------------
void LogicManager::AddModule(const std::string &cataName, LogicModule *lm)
{
	std::map<std::string, LogicModuleCataloguePtr>::iterator it =
		mModuleCatalogues.find(cataName);
	if (it != mModuleCatalogues.end())
	{
		LogicModuleCatalogue *cal = it->second;
		cal->Modules.push_back(lm);
	}
	else
	{
		LogicModuleCatalogue *cal = new0 LogicModuleCatalogue();
		cal->SetName(cataName);
		cal->Modules.push_back(lm);
		mModuleCatalogues[cataName] = cal;
	}
}
//----------------------------------------------------------------------------
LogicModuleCatalogue *LogicManager::GetCatalogue(const std::string &cataName)
{
	std::map<std::string, LogicModuleCataloguePtr>::iterator it = 
		mModuleCatalogues.find(cataName);
	if (it != mModuleCatalogues.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
std::map<std::string, LogicModuleCataloguePtr>& LogicManager::
GetModuleCatalogues()
{
	return mModuleCatalogues;
}
//----------------------------------------------------------------------------
void LogicManager::AddFun_General(const FunObject &funObj,
	const std::string &script)
{
	mGenFunObjects[funObj.FunName] = funObj;
	_AddGenFun(funObj.FunName, script);
}
//----------------------------------------------------------------------------
FunObject &LogicManager::BeginAddFun_General(const std::string &funName,
	const std::string &script)
{
	mCurAddFunObj.ClearInParams();
	mCurAddFunObj.ClearOutParams();

	mCurAddFunObj.FunName = funName;
	_AddGenFun(mCurAddFunObj.FunName, script);
	return mCurAddFunObj;
}
//----------------------------------------------------------------------------
void LogicManager::AddInput(const std::string &name,
	FunParamType funParamType)
{
	if (FPT_INT == funParamType)
	{
		mCurAddFunObj.AddInput(name, funParamType, 0);
	}
	else if (FPT_FLOAT == funParamType)
	{
		mCurAddFunObj.AddInput(name, funParamType, 0.0f);
	}
	else if (FPT_APOINT == funParamType)
	{
		mCurAddFunObj.AddInput(name, funParamType, APoint::ORIGIN);
	}
	else if (FPT_AVECTOR == funParamType)
	{
		mCurAddFunObj.AddInput(name, funParamType, AVector::ZERO);
	}
	else if (FPT_BOOL == funParamType)
	{
		mCurAddFunObj.AddInput(name, funParamType, false);
	}
	else if (FPT_STRING == funParamType)
	{
		mCurAddFunObj.AddInput(name, funParamType, std::string(""));
	}
	else if (FPT_POINTER == funParamType)
	{
		mCurAddFunObj.AddInput(name, funParamType, (Object*)0);
	}
	else if (FPT_POINTER_THIS == funParamType)
	{
		mCurAddFunObj.AddInput(name, funParamType, (Object*)0);
	}
	else if (FPT_POINTER_THIS_STATIC == funParamType)
	{
		mCurAddFunObj.AddInput(name, funParamType, (Object*)0);
	}
}
//----------------------------------------------------------------------------
void LogicManager::AddOutput(const std::string &name,
	FunParamType funParamType)
{
	if (FPT_INT == funParamType)
	{
		mCurAddFunObj.AddOutput(name, funParamType, 0);
	}
	else if (FPT_FLOAT == funParamType)
	{
		mCurAddFunObj.AddOutput(name, funParamType, 0.0f);
	}
	else if (FPT_APOINT == funParamType)
	{
		mCurAddFunObj.AddOutput(name, funParamType, APoint::ORIGIN);
	}
	else if (FPT_AVECTOR == funParamType)
	{
		mCurAddFunObj.AddOutput(name, funParamType, AVector::ZERO);
	}
	else if (FPT_BOOL == funParamType)
	{
		mCurAddFunObj.AddOutput(name, funParamType, false);
	}
	else if (FPT_STRING == funParamType)
	{
		mCurAddFunObj.AddOutput(name, funParamType, std::string(""));
	}
	else if (FPT_POINTER == funParamType)
	{
		mCurAddFunObj.AddOutput(name, funParamType, (Object*)0);
	}
	else if (FPT_POINTER_THIS == funParamType)
	{
		mCurAddFunObj.AddOutput(name, funParamType, (Object*)0);
	}
	else if (FPT_POINTER_THIS_STATIC == funParamType)
	{
		mCurAddFunObj.AddOutput(name, funParamType, (Object*)0);
	}
}
//----------------------------------------------------------------------------
void LogicManager::EndAddFun_General()
{
	mGenFunObjects[mCurAddFunObj.FunName] = mCurAddFunObj;
}
//----------------------------------------------------------------------------
void LogicManager::_AddGenFun(const std::string &funName, const std::string &scriptStr)
{
	mGeneralFunctions.push_back(funName);
	mGeneralFunctionMap[funName] = scriptStr;
}
//----------------------------------------------------------------------------
void LogicManager::_AddGenEvent(const std::string &className, 
	const std::string &eventName)
{
	std::vector<std::string>::iterator it = std::find(mEventClasses.begin(),
		mEventClasses.end(), className);
	if (it == mEventClasses.end())
	{
		mEventClasses.push_back(className);
	}

	mEvents[className].push_back(eventName);
}
//----------------------------------------------------------------------------
void LogicManager::_AddOption(const std::string &optionFun)
{
	mOption.push_back(optionFun);
}
//----------------------------------------------------------------------------
void LogicManager::_AddParam(const std::string &param)
{
	mParam.push_back(param);
}
//----------------------------------------------------------------------------
void LogicManager::_AddOPerator(const std::string &op, const std::string &opStr)
{
	mOperators.push_back(op);
	mOperatorFunctionMap[op] = opStr;
}
//----------------------------------------------------------------------------
std::string LogicManager::GetGFScript(const std::string &name)
{
	std::map<std::string, std::string>::iterator it =
		mGeneralFunctionMap.find(name);
	if (it != mGeneralFunctionMap.end())
	{
		return it->second;
	}

	return "";
}
//----------------------------------------------------------------------------
FunObject *LogicManager::GetGF(const std::string &name)
{
	std::map<std::string, FunObject>::iterator it =
		mGenFunObjects.find(name);
	if (it != mGenFunObjects.end())
	{
		return &(it->second);
	}

	return 0;
}
//----------------------------------------------------------------------------
FunObject *LogicManager::GetEvent(const std::string &className,
	const std::string &name)
{
	std::map<std::string, std::vector<FunObject>>::iterator it =
		mEventObjects.find(className);
	if (it != mEventObjects.end())
	{
		std::vector<FunObject> &funObjs = it->second;
		for (int i = 0; i < (int)funObjs.size(); i++)
		{
			FunObject &funObj = funObjs[i];
			if (name == funObj.FunName)
			{
				return &funObj;
			}
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
FunObject *LogicManager::GetOption(const std::string &name)
{
	std::map<std::string, FunObject>::iterator it =
		mOptionObjects.find(name);
	if (it != mOptionObjects.end())
	{
		return &(it->second);
	}

	return 0;
}
//----------------------------------------------------------------------------
FunObject *LogicManager::GetParam(const std::string &name)
{
	std::map<std::string, FunObject>::iterator it =
		mParamObjects.find(name);
	if (it != mParamObjects.end())
	{
		return &(it->second);
	}

	return 0;
}
//----------------------------------------------------------------------------
FunObject *LogicManager::GetOperator(const std::string &name)
{
	std::map<std::string, FunObject>::iterator it =
		mOperatorsObjects.find(name);
	if (it != mOperatorsObjects.end())
	{
		return &(it->second);
	}

	return 0;
}
//----------------------------------------------------------------------------
std::string LogicManager::GetOPScript(const std::string &name)
{
	std::map<std::string, std::string>::iterator it =
		mOperatorFunctionMap.find(name);
	if (it != mOperatorFunctionMap.end())
	{
		return it->second;
	}

	return "";
}
//----------------------------------------------------------------------------
AVector LogicManager::AVector_X_Float(const AVector &vec, float scale)
{
	return vec * scale;
}
//----------------------------------------------------------------------------
float LogicManager::Float_X_Float(float param0, float param1)
{
	return param0 * param1;
}
//----------------------------------------------------------------------------'