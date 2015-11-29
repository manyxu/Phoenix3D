// PX2BPManager.cpp

#include "PX2BPManager.hpp"
#include "PX2Character.hpp"
#include "PX2Animation3DSkeleton.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2XMLData.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2SimulationEventType.hpp"
#include "PX2SimulationEventHandler.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
BPManager::BPManager()
:
mParamIndex(0)
{
	// events
	for (int i = 0; i < SimuES::E_QUANTITY; i++)
	{
		Object::FunObject entFunObj;
		entFunObj.FunName = SimuES_EventHandler::sGEStrings[i];
		mEventObjects[entFunObj.FunName] = entFunObj;
		_AddGenEvent(entFunObj.FunName);
	}

	// functions
	mFunStartObject.FunName = "FunctionStart";

	// scene
	Object::FunObject funGetScene;
	funGetScene.FunName = "GetScene";
	funGetScene.AddOutput("out_scene", Object::FPT_POINTER, (Object*)0);
	mFunObjects[funGetScene.FunName] = funGetScene;
	_AddGenFun(funGetScene.FunName, "PX2_PROJ:GetScene");

	// ui
	Object::FunObject funGetUI;
	funGetUI.FunName = "GetUI";
	funGetUI.AddOutput("out_ui", Object::FPT_POINTER, (Object*)0);
	mFunObjects[funGetUI.FunName] = funGetUI;
	_AddGenFun(funGetUI.FunName, "PX2_PROJ:GetUI");

	// option
	Object::FunObject fo_SetInt;
	fo_SetInt.FunName = "SetInt";
	fo_SetInt.AddInput("in_from", Object::FPT_INT, 0);
	fo_SetInt.AddInput("in_param", Object::FPT_INT, 0);
	mOptionObjects[fo_SetInt.FunName] = fo_SetInt;
	_AddOption(fo_SetInt.FunName);

	Object::FunObject fo_SetFloat;
	fo_SetFloat.FunName = "SetFloat";
	fo_SetFloat.AddInput("in_from", Object::FPT_FLOAT, 0.0f);
	fo_SetFloat.AddInput("in_param", Object::FPT_FLOAT, 0.0f);
	mOptionObjects[fo_SetFloat.FunName] = fo_SetFloat;
	_AddOption(fo_SetFloat.FunName);

	Object::FunObject fo_Compare;
	fo_Compare.FunName = "Compare";
	fo_Compare.AddInput("in_param0", Object::FPT_INT, 0);
	fo_Compare.AddInput("in_param1", Object::FPT_INT, 0);
	fo_Compare.AddOutput("out_true", Object::FPT_BOOL, true);
	fo_Compare.AddOutput("out_false", Object::FPT_BOOL, false);
	mOptionObjects[fo_Compare.FunName] = fo_Compare;
	_AddOption(fo_Compare.FunName);

	Object::FunObject fo_Switch;
	fo_Switch.FunName = "Switch4";
	fo_Switch.AddInput("in_param", Object::FPT_INT, 0);
	fo_Switch.AddInput("in_c0", Object::FPT_INT, 0);
	fo_Switch.AddInput("in_c1", Object::FPT_INT, 1);
	fo_Switch.AddInput("in_c2", Object::FPT_INT, 2);
	fo_Switch.AddInput("in_c3", Object::FPT_INT, 3);
	fo_Switch.AddOutput("empty", Object::FPT_NONE, Any());
	fo_Switch.AddOutput("out_equal0", Object::FPT_BOOL, true);
	fo_Switch.AddOutput("out_equal1", Object::FPT_BOOL, true);
	fo_Switch.AddOutput("out_equal2", Object::FPT_BOOL, true);
	fo_Switch.AddOutput("out_equal3", Object::FPT_BOOL, true);
	mOptionObjects[fo_Switch.FunName] = fo_Switch;
	_AddOption(fo_Switch.FunName);

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
	Object::FunObject p_Int;
	p_Int.FunName = "Param_Int";
	p_Int.AddInput("in_param", Object::FPT_INT, 0);
	p_Int.AddOutput("out_param", Object::FPT_INT, 0);
	mParamObjects[p_Int.FunName] = p_Int;
	_AddParam(p_Int.FunName);

	Object::FunObject p_Float;
	p_Float.FunName = "Param_Float";
	p_Float.AddInput("in_param", Object::FPT_FLOAT, 0.0f);
	p_Float.AddOutput("out_param", Object::FPT_FLOAT, 0.0f);
	mParamObjects[p_Float.FunName] = p_Float;
	_AddParam(p_Float.FunName);

	Object::FunObject p_APoint;
	p_APoint.FunName = "Param_APoint";
	p_APoint.AddInput("in_param", Object::FPT_APOINT, APoint::ORIGIN);
	p_APoint.AddOutput("out_param", Object::FPT_APOINT, APoint::ORIGIN);
	mParamObjects[p_APoint.FunName] = p_APoint;
	_AddParam(p_APoint.FunName);

	Object::FunObject p_AVector;
	p_AVector.FunName = "Param_AVector";
	p_AVector.AddInput("in_param", Object::FPT_AVECTOR, AVector::ZERO);
	p_AVector.AddOutput("out_param", Object::FPT_AVECTOR, AVector::ZERO);
	mParamObjects[p_AVector.FunName] = p_AVector;
	_AddParam(p_AVector.FunName);

	Object::FunObject p_Bool;
	p_Bool.FunName = "Param_Bool";
	p_Bool.AddInput("in_param", Object::FPT_BOOL, true);
	p_Bool.AddOutput("out_param", Object::FPT_BOOL, true);
	mParamObjects[p_Bool.FunName] = p_Bool;
	_AddParam(p_Bool.FunName);

	Object::FunObject p_String;
	p_String.FunName = "Param_String";
	p_String.AddInput("in_param", Object::FPT_STRING, std::string(""));
	p_String.AddOutput("out_param", Object::FPT_STRING, std::string(""));
	mParamObjects[p_String.FunName] = p_String;
	_AddParam(p_String.FunName);

	Object::FunObject p_Pointer;
	p_Pointer.FunName = "Param_Pointer";
	p_Pointer.AddInput("in_param", Object::FPT_POINTER, (Object*)0);
	p_Pointer.AddOutput("out_param", Object::FPT_POINTER, (Object*)0);
	mParamObjects[p_Pointer.FunName] = p_Pointer;
	_AddParam(p_Pointer.FunName);

	// Operators
	Object::FunObject fun_AVector_X_Float;
	fun_AVector_X_Float.FunName = "AVector_X_Float";
	fun_AVector_X_Float.AddInput("in_vec", Object::FPT_AVECTOR, AVector::ZERO);
	fun_AVector_X_Float.AddInput("in_float", Object::FPT_FLOAT, 0.0f);
	fun_AVector_X_Float.AddOutput("out_vec", Object::FPT_AVECTOR, AVector::ZERO);
	mOperatorsObjects[fun_AVector_X_Float.FunName] = fun_AVector_X_Float;
	_AddOPerator(fun_AVector_X_Float.FunName, "PX2_BPM:AVector_X_Float");

	Object::FunObject fun_Float_X_Float;
	fun_Float_X_Float.FunName = "Float_X_Float";
	fun_Float_X_Float.AddInput("in_param0", Object::FPT_FLOAT, 0.0f);
	fun_Float_X_Float.AddInput("in_param1", Object::FPT_FLOAT, 0.0f);
	fun_Float_X_Float.AddOutput("out_val", Object::FPT_FLOAT, 0.0f);
	mOperatorsObjects[fun_Float_X_Float.FunName] = fun_Float_X_Float;
	_AddOPerator(fun_Float_X_Float.FunName, "PX2_BPM:Float_X_Float");
}
//----------------------------------------------------------------------------
BPManager::~BPManager()
{
}
//----------------------------------------------------------------------------
void BPManager::_AddGenFun(const std::string &funName, const std::string &scriptStr)
{
	mGeneralFunctions.push_back(funName);
	mGeneralFunctionMap[funName] = scriptStr;
}
//----------------------------------------------------------------------------
void BPManager::_AddGenEvent(const std::string &eventName)
{
	mEvents.push_back(eventName);
}
//----------------------------------------------------------------------------
void BPManager::_AddOption(const std::string &optionFun)
{
	mOption.push_back(optionFun);
}
//----------------------------------------------------------------------------
void BPManager::_AddParam(const std::string &param)
{
	mParam.push_back(param);
}
//----------------------------------------------------------------------------
void BPManager::_AddOPerator(const std::string &op, const std::string &opStr)
{
	mOperators.push_back(op);
	mOperatorFunctionMap[op] = opStr;
}
//----------------------------------------------------------------------------
std::string BPManager::GetGFScript(const std::string &name)
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
Object::FunObject *BPManager::GetGF(const std::string &name)
{
	std::map<std::string, Object::FunObject>::iterator it =
		mFunObjects.find(name);
	if (it != mFunObjects.end())
	{
		return &(it->second);
	}

	return 0;
}
//----------------------------------------------------------------------------
void BPManager::ClearObjectFunMap()
{
	mObjectFunMap.clear();
}
//----------------------------------------------------------------------------
Object::FunObject *BPManager::GetFunctionObject(const std::string &className,
	const std::string &objName)
{
	std::map<std::string, std::vector<Object::FunObject> >::iterator it =
		mObjectFunMap.find(className);
	if (it != mObjectFunMap.end())
	{
		for (int i = 0; i < (int)it->second.size(); i++)
		{
			if (objName == it->second[i].FunName)
			{
				return &(it->second[i]);
			}
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
Object::FunObject *BPManager::GetEvent(const std::string &name)
{
	std::map<std::string, Object::FunObject>::iterator it =
		mEventObjects.find(name);
	if (it != mEventObjects.end())
	{
		return &(it->second);
	}

	return 0;
}
//----------------------------------------------------------------------------
Object::FunObject *BPManager::GetOption(const std::string &name)
{
	std::map<std::string, Object::FunObject>::iterator it =
		mOptionObjects.find(name);
	if (it != mOptionObjects.end())
	{
		return &(it->second);
	}

	return 0;
}
//----------------------------------------------------------------------------
Object::FunObject *BPManager::GetParam(const std::string &name)
{
	std::map<std::string, Object::FunObject>::iterator it =
		mParamObjects.find(name);
	if (it != mParamObjects.end())
	{
		return &(it->second);
	}

	return 0;
}
//----------------------------------------------------------------------------
Object::FunObject *BPManager::GetOperator(const std::string &name)
{
	std::map<std::string, Object::FunObject>::iterator it =
		mOperatorsObjects.find(name);
	if (it != mOperatorsObjects.end())
	{
		return &(it->second);
	}

	return 0;
}
//----------------------------------------------------------------------------
std::string BPManager::GetOPScript(const std::string &name)
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
const std::string &BPManager::Compile(BPFile *file)
{
	mParamIndex = 0;
	return file->Compile();
}
//----------------------------------------------------------------------------
void BPManager::Call(BPFile *file, bool alwaysCompile)
{
	if (!file) return;

	if (!file->IsEnable()) return;

	std::string script = file->GetCompiledString();

	bool doCompile = false;
	if (alwaysCompile)
	{
		doCompile = true;
	}
	else
	{
		if (script.empty())
			doCompile = true;
	}

	if (doCompile)
	{
		script = Compile(file);
	}

	PX2_SM.CallString(script.c_str());
}
//----------------------------------------------------------------------------
void BPManager::Call(const std::string &logicFilename)
{
	Object *bpObj = PX2_RM.BlockLoad(logicFilename);
	BPGroup *bpGroup = DynamicCast<BPGroup>(bpObj);
	BPFile *bpFile = DynamicCast<BPFile>(bpGroup);

	if (bpGroup)
	{
		Call(bpGroup, false);
	}
	else if (bpFile)
	{
		Call(bpFile, false);
	}
}
//----------------------------------------------------------------------------
void BPManager::Call(BPGroup *group, bool alwaysCompile)
{
	if (!group) return;

	if (!group->IsEnable()) return;

	for (int i = 0; i < group->GetNumLogicFiles(); i++)
	{
		BPFile *lf = group->GetLogicFile(i);
		Call(lf, alwaysCompile);
	}

	for (int i = 0; i < group->GetNumLogicGroups(); i++)
	{
		BPGroup *lg = group->GetLogicGroup(i);
		Call(lg, alwaysCompile);
	}
}
//----------------------------------------------------------------------------
int BPManager::GetParamIndex()
{
	return mParamIndex++;
}
//----------------------------------------------------------------------------
bool BPManager::CanLinkParam(PX2::BPParam *linkingParam,
	PX2::BPParam *linkedParam)
{
	if (!linkingParam)
		return false;

	if (!linkedParam)
		return false;

	Object::FunParamType linkingParamType = linkingParam->GetDataType();
	bool isLinkingExe = linkingParam->IsExe();
	BPModule::ModuleType linkingModuleType = linkingParam->GetModule()->GetModuleType();

	Object::FunParamType linkedParamType = linkedParam->GetDataType();
	bool isLinkedExe = linkedParam->IsExe();
	BPModule::ModuleType linkedModuleType = linkedParam->GetModule()->GetModuleType();

	if (linkingModuleType == BPModule::MT_EVENT && linkedModuleType != BPModule::MT_FUNCTION_START)
		return false;

	// 同一个输入点，只允许有一个输入参数，函数起点除外
	if (linkedParam->GetNumLinkMeParams() > 0 && BPModule::MT_FUNCTION_START != linkedModuleType)
		return false;

	if (linkedParam->IsIn() && (isLinkingExe == isLinkedExe))
	{
		if (linkingParamType == linkedParamType)
		{
			return true;
		}

		// 分支语句
		if (linkingModuleType == BPModule::MT_OPTION && linkedParamType == Object::FPT_NONE)
			return true;

		if (linkingParamType == Object::FPT_POINTER && linkedParamType == Object::FPT_POINTER_THIS)
			return true;

		if (linkingParamType == Object::FPT_POINTER_THIS && linkedParamType == Object::FPT_POINTER)
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
PX2::Polysegment *BPManager::CreatePoly(InterpCurveFloat *curve,
	const Float3 &color)
{
	VertexFormat *vf = VertexFormat::Create(2,
		VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
		VertexFormat::AU_COLOR, VertexFormat::AT_FLOAT4, 0);

	VertexBufferPtr vbuffer = new0 VertexBuffer(20, vf->GetStride(), Buffer::BU_DYNAMIC);
	Polysegment *p = new0 Polysegment(vf, vbuffer, true);

	UpdatePoly(curve, p, color);

	p->SetMaterialInstance(VertexColor4Material::CreateUniqueInstance());

	return p;
}
//----------------------------------------------------------------------------
void BPManager::UpdatePoly(InterpCurveFloat *curve, Polysegment *p,
	const Float3 &color)
{
	int keyNum = curve->GetNumPoints();
	if (0 == keyNum)
		return;

	VertexFormat *vFormat = p->GetVertexFormat();
	VertexBuffer *vBuffer = p->GetVertexBuffer();
	VertexBufferAccessor vba(vFormat, vBuffer);

	const InterpCurvePoint<float> &point = curve->Points[0];
	const InterpCurvePoint<float> &pointP1 = curve->Points[1];

	int numPoints = 0;
	for (int k = 0; k < 20; k++)
	{
		float inVal = Lerp<float, float>(point.InVal, pointP1.InVal, (float)k / 19.0f);
		float outVal = curve->Eval(inVal, 0.0f);

		vba.Position<Float3>(numPoints) = Float3(inVal, 0.0f, outVal);
		vba.Color<Float3>(0, numPoints) = color;
		numPoints++;
	}

	p->SetNumSegments(numPoints - 1);
	p->UpdateModelSpace(Renderable::GU_MODEL_BOUND_ONLY);

	Renderer::UpdateAll(vBuffer);
}
//----------------------------------------------------------------------------
AVector BPManager::AVector_X_Float(const AVector &vec, float scale)
{
	return vec * scale;
}
//----------------------------------------------------------------------------
float BPManager::Float_X_Float(float param0, float param1)
{
	return param0 * param1;
}
//----------------------------------------------------------------------------'