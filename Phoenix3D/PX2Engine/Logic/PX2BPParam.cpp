// PX2BPParam.cpp

#include "PX2BPParam.hpp"
#include "PX2BPModule.hpp"
#include "PX2BPManager.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, BPParam);
PX2_IMPLEMENT_STREAM(BPParam);
PX2_IMPLEMENT_FACTORY(BPParam);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, BPParam);

//----------------------------------------------------------------------------
BPParam::BPParam(bool isIn, bool isExe) :
mDataType(FPT_NONE),
mIsIn(isIn),
mIsExe(isExe),
mModule(0)
{
	EnableAnchorLayout(true);

	float butSize = 12.0f;
	SetSize(butSize, butSize);

	mBut = new0 UIButton();
	AttachChild(mBut);
	mBut->SetSize(butSize, butSize);

	mBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NORMAL);
	//mBut->GetPicBoxAtState(UIButtonBase::BS_HOVERED)->SetPicBoxType(UIPicBox::PBT_NORMAL);
	//mBut->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetPicBoxType(UIPicBox::PBT_NORMAL);
	//mBut->GetPicBoxAtState(UIButtonBase::BS_DISABLED)->SetPicBoxType(UIPicBox::PBT_NORMAL);
	//mBut->GetPicBoxAtState(UIButtonBase::BS_DISABLED)->SetColor(Float3::MakeColor(160, 160, 160));

	if (isIn)
	{
		mBut->SetAnchorHor(0.0f, 0.0f);
		mBut->SetAnchorVer(0.5f, 0.5f);
		mBut->SetPivot(Float2(0.0f, 0.5f));
		mBut->SetAnchorParamHor(0.0f, 0.0f);

		mBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetColor(Float3::MakeColor(163, 73, 164));
		//mBut->GetPicBoxAtState(UIButtonBase::BS_HOVERED)->SetColor(Float3::MakeColor(163, 73, 164));
		//mBut->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetColor(Float3::MakeColor(163, 73, 164));
		//mBut->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetBrightness(1.1f);
	}
	else
	{
		mBut->SetAnchorHor(1.0f, 1.0f);
		mBut->SetAnchorVer(0.5f, 0.5f);
		mBut->SetPivot(Float2(1.0f, 0.5f));
		mBut->SetAnchorParamHor(0.0f, 0.0f);

		mBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetColor(Float3::MakeColor(200, 191, 231));
		//mBut->GetPicBoxAtState(UIButtonBase::BS_HOVERED)->SetColor(Float3::MakeColor(200, 191, 231));
		//mBut->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetColor(Float3::MakeColor(200, 191, 231));
		//mBut->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetBrightness(1.1f);
	}

	mNameText = new0 UIFText();
	AttachChild(mNameText);
	mNameText->GetText()->SetColor(Float3::BLUE);
	mNameText->GetText()->SetFontScale(0.5f);
	mNameText->GetText()->SetDoPick(false);

	if (isIn)
	{
		mNameText->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
		mNameText->SetAnchorHor(0.0f, 1.0f);
		mNameText->SetAnchorVer(0.5f, 0.5f);
		mNameText->SetAnchorParamHor(butSize, 0.0f);
	}
	else
	{
		mNameText->GetText()->SetAligns(TEXTALIGN_RIGHT | TEXTALIGN_VCENTER);
		mNameText->SetAnchorHor(0.0f, 1.0f);
		mNameText->SetAnchorVer(0.5f, 0.5f);
		mNameText->SetAnchorParamHor(0.0f, -butSize);
	}

	if (!mIsIn)
	{
		mCurveLinking.AddPoint(0.0f, 0.0f, 0.0f, 0.0f, ICM_CURVE_AUTO);
		mCurveLinking.AddPoint(100.0f, 100.0f, 0.0f, 0.0f, ICM_CURVE_AUTO);

		mPolysegmentLinking = PX2_BPM.CreatePoly(&mCurveLinking, Float3::BLACK);
		AttachChild(mPolysegmentLinking);

		mPolysegmentLinking->Show(false);
	}

	mLinkingColor = Float3::BLACK;
}
//----------------------------------------------------------------------------
BPParam::~BPParam()
{
}
//----------------------------------------------------------------------------
void BPParam::SetDataType(FunParamType type)
{
	mDataType = type;
}
//----------------------------------------------------------------------------
void BPParam::SetLinkingWorldPos(const APoint &worldPos)
{
	mLinkingWorldPos = worldPos;

	AVector relate = worldPos - WorldTransform.GetTranslate();

	if (relate[0] >= 0.0f)
	{
		InterpCurvePointFloat point0;
		point0.ArriveTangent = 0.0f;
		point0.LeaveTangent = 0.0f;
		point0.InterpMode = ICM_CURVE_AUTO;
		point0.InVal = 0.0f;
		point0.OutVal = 0.0f;
		mCurveLinking.Points[0] = point0;

		InterpCurvePointFloat point;
		point.ArriveTangent = 0.0f;
		point.LeaveTangent = 0.0f;
		point.InterpMode = ICM_CURVE_AUTO;
		point.InVal = relate[0];
		point.OutVal = relate[2];
		mCurveLinking.Points[1] = point;
	}
	else
	{
		InterpCurvePointFloat point0;
		point0.ArriveTangent = 0.0f;
		point0.LeaveTangent = 0.0f;
		point0.InterpMode = ICM_CURVE_AUTO;
		point0.InVal = 0.0f;
		point0.OutVal = 0.0f;
		mCurveLinking.Points[1] = point0;

		InterpCurvePointFloat point;
		point.ArriveTangent = 0.0f;
		point.LeaveTangent = 0.0f;
		point.InterpMode = ICM_CURVE_AUTO;
		point.InVal = relate[0];
		point.OutVal = relate[2];
		mCurveLinking.Points[0] = point;
	}

	PX2_BPM.UpdatePoly(&mCurveLinking, mPolysegmentLinking, mLinkingColor);
	mPolysegmentLinking->Show(true);
}
//----------------------------------------------------------------------------
void BPParam::SetLinkingColor(const Float3 &color)
{
	mLinkingColor = color;
	PX2_BPM.UpdatePoly(&mCurveLinking, mPolysegmentLinking, mLinkingColor);
}
//----------------------------------------------------------------------------
void BPParam::DisLinking()
{
	mPolysegmentLinking->Show(false);
}
//----------------------------------------------------------------------------
void BPParam::PrepareCompile()
{
	mOutputScriptVarString = "_p_" + GetName() + StringHelp::IntToString(
		PX2_BPM.GetParamIndex());
}
//----------------------------------------------------------------------------
void BPParam::SetModule(BPModule *module)
{
	mModule = module;

	const Sizef &moduleSize = mModule->GetSize();
	float itemHeight = mModule->GetItemHeight();

	mNameText->GetText()->SetText(GetName());
}
//----------------------------------------------------------------------------
bool BPParam::SortFun(const BPParam *param0, const BPParam *param1)
{
	float z0 = param0->WorldTransform.GetTranslate().Z();
	float z1 = param1->WorldTransform.GetTranslate().Z();

	if (z0 == z1)
	{
		return param0 < param1;
	}
	else
	{
		return z0 > z1;
	}
}
//----------------------------------------------------------------------------
std::vector<BPParam *> BPParam::GetSortedParams()
{
	std::vector<BPParam *> params;

	for (int i = 0; i < (int)mLinkToParamVec.size(); i++)
	{
		params.push_back(mLinkToParamVec[i].TheParam);
	}

	std::sort(params.begin(), params.end(), SortFun);

	return params;
}
//----------------------------------------------------------------------------
bool BPParam::IsHasLinkToParam(BPParam *param)
{
	if (!param)
		return false;

	for (int i = 0; i < (int)mLinkToParamVec.size(); i++)
	{
		if (param == mLinkToParamVec[i].TheParam)
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool BPParam::AddLinkToParam(BPParam *param)
{
	if (!param)
		return false;

	if (IsHasLinkToParam(param))
		return false;

	param->AddLinkMeParam(this);

	AVector relate = param->WorldTransform.GetTranslate() -
		WorldTransform.GetTranslate();

	ParamObj obj;
	obj.TheParam = param;
	obj.TheCurve.AddPoint(0.0f, 0.0f, 0.0f, 0.0f, ICM_CURVE_AUTO);
	obj.TheCurve.AddPoint(relate[0], relate[2], 0.0f, 0.0f, ICM_CURVE_AUTO);
	obj.ThePolysegment = PX2_BPM.CreatePoly(&mCurveLinking, Float3::BLACK);

	mLinkToParamVec.push_back(obj);

	AttachChild(obj.ThePolysegment);

	return true;
}
//----------------------------------------------------------------------------
bool BPParam::RemoveLinkToParam(BPParam *param)
{
	if (!param)
		return false;

	if (!IsHasLinkToParam(param))
		return false;

	std::vector<ParamObj>::iterator it = mLinkToParamVec.begin();
	for (; it != mLinkToParamVec.end(); it++)
	{
		ParamObj &paramObj = *it;

		if (param == paramObj.TheParam)
		{
			paramObj.TheParam->RemoveLinkMeParam(this);
			DetachChild(paramObj.ThePolysegment);

			mLinkToParamVec.erase(it);

			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void BPParam::RemoveAllLinkToParams()
{
	std::vector<ParamObj>::iterator it = mLinkToParamVec.begin();
	for (; it != mLinkToParamVec.end(); it++)
	{
		ParamObj &obj = *it;

		obj.TheParam->RemoveLinkMeParam(this);

		DetachChild(obj.ThePolysegment);
	}

	mLinkToParamVec.clear();
}
//----------------------------------------------------------------------------
bool BPParam::AddLinkMeParam(BPParam *param)
{
	if (!param)
		return false;

	if (IsHasLinkMeParam(param))
		return false;

	mLinkMeParams.push_back(param);

	return true;
}
//----------------------------------------------------------------------------
bool BPParam::IsHasLinkMeParam(BPParam *param) const
{
	if (!param)
		return false;

	for (int i = 0; i < (int)mLinkMeParams.size(); i++)
	{
		if (param == mLinkMeParams[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool BPParam::RemoveLinkMeParam(BPParam *param)
{
	if (!param)
		return false;

	std::vector<BPParam *>::iterator it = mLinkMeParams.begin();
	for (; it != mLinkMeParams.end(); it++)
	{
		if (param == *it)
		{
			mLinkMeParams.erase(it);
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void BPParam::RemoveAllLinkMeParams()
{
	mLinkMeParams.clear();
}
//----------------------------------------------------------------------------
void BPParam::SetValue(const Any &any)
{
	mData = any;
}
//----------------------------------------------------------------------------
bool BPParam::GetValueB() const
{
	if (FPT_BOOL == mDataType)
		return PX2_ANY_AS(mData, bool);

	return false;
}
//----------------------------------------------------------------------------
int BPParam::GetValueI() const
{
	if (FPT_INT == mDataType)
		return PX2_ANY_AS(mData, int);

	return 0;
}
//----------------------------------------------------------------------------
float BPParam::GetValueF() const
{
	if (FPT_FLOAT == mDataType)
		return PX2_ANY_AS(mData, float);

	return 0.0f;
}
//----------------------------------------------------------------------------
APoint BPParam::GetValueAPoint() const
{
	if (FPT_APOINT == mDataType)
		return PX2_ANY_AS(mData, APoint);

	return APoint::ORIGIN;
}
//----------------------------------------------------------------------------
AVector BPParam::GetValueAVector() const
{
	if (FPT_AVECTOR == mDataType)
		return PX2_ANY_AS(mData, AVector);

	return AVector::ZERO;
}
//----------------------------------------------------------------------------
std::string BPParam::GetValueStr() const
{
	if (FPT_STRING == mDataType)
		return PX2_ANY_AS(mData, std::string);

	return "";
}
//----------------------------------------------------------------------------
Object *BPParam::GetValuePointer() const
{
	if (FPT_POINTER == mDataType || FPT_POINTER_THIS == mDataType)
		return PX2_ANY_AS(mData, Object*);

	return 0;
}
//----------------------------------------------------------------------------
std::string BPParam::GetValueScriptStr()
{
	std::string str;

	if (FPT_INT == mDataType)
	{
		str += StringHelp::IntToString(GetValueI());
	}
	else if (FPT_FLOAT == mDataType)
	{
		str += StringHelp::FloatToString(GetValueF());
	}
	else if (FPT_APOINT == mDataType)
	{
		APoint point = GetValueAPoint();
		str += "APoint("
			+ StringHelp::FloatToString(point[0]) + ", "
			+ StringHelp::FloatToString(point[1]) + ", "
			+ StringHelp::FloatToString(point[2]) + ")";
	}
	else if (FPT_AVECTOR == mDataType)
	{
		AVector vec = GetValueAVector();
		str += "AVector("
			+ StringHelp::FloatToString(vec[0]) + ", "
			+ StringHelp::FloatToString(vec[1]) + ", "
			+ StringHelp::FloatToString(vec[2]) + ")";
	}
	else if (FPT_BOOL == mDataType)
	{
		bool b = GetValueB();
		str += b ? "true" : "false";
	}
	else if (FPT_STRING == mDataType)
	{
		str += "\"" + GetValueStr() + "\"";
	}
	else if (FPT_POINTER == mDataType)
	{
		str += "Pointer_" + StringHelp::IntToString((int)GetValuePointer());
	}
	else if (FPT_POINTER_THIS == mDataType)
	{
		str += "Pointer_" + StringHelp::IntToString((int)GetValuePointer());
	}
	else if (FPT_POINTER_THIS_STATIC == mDataType)
	{
		str += GetName();
	}

	return str;
}
//----------------------------------------------------------------------------
void BPParam::UpdateCurve()
{
	for (int i = 0; i < (int)mLinkToParamVec.size(); i++)
	{
		ParamObj &paramObj = mLinkToParamVec[i];

		AVector relate = paramObj.TheParam->WorldTransform.GetTranslate() -
			WorldTransform.GetTranslate();

		if (relate[0] >= 0.0f)
		{
			InterpCurvePointFloat point0;
			point0.ArriveTangent = 0.0f;
			point0.LeaveTangent = 0.0f;
			point0.InterpMode = ICM_CURVE_AUTO;
			point0.InVal = 0.0f;
			point0.OutVal = 0.0f;
			paramObj.TheCurve.Points[0] = point0;

			InterpCurvePointFloat point;
			point.ArriveTangent = 0.0f;
			point.LeaveTangent = 0.0f;
			point.InterpMode = ICM_CURVE_AUTO;
			point.InVal = relate[0];
			point.OutVal = relate[2];
			paramObj.TheCurve.Points[1] = point;
		}
		else
		{
			InterpCurvePointFloat point0;
			point0.ArriveTangent = 0.0f;
			point0.LeaveTangent = 0.0f;
			point0.InterpMode = ICM_CURVE_AUTO;
			point0.InVal = 0.0f;
			point0.OutVal = 0.0f;
			paramObj.TheCurve.Points[1] = point0;

			InterpCurvePointFloat point;
			point.ArriveTangent = 0.0f;
			point.LeaveTangent = 0.0f;
			point.InterpMode = ICM_CURVE_AUTO;
			point.InVal = relate[0];
			point.OutVal = relate[2];
			paramObj.TheCurve.Points[0] = point;
		}

		Float3 color;
		if (mIsExe)
		{
			color = Float3::GREEN*0.8f;
		}
		else
		{
			color = Float3::YELLOW*0.8f;
		}
		PX2_BPM.UpdatePoly(&paramObj.TheCurve, paramObj.ThePolysegment,
			color);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void BPParam::RegistProperties()
{
	SetNamePropChangeable(false);

	Object::RegistProperties();

	AddPropertyClass("BPParam");

	AddProperty("IsIn", Object::PT_BOOL, IsIn(), false);
	AddProperty("IsExe", Object::PT_BOOL, IsExe(), false);

	if (!IsExe())
	{
		std::vector<std::string> paramTypes;
		paramTypes.push_back("FPT_NONE");
		paramTypes.push_back("FPT_INT");
		paramTypes.push_back("FPT_FLOAT");
		paramTypes.push_back("FPT_APOINT");
		paramTypes.push_back("FPT_AVECTOR");
		paramTypes.push_back("FPT_BOOL");
		paramTypes.push_back("FPT_STRING");
		paramTypes.push_back("FPT_POINTER");
		paramTypes.push_back("FPT_POINTER_THIS");
		paramTypes.push_back("FPT_POINTER_THIS_STATIC");
		AddPropertyEnum("DataType", (int)mDataType, paramTypes, false);

		if (mIsIn)
		{
			if (FPT_INT == mDataType)
			{
				AddProperty("Value", Object::PT_INT, GetValueI());
			}
			else if (FPT_FLOAT == mDataType)
			{
				AddProperty("Value", Object::PT_FLOAT, GetValueF());
			}
			else if (FPT_APOINT == mDataType)
			{
				AddProperty("Value", Object::PT_APOINT3, GetValueAPoint());
			}
			else if (FPT_AVECTOR == mDataType)
			{
				AddProperty("Value", Object::PT_AVECTOR3, GetValueAVector());
			}
			else if (FPT_BOOL == mDataType)
			{
				AddProperty("Value", Object::PT_BOOL, GetValueB());
			}
			else if (FPT_STRING == mDataType)
			{
				AddProperty("Value", Object::PT_STRING, GetValueStr());
			}
			else if (FPT_POINTER == mDataType || FPT_POINTER_THIS == mDataType)
			{
				AddProperty("Value", Object::PT_INT, (int)GetValuePointer());
			}
		}
	}
}
//----------------------------------------------------------------------------
void BPParam::OnPropertyChanged(const PropertyObject &obj)
{
	Object::OnPropertyChanged(obj);

	if (!IsExe())
	{
		if ("Value" == obj.Name)
		{
			if (FPT_POINTER == mDataType || FPT_POINTER_THIS == mDataType)
			{
				SetValue((void *)PX2_ANY_AS(obj.Data, int));
			}
			else
			{
				SetValue(obj.Data);
			}
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
BPParam::BPParam(LoadConstructor value)
:
UIFrame(value),
mIsIn(false),
mIsExe(false),
mModule(0),
mDataType(FPT_NONE)
{
	mLinkingColor = Float3::BLACK;
	mCurveLinking.AddPoint(0.0f, 0.0f, 0.0f, 0.0f, ICM_CURVE_AUTO);
	mCurveLinking.AddPoint(100.0f, 100.0f, 0.0f, 0.0f, ICM_CURVE_AUTO);
}
//----------------------------------------------------------------------------
void BPParam::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadBool(mIsIn);
	source.ReadBool(mIsExe);

	source.ReadEnum(mDataType);

	if (FPT_INT == mDataType)
	{
		int v;
		source.Read(v);
		mData = v;
	}
	else if (FPT_FLOAT == mDataType)
	{
		float v;
		source.Read(v);
		mData = v;
	}
	else if (FPT_APOINT == mDataType)
	{
		APoint v;
		source.ReadAggregate(v);
		mData = v;
	}
	else if (FPT_AVECTOR == mDataType)
	{
		AVector v;
		source.ReadAggregate(v);
		mData = v;
	}
	if (FPT_BOOL == mDataType)
	{
		bool b;
		source.ReadBool(b);
		mData = b;
	}
	else if (FPT_STRING == mDataType)
	{
		std::string v;
		source.ReadString(v);
		mData = v;
	}
	else if (FPT_POINTER == mDataType || FPT_POINTER_THIS == mDataType ||
		FPT_POINTER_THIS_STATIC == mDataType)
	{
		Object *p = 0;
		source.ReadPointer(p);
		mData = p;
	}

	int vecSize = 0;
	source.Read(vecSize);
	mLinkToParamVec.resize(vecSize);
	for (int i = 0; i < vecSize; i++)
	{
		BPParam *param = 0;
		source.ReadPointer(param);

		Polysegment *polysement = 0;
		source.ReadPointer(polysement);

		ParamObj paramObj;
		paramObj.TheParam = param;
		paramObj.TheCurve.AddPoint(0.0f, 0.0f, 0.0f, 0.0f, ICM_CURVE_AUTO);
		paramObj.TheCurve.AddPoint(100.0f, 0.0f, 0.0f, 0.0f, ICM_CURVE_AUTO);
		paramObj.ThePolysegment = polysement;

		mLinkToParamVec[i] = paramObj;
	}

	source.ReadPointer(mBut);
	source.ReadPointer(mNameText);
	source.ReadPointer(mPolysegmentLinking);
	source.ReadAggregate(mLinkingColor);

	PX2_END_DEBUG_STREAM_LOAD(BPParam, source);
}
//----------------------------------------------------------------------------
void BPParam::Link(InStream& source)
{
	UIFrame::Link(source);

	for (int i = 0; i < (int)mLinkToParamVec.size(); i++)
	{
		source.ResolveLink(mLinkToParamVec[i].TheParam);
		source.ResolveLink(mLinkToParamVec[i].ThePolysegment);
	}

	if (mBut)
		source.ResolveLink(mBut);

	if (mNameText)
		source.ResolveLink(mNameText);

	if (mPolysegmentLinking)
		source.ResolveLink(mPolysegmentLinking);
}
//----------------------------------------------------------------------------
void BPParam::PostLink()
{
	UIFrame::PostLink();

	if (mNameText)
		mNameText->SetDoPick(false);

	for (int i = 0; i < (int)mLinkToParamVec.size(); i++)
	{
		mLinkToParamVec[i].TheParam->PostLink();
		mLinkToParamVec[i].ThePolysegment->PostLink();

		mLinkToParamVec[i].TheParam->AddLinkMeParam(this);
	}

	if (mPolysegmentLinking)
	{
		mPolysegmentLinking->PostLink();
	}
}
//----------------------------------------------------------------------------
bool BPParam::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		for (int i = 0; i < (int)mLinkToParamVec.size(); i++)
		{
			target.Register(mLinkToParamVec[i].TheParam);
			target.Register(mLinkToParamVec[i].ThePolysegment);
		}

		if (mBut)
			target.Register(mBut);

		if (mNameText)
			target.Register(mNameText);

		if (mPolysegmentLinking)
			target.Register(mPolysegmentLinking);

		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void BPParam::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteBool(mIsIn);
	target.WriteBool(mIsExe);

	target.WriteEnum(mDataType);

	if (FPT_INT == mDataType)
	{
		int v = PX2_ANY_AS(mData, int);
		target.Write(v);
	}
	else if (FPT_FLOAT == mDataType)
	{
		float v = PX2_ANY_AS(mData, float);
		target.Write(v);
	}
	else if (FPT_APOINT == mDataType)
	{
		APoint v = PX2_ANY_AS(mData, APoint);
		target.WriteAggregate(v);
	}
	else if (FPT_AVECTOR == mDataType)
	{
		AVector v = PX2_ANY_AS(mData, AVector);
		target.WriteAggregate(v);
	}
	if (FPT_BOOL == mDataType)
	{
		bool b = PX2_ANY_AS(mData, bool);
		target.WriteBool(b);
	}
	else if (FPT_STRING == mDataType)
	{
		std::string v = PX2_ANY_AS(mData, std::string);
		target.WriteString(v);
	}
	else if (FPT_POINTER == mDataType || FPT_POINTER_THIS == mDataType ||
		FPT_POINTER_THIS_STATIC == mDataType)
	{
		Object *p = PX2_ANY_AS(mData, Object*);
		target.WritePointer(p);
	}

	int linkeToParamSize = (int)mLinkToParamVec.size();
	target.Write(linkeToParamSize);
	for (int i = 0; i < linkeToParamSize; i++)
	{
		target.WritePointer(mLinkToParamVec[i].TheParam);
		target.WritePointer(mLinkToParamVec[i].ThePolysegment);
	}

	target.WritePointer(mBut);
	target.WritePointer(mNameText);
	target.WritePointer(mPolysegmentLinking);
	target.WriteAggregate(mLinkingColor);

	PX2_END_DEBUG_STREAM_SAVE(BPParam, target);
}
//----------------------------------------------------------------------------
int BPParam::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_BOOLSIZE(mIsIn);
	size += PX2_BOOLSIZE(mIsExe);

	size += PX2_ENUMSIZE(mDataType);

	if (FPT_BOOL == mDataType)
	{
		//bool b = PX2_ANY_AS(mData, bool);
		size += PX2_BOOLSIZE(b);
	}
	else if (FPT_INT == mDataType)
	{
		//int v = PX2_ANY_AS(mData, int);
		size += sizeof(int);
	}
	else if (FPT_FLOAT == mDataType)
	{
		//float v = PX2_ANY_AS(mData, float);
		size += sizeof(float);
	}
	else if (FPT_APOINT == mDataType)
	{
		APoint v = PX2_ANY_AS(mData, APoint);
		size += sizeof(v);
	}
	else if (FPT_AVECTOR == mDataType)
	{
		AVector v = PX2_ANY_AS(mData, AVector);
		size += sizeof(v);
	}
	else if (FPT_STRING == mDataType)
	{
		std::string v = PX2_ANY_AS(mData, std::string);
		size += PX2_STRINGSIZE(v);
	}
	else if (FPT_POINTER == mDataType || FPT_POINTER_THIS == mDataType ||
		FPT_POINTER_THIS_STATIC == mDataType)
	{
		//Object *p = PX2_ANY_AS(mData, Object*);
		size += PX2_POINTERSIZE(0);
	}

	int linkToParamSize = (int)mLinkToParamVec.size();
	size += sizeof(linkToParamSize);
	for (int i = 0; i < linkToParamSize; i++)
	{
		size += PX2_POINTERSIZE(mLinkToParamVec[i].TheParam);
		size += PX2_POINTERSIZE(mLinkToParamVec[i].ThePolysegment);
	}

	size += PX2_POINTERSIZE(mBut);
	size += PX2_POINTERSIZE(mNameText);
	size += PX2_POINTERSIZE(mPolysegmentLinking);
	size += sizeof(mLinkingColor);

	return size;
}
//----------------------------------------------------------------------------