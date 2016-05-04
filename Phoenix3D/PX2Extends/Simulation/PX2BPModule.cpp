// PX2BPModule.cpp

#include "PX2BPModule.hpp"
#include "PX2FontManager.hpp"
#include "PX2BPManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, BPModule);
PX2_IMPLEMENT_STREAM(BPModule);
PX2_IMPLEMENT_FACTORY(BPModule);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, BPModule);

//----------------------------------------------------------------------------
BPModule::BPModule(ModuleType type) :
mModuleType(type),
mOwnObjectParam(0),
mIsCompiled(false),
mIsCompilingAsParam_DoNotNeedCall(false)
{
	EnableAnchorLayout(false);
	SetPivot(0.0f, 1.0f);

	mItemHeight = 16.0f;
	mInOutStartPos = -mItemHeight*2.5f;

	mActInBut = new0 BPParam(true, true);
	AttachChild(mActInBut);
	mActInBut->SetName("");
	mActInBut->SetModule(this);
	mActInBut->LocalTransform.SetTranslateY(-1.0f);
	mActInBut->SetAnchorHor(0.0f, 0.0f);
	mActInBut->SetAnchorVer(1.0f, 1.0f);
	mActInBut->SetPivot(0.0f, 0.5f);
	mActInBut->SetAnchorParamHor(0.0f, 0.0f);
	mActInBut->SetAnchorParamVer(-mItemHeight / 2.0f, 0.0f);
	mActInBut->GetButton()->SetAnchorParamVer(2.0f, 0.0f);

	mActOutBut = new0 BPParam(false, true);
	AttachChild(mActOutBut);
	mActOutBut->SetName("");
	mActOutBut->SetModule(this);
	mActOutBut->LocalTransform.SetTranslateY(-1.0f);
	mActOutBut->SetAnchorHor(1.0f, 1.0f);
	mActOutBut->SetAnchorVer(1.0f, 1.0f);
	mActOutBut->SetPivot(1.0f, 0.5f);
	mActOutBut->SetAnchorParamHor(0.0f, 0.0f);
	mActOutBut->SetAnchorParamVer(-mItemHeight/2.0f, 0.0f);
	mActOutBut->GetButton()->SetAnchorParamVer(2.0f, 0.0f);

	mClassText = new0 UIFText();
	AttachChild(mClassText);
	mClassText->LocalTransform.SetTranslateY(-2.0f);
	mClassText->GetText()->SetFontScale(0.4f);
	mClassText->SetColor(Float3::GREEN);
	mClassText->GetText()->SetAligns(TEXTALIGN_HCENTER | TEXTALIGN_VCENTER);
	mClassText->SetAnchorHor(0.0f, 1.0f);
	mClassText->SetAnchorVer(1.0f, 1.0f);
	mClassText->SetAnchorParamVer(-mItemHeight*1.5f, 0.0f);
	mClassText->SetSize(0.0f, mItemHeight);
	mClassText->SetDoPick(false);
	UIPicBox *uiPicBox = mClassText->CreateAddBackgroundPicBox();
	mClassText->GetText()->LocalTransform.SetTranslateY(-1.0f);

	mNameText = new0 UIFText();
	AttachChild(mNameText);
	mNameText->LocalTransform.SetTranslateY(-1.0f);
	mNameText->GetText()->SetFontScale(0.5f);
	mNameText->SetColor(Float3::WHITE);
	mNameText->SetAnchorHor(0.0f, 1.0f);
	mNameText->SetAnchorVer(1.0f, 1.0f);
	mNameText->SetAnchorParamVer(-mItemHeight*0.5f, 0.0f);
	mNameText->SetSize(0.0f, mItemHeight);
	mNameText->SetDoPick(false);

	mSize = Sizef(125.0f, 50.0f);
	SetSize(mSize);

	UIPicBox *picBox = CreateAddBackgroundPicBox();
}
//----------------------------------------------------------------------------
BPModule::~BPModule()
{
}
//----------------------------------------------------------------------------
void BPModule::RegistFunObj(const FunObject &funObj)
{
	SetName(funObj.FunName);

	float fntMaxWidth0 = 0.0f;
	float fntMaxWidth1 = 0.0f;

	float fntParamInWidth = 0.0f;
	float fntParamHeight = 0.0f;

	Font *defFont = PX2_FM.GetDefaultFont();

	for (int i = 0; i < funObj.GetNumInParams(); i++)
	{
		const FunParam &funParam = funObj.GetInParam(i);

		std::map<std::string, BPParam*>::iterator it = mInputParams.find(funParam.Name);
		if (it == mInputParams.end())
		{
			BPParamPtr param = new0 BPParam();
			param->SetName(funParam.Name);
			param->SetDataType(funParam.Type);
			param->SetValue(funParam.Value);
			param->SetPivot(0.0f, 0.5f);
			param->SetAnchorHor(0.0f, 0.0f);
			param->SetAnchorVer(1.0f, 1.0f);
			param->SetAnchorParamVer(mInOutStartPos - i*mItemHeight, 0.0f);

			mInputParams[funParam.Name] = param;
			mInputParamsVec.push_back(param);

			AttachChild(param);
			param->LocalTransform.SetTranslateY(-1.0f);
			param->SetModule(this);

			if (FPT_POINTER_THIS == funParam.Type || FPT_POINTER_THIS_STATIC == funParam.Type)
			{
				mOwnObjectParam = param;
			}

			if (FPT_POINTER_THIS_STATIC == funParam.Type)
			{
				mOwnObjectParam->Enable(false);
			}

			float fontScale = param->GetNameText()->GetText()->GetFontScale();
			defFont->GetTextExtent(param->GetName().c_str(), fntParamInWidth, fntParamHeight, 
				false, fontScale);
			if (fntMaxWidth0 < fntParamInWidth)
				fntMaxWidth0 = fntParamInWidth;

			param->SetSize(fntMaxWidth0, mItemHeight);
		}
	}

	for (int i = 0; i < funObj.GetNumOutParams(); i++)
	{
		const FunParam &funParam = funObj.GetOutParam(i);

		std::map<std::string, BPParam*>::iterator it = mOutputParams.find(funParam.Name);
		if (it == mOutputParams.end())
		{
			bool isExe = false;
			if (MT_OPTION == mModuleType)
			{
				isExe = true;
			}

			BPParamPtr param = new0 BPParam(false, isExe);
			param->SetName(funParam.Name);
			param->SetDataType(funParam.Type);
			param->SetValue(funParam.Value);
			param->SetPivot(1.0f, 0.5f);
			param->SetAnchorHor(1.0f, 1.0f);
			param->SetAnchorVer(1.0f, 1.0f);
			param->SetAnchorParamVer(mInOutStartPos - i*mItemHeight, 0.0f);

			mOutputParams[funParam.Name] = param;
			mOutputParamsVec.push_back(param);

			AttachChild(param);
			param->LocalTransform.SetTranslateY(-1.0f);
			param->SetModule(this);

			if (FPT_NONE == funParam.Type)
			{
				param->Show(false);
			}

			float fontScale = param->GetNameText()->GetText()->GetFontScale();
			defFont->GetTextExtent(param->GetName().c_str(), fntParamInWidth, fntParamHeight, 
				false, fontScale);
			if (fntMaxWidth1 < fntParamInWidth)
				fntMaxWidth1 = fntParamInWidth;

			param->SetSize(fntMaxWidth1, mItemHeight);
		}
	}

	mNameText->GetText()->SetText(funObj.FunName);
	mClassText->GetText()->SetText(funObj.ClassName);

	float heightIn = -(mInOutStartPos - mItemHeight*0.5f - mItemHeight * (float)mInputParamsVec.size());
	float heightOut = -(mInOutStartPos - mItemHeight*0.5f - mItemHeight * (float)mOutputParamsVec.size());

	float widthLast = fntMaxWidth0 + fntMaxWidth1;
	mSize.Width = Mathf::Max(mSize.Width, widthLast);
	mSize.Height = Mathf::Max(heightIn, heightOut);

	float fntNameWidth = 0.0f;
	float fntNameHeight = 0.0f;
	float fontScale = mNameText->GetText()->GetFontScale();
	defFont->GetTextExtent(GetName().c_str(), fntNameWidth, fntNameHeight, 
		false, fontScale);
	fntNameWidth += 100.0f;

	mSize.Width = Mathf::Max(mSize.Width, fntNameWidth);

	SetSize(mSize);

	_UpdateModuleColorPos();
}
//----------------------------------------------------------------------------
const BPParam *BPModule::GetOwnerObjectParam() const
{
	return mOwnObjectParam;
}
//----------------------------------------------------------------------------
void BPModule::PrepareCompile()
{
	mActInBut->PrepareCompile();
	mActOutBut->PrepareCompile();

	for (int i = 0; i < (int)mInputParamsVec.size(); i++)
	{
		BPParam *param = mInputParamsVec[i];
		param->PrepareCompile();
	}

	for (int i = 0; i < (int)mOutputParamsVec.size(); i++)
	{
		BPParam *param = mOutputParamsVec[i];
		param->PrepareCompile();

		for (int i = 0; i < param->GetNumLinkToParams(); i++)
		{
			BPParam *linkToParam = param->GetLinkToParam(i);
			if (linkToParam && linkToParam->IsExe())
			{
				BPModule *linkToParamModule = linkToParam->GetModule();
				if (linkToParamModule)
				{
					linkToParamModule->PrepareCompile();
				}
			}
		}
	}

	for (int i = 0; i < mActOutBut->GetNumLinkToParams(); i++)
	{
		BPModule *linktoModule = mActOutBut->GetLinkToParam(i)->GetModule();
		linktoModule->PrepareCompile();
	}

	mIsCompiled = false;
}
//----------------------------------------------------------------------------
void BPModule::PrepareCompile_SetNoCompile()
{
	mIsCompiled = false;

	for (int i = 0; i < mActOutBut->GetNumLinkToParams(); i++)
	{
		BPModule *linktoModule = mActOutBut->GetLinkToParam(i)->GetModule();
		linktoModule->PrepareCompile_SetNoCompile();
	}
}
//----------------------------------------------------------------------------
void BPModule::Compile(std::string &script, int numTable,
	bool isOriginalStart)
{
	if (MT_EVENT == mModuleType)
	{
		std::string outputScriptVarString = mActOutBut->GetOutputScriptVarString();
		script += std::string("local ") + outputScriptVarString
			+ " = "
			+ "ScriptHandler:Create(" + "\"" + GetName() + "\"" + ")\n";

		mIsCompiled = true;

		std::vector<BPParam*> linkToParams = mActOutBut->GetSortedParams();
		for (int i = 0; i < (int)linkToParams.size(); i++)
		{
			BPModule *linktoModule = linkToParams[i]->GetModule();
			script += outputScriptVarString +
				":AddCall(" + "\"" + linktoModule->GetFunctionName() + "\"" + ")\n";
		}
	}
	else if (MT_FUNCTION_START == mModuleType)
	{
		if (isOriginalStart)
		{ // 函数定义

			if (mIsCompiled)
				return;

			std::vector<BPParam*> linkToParams = mActOutBut->GetSortedParams();
			for (int i = 0; i < (int)linkToParams.size(); i++)
			{
				BPModule *linktoModule = linkToParams[i]->GetModule();
				linktoModule->Compile(script, numTable + 1, false);
			}

			mIsCompiled = true;
		}
		else
		{ // 函数调用
			for (int i = 0; i < numTable; i++) script += "	";
			script += GetFunctionName() + " ()\n";
		}
	}
	else if (MT_PARAM == mModuleType)
	{
		if (isOriginalStart)
		{ // 定义
			mModuleAs_ParamName = "gParam_" + GetParent()->GetName() + "_" + GetName();

			script += mModuleAs_ParamName + " = " + mInputParamsVec[0]->GetValueScriptStr();
		}
		else
		{
			if (mIsCompiled)
				return;

			if (mInputParamsVec[0]->GetNumLinkMeParams() > 0)
			{ // 当变量有输入时，才需要处理预先赋值语句"="
				_ProcessInputParamsDepend(script, numTable);

				for (int i = 0; i < numTable; i++) script += "	";
				script += mModuleAs_ParamName + " = ";
				_ProcessInputParams(script);
				script += "\n";
			}

			mIsCompiled = true;
		}
	}
	else if (MT_FUNCTION_OBJECT == mModuleType ||
		MT_FUNCTION_GENERAL == mModuleType ||
		MT_FUNCTION_OPERATOR == mModuleType)
	{
		if (mIsCompiled)
			return;

		_ProcessInputParamsDepend(script, numTable);

		_ProcessOutputParams(script, numTable);
		_ProcessOperations(script);

		script += "(";
		_ProcessInputParams(script);
		script += ")\n";

		mIsCompiled = true;

		if (!mIsCompilingAsParam_DoNotNeedCall)
		{
			std::vector<BPParam*> linkToParams = mActOutBut->GetSortedParams();
			for (int i = 0; i < (int)linkToParams.size(); i++)
			{
				BPModule *linktoModule = linkToParams[i]->GetModule();
				linktoModule->Compile(script, numTable, false);
			}
		}
	}
	else if (MT_OPTION == mModuleType)
	{
		if (mIsCompiled)
			return;

		_ProcessInputParamsDepend(script, numTable);

		const std::string &moduleName = GetName();
		if ("SetFloat" == moduleName)
		{
			for (int i = 0; i < numTable; i++) script += "	";
			_ProcessInputParams(script);
			script += "\n";

			std::vector<BPParam*> linkToParams = mActOutBut->GetSortedParams();
			for (int i = 0; i < (int)linkToParams.size(); i++)
			{
				BPModule *linktoModule = linkToParams[i]->GetModule();
				linktoModule->Compile(script, numTable, false);
			}
		}
		else if ("Compare" == moduleName || "CompareString"==moduleName)
		{
			script += "\n";
			for (int i = 0; i < numTable; i++) script += "	";
			script += "if "; _ProcessInputParams(script); script += " then\n";
			{
				BPParam *lmParamTrue = mOutputParamsVec[0];

				std::vector<BPParam*> linkToParams = lmParamTrue->GetSortedParams();
				for (int i = 0; i < (int)linkToParams.size(); i++)
				{
					BPModule *linktoModule = linkToParams[i]->GetModule();
					linktoModule->PrepareCompile_SetNoCompile(); // 分支语句可以反复编译
					linktoModule->Compile(script, numTable + 1, false);
				}
			}

			for (int i = 0; i < numTable; i++) script += "	";
			script += "else\n";
			{
				for (int i = 0; i < numTable; i++) script += "	";

				BPParam *lmParamFalse = mOutputParamsVec[1];

				std::vector<BPParam*> linkToParams = lmParamFalse->GetSortedParams();
				for (int i = 0; i < (int)linkToParams.size(); i++)
				{
					BPModule *linktoModule = linkToParams[i]->GetModule();
					linktoModule->PrepareCompile_SetNoCompile(); // 分支语句可以反复编译
					linktoModule->Compile(script, numTable + 1, false);
				}
			}
			for (int j = 0; j < numTable; j++) script += "	";
			script += "end\n";
			script += "\n";
		}
		else if (moduleName.find("Switch4") != std::string::npos)
		{
			std::string numStr = moduleName.substr(std::string("Switch").size(),
				(int)moduleName.size() - 1);
			int switchNum = StringHelp::StringToInt(numStr);

			script += "\n";
			for (int i = 0; i < numTable; i++) script += "	";
			script += "if "; _ProcessInputParams(script, 0, 1); script += " then\n";
			{
				BPParam *lmParamTrue = mOutputParamsVec[1];

				std::vector<BPParam*> linkToParams = lmParamTrue->GetSortedParams();
				for (int i = 0; i < (int)linkToParams.size(); i++)
				{
					BPModule *linktoModule = linkToParams[i]->GetModule();
					linktoModule->PrepareCompile_SetNoCompile(); // 分支语句可以反复编译
					linktoModule->Compile(script, numTable + 1, false);
				}
			}

			for (int i = 1; i < switchNum; i++)
			{
				for (int j = 0; j < numTable; j++) script += "	";

				script += "elseif "; _ProcessInputParams(script, 0, i + 1); script += " then\n";
				{
					BPParam *lmParamTrue = mOutputParamsVec[i + 1];

					std::vector<BPParam*> linkToParams = lmParamTrue->GetSortedParams();
					for (int k = 0; k < (int)linkToParams.size(); k++)
					{
						BPModule *linktoModule = linkToParams[k]->GetModule();
						linktoModule->PrepareCompile_SetNoCompile(); // 分支语句可以反复编译
						linktoModule->Compile(script, numTable + 1, false);
					}
				}
			}
			for (int j = 0; j < numTable; j++) script += "	";
			script += "end\n";
			script += "\n";
		}

		mIsCompiled = true;
	}
}
//----------------------------------------------------------------------------
void BPModule::FunStartOnPreCompile(std::string &script)
{
	if (MT_FUNCTION_START == mModuleType)
	{
		script += "\nfunction " + GetFunctionName() + " ()\n";
	}
}
//----------------------------------------------------------------------------
std::string BPModule::GetFunctionName()
{
	BPFile *bpFile = DynamicCast<BPFile>(GetParent());
	if (bpFile)
	{
		return bpFile->GetName() + "_" + GetName();
	}

	return GetName();
}
//----------------------------------------------------------------------------
void BPModule::FunEndOnAfterCompile(std::string &script)
{
	if (MT_FUNCTION_START == mModuleType)
	{
		script += "end\n";
	}
}
//----------------------------------------------------------------------------
void BPModule::_ProcessInputParamsDepend(std::string &script, int numTable)
{
	for (int i = 0; i < (int)mInputParamsVec.size(); i++)
	{
		BPParam *inputParam = mInputParamsVec[i];

		int numLinkMeParams = inputParam->GetNumLinkMeParams();
		for (int i = 0; i < numLinkMeParams; i++)
		{ // 一般说来，这里只会有一个
			BPParam *linkMeParam = inputParam->GetLinkMeParam(i);

			BPModule *linkMeParamModule = linkMeParam->GetModule();
			bool isLinkMeParamModuleCompiled = linkMeParamModule->IsCompiled();
			BPModule::ModuleType linkMeParamModuleType = linkMeParamModule->GetModuleType();

			if (!isLinkMeParamModuleCompiled &&
				(linkMeParamModuleType == BPModule::MT_FUNCTION_OBJECT ||
				linkMeParamModuleType == BPModule::MT_FUNCTION_GENERAL ||
				linkMeParamModuleType == BPModule::MT_FUNCTION_OPERATOR ||
				linkMeParamModuleType == BPModule::MT_PARAM))
			{
				linkMeParamModule->SetCompilingAsParam(true);
				linkMeParamModule->Compile(script, numTable, false);
				linkMeParamModule->SetCompilingAsParam(false);
			}
		}
	}
}
//----------------------------------------------------------------------------
void BPModule::_ProcessOutputParams(std::string &script, int numTable)
{
	for (int i = 0; i<numTable; i++) script += "	";
	if ((int)mOutputParamsVec.size() > 0)
	{ // 现在只支持一个输出
		script += "local " + mOutputParamsVec[0]->GetOutputScriptVarString() + " = ";
	}
}
//----------------------------------------------------------------------------
void BPModule::_ProcessOperations(std::string &script)
{
	if (MT_FUNCTION_OBJECT == mModuleType)
	{
		// owner
		std::string callObjectStr;
		if (mOwnObjectParam)
		{
			FunParamType fpt = mOwnObjectParam->GetDataType();
			if (fpt == FPT_POINTER_THIS)
			{
				int numLinkMeParams = mOwnObjectParam->GetNumLinkMeParams();
				if (numLinkMeParams > 0)
				{ // 一般说来，这里只会有一个
					const BPParam *linkMeParam = mOwnObjectParam->GetLinkMeParam(0);
					callObjectStr = linkMeParam->GetOutputScriptVarString();
				}
				else
				{
					callObjectStr = mOwnObjectParam->GetValueScriptStr();
				}

				// params
				script += callObjectStr + ":" + GetName();
			}
			else
			{
				callObjectStr = mClassText->GetText()->GetText();

				script += callObjectStr + ":" + GetName();
			}
		}
	}
	else if (MT_FUNCTION_GENERAL == mModuleType)
	{
		std::string scriptStr = PX2_BPM.GetGFScript(GetName());

		// params
		script += scriptStr;
	}
	else if (MT_FUNCTION_OPERATOR == mModuleType)
	{
		std::string scriptStr = PX2_BPM.GetOPScript(GetName());

		// params
		script += scriptStr;
	}
}
//----------------------------------------------------------------------------
void BPModule::_ProcessInputParams(std::string &script,
	int paramIndex0, int paramIndex1)
{
	int inputParamSize = (int)mInputParamsVec.size();
	for (int i = 0; i < inputParamSize; i++)
	{
		if (paramIndex0 != -1 && paramIndex1 != -1)
		{
			if (paramIndex0 != i && paramIndex1 != i)
				continue;
		}

		BPParam *param = mInputParamsVec[i];

		if (!param->IsOwnObjectParam())
		{
			std::string paramStr;

			int numLinkMeParams = param->GetNumLinkMeParams();
			if (numLinkMeParams > 0)
			{ // 一般说来，这里只会有一个
				const BPParam *linkMeParam = param->GetLinkMeParam(0);
				const BPModule *linkMeModule = linkMeParam->GetModule();

				if (BPModule::MT_PARAM == linkMeModule->GetModuleType())
				{
					paramStr = linkMeModule->GetModuleAs_ParamName();
				}
				else
				{
					paramStr = linkMeParam->GetOutputScriptVarString();
				}
			}
			else
			{
				paramStr = param->GetValueScriptStr();
			}

			script += paramStr;

			if (MT_FUNCTION_OBJECT == mModuleType ||
				MT_FUNCTION_GENERAL == mModuleType ||
				MT_FUNCTION_OPERATOR == mModuleType)
			{
				if (i != ((int)inputParamSize - 1))
				{
					script += ", ";
				}
			}
			else if (MT_OPTION == mModuleType)
			{
				const std::string &moduleName = GetName();
				if ("SetFloat" == moduleName)
				{
					if (i != ((int)inputParamSize - 1))
					{
						script += " = ";
					}
				}
				else if ("Compare" == moduleName)
				{
					if (i != ((int)inputParamSize - 1))
					{
						script += " == ";
					}
				}
				else if (moduleName.find("Switch") != std::string::npos)
				{
					if (i == paramIndex0)
					{
						script += " == ";
					}
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void BPModule::_UpdateModuleColorPos()
{
	if (mBackgroundPicBox)
		mBackgroundPicBox->SetColor(Float3::MakeColor(230, 230, 230));

	UIPicBox *classPB = mClassText->GetBackgroundPicBox();

	if (MT_EVENT == mModuleType)
	{
		if (mBackgroundPicBox)
			mBackgroundPicBox->SetColor(Float3::MakeColor(255, 174, 201));

		mClassText->Show(false);
	}
	else if (MT_FUNCTION_START == mModuleType)
	{
		if (mBackgroundPicBox)
			mBackgroundPicBox->SetColor(Float3::MakeColor(153, 217, 234));

		mClassText->Show(false);
	}
	else if (MT_FUNCTION_OBJECT == mModuleType)
	{
		if (mBackgroundPicBox)
			mBackgroundPicBox->SetColor(Float3::MakeColor(230, 230, 230));

		mClassText->Show(true);
	}
	else if (MT_OPTION == mModuleType)
	{
		if (mBackgroundPicBox)
			mBackgroundPicBox->SetColor(Float3::MakeColor(206, 239, 107));

		mClassText->Show(false);
	}
	else if (MT_PARAM == mModuleType)
	{
		if (mBackgroundPicBox)
			mBackgroundPicBox->SetColor(Float3::MakeColor(239, 228, 176));

		mClassText->Show(false);
	}

	if (MT_OPTION == mModuleType)
	{
		mActOutBut->Show(false);

		if (GetName().find("Set") != std::string::npos)
		{
			mActOutBut->Show(true);
		}
	}

	if (MT_PARAM == mModuleType)
	{
		mActInBut->Show(false);
		mActOutBut->Show(false);
	}
}
//----------------------------------------------------------------------------
void BPModule::OnDetach()
{
	for (int i = 0; i < (int)mInputParamsVec.size(); i++)
	{
		BPParam *param = mInputParamsVec[i];
		for (int i = 0; i < param->GetNumLinkMeParams(); i++)
		{
			BPParam *linkMeParam = param->GetLinkMeParam(i);
			linkMeParam->RemoveLinkToParam(param);
		}
	}

	for (int i = 0; i < (int)mOutputParamsVec.size(); i++)
	{
		BPParam *param = mOutputParamsVec[i];
		param->RemoveAllLinkToParams();
	}

	if (mActInBut)
	{
		for (int i = 0; i < mActInBut->GetNumLinkMeParams(); i++)
		{
			BPParam *linkMeParam = mActInBut->GetLinkMeParam(i);
			linkMeParam->RemoveLinkToParam(mActInBut);
		}
	}

	if (mActOutBut)
	{
		mActOutBut->RemoveAllLinkToParams();
	}
}
//----------------------------------------------------------------------------
void BPModule::UpdateCurve()
{
	int outputSize = (int)mOutputParamsVec.size();
	for (int i = 0; i < outputSize; i++)
	{
		mOutputParamsVec[i]->UpdateCurve();
	}

	if (mActOutBut)
	{
		mActOutBut->UpdateCurve();
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void BPModule::RegistProperties()
{
	if (MT_FUNCTION_START == mModuleType ||
		MT_PARAM == mModuleType)
	{
		SetNamePropChangeable(true);
	}
	else
	{
		SetNamePropChangeable(false);
	}

	Object::RegistProperties();

	AddPropertyClass("BPModule");

	std::vector<std::string> moduleTypes;
	moduleTypes.push_back("MT_EVENT");
	moduleTypes.push_back("MT_PROCESS");
	AddPropertyEnum("ModuleType", (int)GetModuleType(), moduleTypes, false);
	AddProperty("IsEnable", Object::PT_BOOL, IsEnable());
	AddProperty("NumInputs", Object::PT_INT, GetNumInputs(), false);
	AddProperty("NumOutputs", Object::PT_INT, GetNumOutputs(), false);
}
//----------------------------------------------------------------------------
void BPModule::OnPropertyChanged(const PropertyObject &obj)
{
	Object::OnPropertyChanged(obj);

	if ("Name" == obj.Name)
	{
		std::string nameStr = PX2_ANY_AS(obj.Data, std::string);
		mNameText->GetText()->SetText(nameStr);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
BPModule::BPModule(LoadConstructor value) :
UIFrame(value),
mOwnObjectParam(0),
mIsCompiled(false),
mIsCompilingAsParam_DoNotNeedCall(false)
{
	mItemHeight = 16.0f;
	mInOutStartPos = -mItemHeight*2.0f;
}
//----------------------------------------------------------------------------
void BPModule::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadEnum(mModuleType);

	int inputSize = 0;
	source.Read(inputSize);
	mInputParamsVec.resize(inputSize);
	for (int i = 0; i < inputSize; i++)
	{
		source.ReadPointer(mInputParamsVec[i]);
	}

	int outputParamSize = 0;
	source.Read(outputParamSize);
	mOutputParamsVec.resize(outputParamSize);
	for (int i = 0; i < outputParamSize; i++)
	{
		source.ReadPointer(mOutputParamsVec[i]);
	}

	source.Read(mItemHeight);
	source.ReadPointer(mClassText);
	source.ReadPointer(mNameText);

	source.ReadPointer(mActInBut);
	source.ReadPointer(mActOutBut);
	source.Read(mInOutStartPos);

	PX2_END_DEBUG_STREAM_LOAD(BPModule, source);
}
//----------------------------------------------------------------------------
void BPModule::Link(InStream& source)
{
	UIFrame::Link(source);

	int inputSize = (int)mInputParamsVec.size();
	for (int i = 0; i < inputSize; i++)
	{
		if (mInputParamsVec[i])
		{
			source.ResolveLink(mInputParamsVec[i]);
		}
	}

	int outputSize = (int)mOutputParamsVec.size();
	for (int i = 0; i < outputSize; i++)
	{
		if (mOutputParamsVec[i])
		{
			source.ResolveLink(mOutputParamsVec[i]);
		}
	}

	if (mClassText)
		source.ResolveLink(mClassText);

	if (mNameText)
		source.ResolveLink(mNameText);

	if (mActInBut)
	{
		source.ResolveLink(mActInBut);
	}

	if (mActOutBut)
	{
		source.ResolveLink(mActOutBut);
	}
}
//----------------------------------------------------------------------------
void BPModule::PostLink()
{
	UIFrame::PostLink();

	int inputSize = (int)mInputParamsVec.size();
	for (int i = 0; i < inputSize; i++)
	{
		if (mInputParamsVec[i])
		{
			mInputParamsVec[i]->PostLink();
			mInputParamsVec[i]->SetModule(this);

			mInputParams[mInputParamsVec[i]->GetName()] = mInputParamsVec[i];

			if (FPT_POINTER_THIS == mInputParamsVec[i]->GetDataType() ||
				FPT_POINTER_THIS_STATIC == mInputParamsVec[i]->GetDataType())
			{
				mOwnObjectParam = mInputParamsVec[i];
			}
		}
	}

	int outputSize = (int)mOutputParamsVec.size();
	for (int i = 0; i < outputSize; i++)
	{
		if (mOutputParamsVec[i])
		{
			mOutputParamsVec[i]->PostLink();
			mOutputParamsVec[i]->SetModule(this);

			mOutputParams[mOutputParamsVec[i]->GetName()] = mOutputParamsVec[i];
		}
	}

	if (mActInBut)
	{
		mActInBut->SetModule(this);
		mActInBut->PostLink();
	}

	if (mActOutBut)
	{
		mActOutBut->SetModule(this);
		mActOutBut->PostLink();
	}

	_UpdateModuleColorPos();
}
//----------------------------------------------------------------------------
bool BPModule::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		int inputSize = (int)mInputParamsVec.size();
		for (int i = 0; i < inputSize; i++)
		{
			target.Register(mInputParamsVec[i]);
		}

		int outputSize = (int)mOutputParamsVec.size();
		for (int i = 0; i < outputSize; i++)
		{
			target.Register(mOutputParamsVec[i]);
		}

		if (mClassText)
			target.Register(mClassText);

		if (mNameText)
			target.Register(mNameText);

		if (mActInBut)
			target.Register(mActInBut);

		if (mActOutBut)
			target.Register(mActOutBut);

		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void BPModule::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(mModuleType);

	int inputSize = (int)mInputParamsVec.size();
	target.Write(inputSize);
	for (int i = 0; i < inputSize; i++)
	{
		target.WritePointer(mInputParamsVec[i]);
	}

	int outputSize = (int)mOutputParamsVec.size();
	target.Write(outputSize);
	for (int i = 0; i < outputSize; i++)
	{
		target.WritePointer(mOutputParamsVec[i]);
	}

	target.Write(mItemHeight);
	target.WritePointer(mClassText);
	target.WritePointer(mNameText);

	target.WritePointer(mActInBut);
	target.WritePointer(mActOutBut);
	target.Write(mInOutStartPos);

	PX2_END_DEBUG_STREAM_SAVE(BPModule, target);
}
//----------------------------------------------------------------------------
int BPModule::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_ENUMSIZE(mModuleType);

	int inputSize = (int)mInputParamsVec.size();
	size += sizeof(inputSize);
	if (inputSize > 0)
		size += inputSize * PX2_POINTERSIZE(mInputParamsVec[0]);

	int outputSize = (int)mOutputParamsVec.size();
	size += sizeof(outputSize);
	if (outputSize > 0)
		size += outputSize * PX2_POINTERSIZE(mOutputParamsVec[0]);

	size += sizeof(mItemHeight);
	size += PX2_POINTERSIZE(mClassText);
	size += PX2_POINTERSIZE(mNameText);

	size += PX2_POINTERSIZE(mActInBut);
	size += PX2_POINTERSIZE(mActOutBut);
	size += sizeof(mInOutStartPos);

	return size;
}
//----------------------------------------------------------------------------