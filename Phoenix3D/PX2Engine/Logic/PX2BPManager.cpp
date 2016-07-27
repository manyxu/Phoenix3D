// PX2BPManager.cpp

#include "PX2BPManager.hpp"
#include "PX2Animation3DSkeleton.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2XMLData.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2FunObject.hpp"
#include "PX2Renderer.hpp"
#include "PX2VertexColor4Material.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
BPManager::BPManager() :
mParamIndex(0)
{
}
//----------------------------------------------------------------------------
BPManager::~BPManager()
{
}
//----------------------------------------------------------------------------
bool BPManager::Initlize()
{
	return true;
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

	//PX2_SM.CallString(script.c_str());
}
//----------------------------------------------------------------------------
void BPManager::Call(const std::string &logicFilename)
{
	Object *bpObj = PX2_RM.BlockLoad(logicFilename);
	BPPackage *bpGroup = DynamicCast<BPPackage>(bpObj);
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
void BPManager::Call(BPPackage *group, bool alwaysCompile)
{
	if (!group) return;

	if (!group->IsEnable()) return;

	for (int i = 0; i < group->GetNumBPFiles(); i++)
	{
		BPFile *lf = group->GetBPFile(i);
		Call(lf, alwaysCompile);
	}

	for (int i = 0; i < group->GetNumBPPackages(); i++)
	{
		BPPackage *lg = group->GetBPPackage(i);
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

	FunParamType linkingParamType = linkingParam->GetDataType();
	bool isLinkingExe = linkingParam->IsExe();
	BPModule::ModuleType linkingModuleType = linkingParam->GetModule()->GetModuleType();

	FunParamType linkedParamType = linkedParam->GetDataType();
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
		if (linkingModuleType == BPModule::MT_OPTION && linkedParamType == FPT_NONE)
			return true;

		if (linkingParamType == FPT_POINTER && linkedParamType == FPT_POINTER_THIS)
			return true;

		if (linkingParamType == FPT_POINTER_THIS && linkedParamType == FPT_POINTER)
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