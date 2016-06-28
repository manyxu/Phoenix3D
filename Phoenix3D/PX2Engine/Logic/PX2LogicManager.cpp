// PX2LogicManager.cpp

#include "PX2LogicManager.hpp"
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
	return true;
}
//----------------------------------------------------------------------------
bool LogicManager::Initlize()
{
	LogicModule *lm = new0 LogicModule();
	AddModule("Events", lm);

	lm = new0 LogicModule();
	AddModule("Events", lm);

	lm = new0 LogicModule();
	AddModule("Events", lm);

	lm = new0 LogicModule();
	AddModule("Events", lm);
	
	// events
	//for (int i = 0; i < SimuES::E_QUANTITY; i++)
	//{
	//	FunObject entFunObj;
	//	entFunObj.FunName = General_EventHandler::sGEStrings[i];
	//	mEventObjects[entFunObj.FunName] = entFunObj;
	//	_AddGenEvent(entFunObj.FunName);
	//}

	//// functions
	//mFunStartObject.FunName = "FunctionStart";

	////// scene
	////FunObject funGetScene;
	////funGetScene.FunName = "GetScene";
	////funGetScene.AddOutput("ot_scene", FPT_POINTER, (Object*)0);
	////mGenFunObjects[funGetScene.FunName] = funGetScene;
	////_AddGenFun(funGetScene.FunName, "PX2_PROJ:GetScene");

	////// ui
	////FunObject funGetUI;
	////funGetUI.FunName = "GetUI";
	////funGetUI.AddOutput("ot_ui", FPT_POINTER, (Object*)0);
	////mGenFunObjects[funGetUI.FunName] = funGetUI;
	////_AddGenFun(funGetUI.FunName, "PX2_PROJ:GetUI");

	//FunObject funGetUI;
	//funGetUI.FunName = "GetReleasedBut";
	//funGetUI.AddOutput("ot_but", FPT_POINTER, (Object*)0);
	//mGenFunObjects[funGetUI.FunName] = funGetUI;
	//_AddGenFun(funGetUI.FunName, "PX2_BPM:GetReleasedBut");

	//PX2_SC_LUA->CallFile("Data/engine/scripts/lua/engine.lua");

	//// object funs
	//Object::RegistClassFunctions();
	//UIFrame::RegistClassFunctions();
	//UIButton::RegistClassFunctions();

	//// option
	//FunObject fo_SetInt;
	//fo_SetInt.FunName = "SetInt";
	//fo_SetInt.AddInput("in_from", FPT_INT, 0);
	//fo_SetInt.AddInput("in_param", FPT_INT, 0);
	//mOptionObjects[fo_SetInt.FunName] = fo_SetInt;
	//_AddOption(fo_SetInt.FunName);

	//FunObject fo_SetFloat;
	//fo_SetFloat.FunName = "SetFloat";
	//fo_SetFloat.AddInput("in_from", FPT_FLOAT, 0.0f);
	//fo_SetFloat.AddInput("in_param", FPT_FLOAT, 0.0f);
	//mOptionObjects[fo_SetFloat.FunName] = fo_SetFloat;
	//_AddOption(fo_SetFloat.FunName);

	//FunObject fo_Compare;
	//fo_Compare.FunName = "Compare";
	//fo_Compare.AddInput("in_param0", FPT_INT, 0);
	//fo_Compare.AddInput("in_param1", FPT_INT, 0);
	//fo_Compare.AddOutput("ot_true", FPT_BOOL, true);
	//fo_Compare.AddOutput("ot_false", FPT_BOOL, false);
	//mOptionObjects[fo_Compare.FunName] = fo_Compare;
	//_AddOption(fo_Compare.FunName);

	//FunObject fo_CompareString;
	//fo_CompareString.FunName = "CompareString";
	//fo_CompareString.AddInput("in_param0", FPT_STRING, std::string("param0"));
	//fo_CompareString.AddInput("in_param1", FPT_STRING, std::string("param1"));
	//fo_CompareString.AddOutput("ot_true", FPT_BOOL, true);
	//fo_CompareString.AddOutput("ot_false", FPT_BOOL, false);
	//mOptionObjects[fo_CompareString.FunName] = fo_CompareString;
	//_AddOption(fo_CompareString.FunName);

	////FunObject fo_Switch;
	////fo_Switch.FunName = "Switch4";
	////fo_Switch.AddInput("in_param", FPT_INT, 0);
	////fo_Switch.AddInput("in_c0", FPT_INT, 0);
	////fo_Switch.AddInput("in_c1", FPT_INT, 1);
	////fo_Switch.AddInput("in_c2", FPT_INT, 2);
	////fo_Switch.AddInput("in_c3", FPT_INT, 3);
	////fo_Switch.AddOutput("empty", FPT_NONE, Any());
	////fo_Switch.AddOutput("ot_equal0", FPT_BOOL, true);
	////fo_Switch.AddOutput("ot_equal1", FPT_BOOL, true);
	////fo_Switch.AddOutput("ot_equal2", FPT_BOOL, true);
	////fo_Switch.AddOutput("ot_equal3", FPT_BOOL, true);
	////mOptionObjects[fo_Switch.FunName] = fo_Switch;
	////_AddOption(fo_Switch.FunName);

	//FunObject fo_SwitchString4;
	//fo_SwitchString4.FunName = "Switch4";
	//fo_SwitchString4.AddInput("in_param", FPT_STRING, std::string());
	//fo_SwitchString4.AddInput("in_c0", FPT_STRING, std::string());
	//fo_SwitchString4.AddInput("in_c1", FPT_STRING, std::string());
	//fo_SwitchString4.AddInput("in_c2", FPT_STRING, std::string());
	//fo_SwitchString4.AddInput("in_c3", FPT_STRING, std::string());
	//fo_SwitchString4.AddOutput("empty", FPT_NONE, Any());
	//fo_SwitchString4.AddOutput("ot_equal0", FPT_BOOL, true);
	//fo_SwitchString4.AddOutput("ot_equal1", FPT_BOOL, true);
	//fo_SwitchString4.AddOutput("ot_equal2", FPT_BOOL, true);
	//fo_SwitchString4.AddOutput("ot_equal3", FPT_BOOL, true);
	//mOptionObjects[fo_SwitchString4.FunName] = fo_SwitchString4;
	//_AddOption(fo_SwitchString4.FunName);

	//	FPT_NONE,
	//	FPT_INT,
	//	FPT_FLOAT,
	//	FPT_APOINT,
	//	FPT_AVECTOR,
	//	FPT_BOOL,
	//	FPT_STRING,
	//	FPT_POINTER,
	//	FPT_POINTER_THIS,

	// param
	//FunObject p_Int;
	//p_Int.FunName = "Param_Int";
	//p_Int.AddInput("in_param", FPT_INT, 0);
	//p_Int.AddOutput("ot_param", FPT_INT, 0);
	//mParamObjects[p_Int.FunName] = p_Int;
	//_AddParam(p_Int.FunName);

	//FunObject p_Float;
	//p_Float.FunName = "Param_Float";
	//p_Float.AddInput("in_param", FPT_FLOAT, 0.0f);
	//p_Float.AddOutput("ot_param", FPT_FLOAT, 0.0f);
	//mParamObjects[p_Float.FunName] = p_Float;
	//_AddParam(p_Float.FunName);

	//FunObject p_APoint;
	//p_APoint.FunName = "Param_APoint";
	//p_APoint.AddInput("in_param", FPT_APOINT, APoint::ORIGIN);
	//p_APoint.AddOutput("ot_param", FPT_APOINT, APoint::ORIGIN);
	//mParamObjects[p_APoint.FunName] = p_APoint;
	//_AddParam(p_APoint.FunName);

	//FunObject p_AVector;
	//p_AVector.FunName = "Param_AVector";
	//p_AVector.AddInput("in_param", FPT_AVECTOR, AVector::ZERO);
	//p_AVector.AddOutput("ot_param", FPT_AVECTOR, AVector::ZERO);
	//mParamObjects[p_AVector.FunName] = p_AVector;
	//_AddParam(p_AVector.FunName);

	//FunObject p_Bool;
	//p_Bool.FunName = "Param_Bool";
	//p_Bool.AddInput("in_param", FPT_BOOL, true);
	//p_Bool.AddOutput("ot_param", FPT_BOOL, true);
	//mParamObjects[p_Bool.FunName] = p_Bool;
	//_AddParam(p_Bool.FunName);

	//FunObject p_String;
	//p_String.FunName = "Param_String";
	//p_String.AddInput("in_param", FPT_STRING, std::string(""));
	//p_String.AddOutput("ot_param", FPT_STRING, std::string(""));
	//mParamObjects[p_String.FunName] = p_String;
	//_AddParam(p_String.FunName);

	//FunObject p_Pointer;
	//p_Pointer.FunName = "Param_Pointer";
	//p_Pointer.AddInput("in_param", FPT_POINTER, (Object*)0);
	//p_Pointer.AddOutput("ot_param", FPT_POINTER, (Object*)0);
	//mParamObjects[p_Pointer.FunName] = p_Pointer;
	//_AddParam(p_Pointer.FunName);

	//// Operators
	//FunObject fun_AVector_X_Float;
	//fun_AVector_X_Float.FunName = "AVector_X_Float";
	//fun_AVector_X_Float.AddInput("in_vec", FPT_AVECTOR, AVector::ZERO);
	//fun_AVector_X_Float.AddInput("in_float", FPT_FLOAT, 0.0f);
	//fun_AVector_X_Float.AddOutput("ot_vec", FPT_AVECTOR, AVector::ZERO);
	//mOperatorsObjects[fun_AVector_X_Float.FunName] = fun_AVector_X_Float;
	//_AddOPerator(fun_AVector_X_Float.FunName, "PX2_BPM:AVector_X_Float");

	//FunObject fun_Float_X_Float;
	//fun_Float_X_Float.FunName = "Float_X_Float";
	//fun_Float_X_Float.AddInput("in_param0", FPT_FLOAT, 0.0f);
	//fun_Float_X_Float.AddInput("in_param1", FPT_FLOAT, 0.0f);
	//fun_Float_X_Float.AddOutput("ot_val", FPT_FLOAT, 0.0f);
	//mOperatorsObjects[fun_Float_X_Float.FunName] = fun_Float_X_Float;
	//_AddOPerator(fun_Float_X_Float.FunName, "PX2_BPM:Float_X_Float");


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
FunObject *LogicManager::GetEvent(const std::string &name)
{
	std::map<std::string, FunObject>::iterator it =
		mEventObjects.find(name);
	if (it != mEventObjects.end())
	{
		return &(it->second);
	}

	return 0;
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
void LogicManager::_AddGenEvent(const std::string &eventName)
{
	mEvents.push_back(eventName);
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