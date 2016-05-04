// PX2BPFile.cpp

#include "PX2BPFile.hpp"
#include "PX2BPManager.hpp"
#include "PX2FontManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, BPFile);
PX2_IMPLEMENT_STREAM(BPFile);
PX2_IMPLEMENT_FACTORY(BPFile);

//----------------------------------------------------------------------------
BPFile::BPFile()
{
	EnableAnchorLayout(false);
	SetPivot(0.0f, 1.0f);

	SetName("BPFile");

	mIsEnable = true;

	mNameText = new0 UIFText();
	AttachChild(mNameText);
	mNameText->LocalTransform.SetTranslateY(-1.0f);
	mNameText->GetText()->SetFontScale(0.65f);
	mNameText->GetText()->SetColor(Float3::WHITE);
	mNameText->GetText()->SetText(GetName());
	mNameText->GetText()->SetAligns(TEXTALIGN_LEFT);
	mNameText->SetAnchorHor(0.0f, 1.0f);
	mNameText->SetAnchorVer(1.0f, 1.0f);
	mNameText->SetAnchorParamVer(5.0f, 0.0f);

	mTipText = new0 UIFText();
	AttachChild(mTipText);
	mTipText->LocalTransform.SetTranslateY(-1.0f);
	mTipText->GetText()->SetFontScale(0.5f);
	mTipText->GetText()->SetColor(Float3::YELLOW);
	mTipText->GetText()->SetText("BPFile contains functions.");
	mTipText->GetText()->SetAligns(TEXTALIGN_RIGHT);
	mTipText->SetAnchorHor(0.5f, 1.0f);
	mTipText->SetAnchorVer(1.0f, 1.0f);
	mTipText->SetAnchorParamVer(5.0f, 0.0f);

	BPModulePtr startModule = new0 BPModule(BPModule::MT_FUNCTION_START);
	startModule->RegistFunObj(*PX2_BPM.GetStartGF());
	startModule->LocalTransform.SetTranslate(APoint(0.0f, -1.0f, 0.0f));
	AttachChild(startModule);

	mSize = Sizef(1000.0f, 400.0f);
	SetSize(mSize);

	UIPicBox *backPicBox = CreateAddBackgroundPicBox();
	backPicBox->SetAlpha(0.5f);
}
//----------------------------------------------------------------------------
BPFile::~BPFile()
{
}
//----------------------------------------------------------------------------
void BPFile::Enable(bool enable)
{
	mIsEnable = enable;

	if (mBackgroundPicBox)
	{
		if (mIsEnable)
		{
			mBackgroundPicBox->SetColor(Float3::WHITE);
		}
		else
		{
			mBackgroundPicBox->SetColor(Float3(0.3f, 0.3f, 0.3f));
		}
	}
}
//----------------------------------------------------------------------------
void BPFile::SetTip(const std::string &tip)
{
	mTip = tip;
	mTipText->GetText()->SetText(tip);
}
//----------------------------------------------------------------------------
void BPFile::OnSizeChanged()
{
	UIFrame::OnSizeChanged();

	_UpdateModuleColorPos();
}
//----------------------------------------------------------------------------
void BPFile::OnChildAttached(Movable *child)
{
	UIFrame::OnChildAttached(child);

	BPModule *logicModule = DynamicCast<BPModule>(child);
	if (logicModule)
	{
		mBPModules.push_back(logicModule);
	}
}
//----------------------------------------------------------------------------
void BPFile::OnChildDetach(Movable *child)
{
	UIFrame::OnChildDetach(child);

	BPModule *logicModule = DynamicCast<BPModule>(child);
	if (logicModule)
	{
		std::vector<BPModulePtr>::iterator it = mBPModules.begin();
		for (; it != mBPModules.end(); it++)
		{
			if (*it == logicModule)
			{
				logicModule->OnDetach();
				mBPModules.erase(it);
				break;
			}
		}
	}
}
//----------------------------------------------------------------------------
bool BPFile::IsHasBPModule(BPModule *logicModule)
{
	std::vector<BPModulePtr>::iterator it = mBPModules.begin();
	for (; it != mBPModules.end(); it++)
	{
		if (*it == logicModule)
		{
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void BPFile::UpdateWorldData(double applicationTime, 
	double elapsedTime)
{
	UIFrame::UpdateWorldData(applicationTime, elapsedTime);

	for (int i = 0; i < (int)mBPModules.size(); i++)
	{
		mBPModules[i]->UpdateCurve();
	}
}
//----------------------------------------------------------------------------
void BPFile::_PreCompileOne()
{
	for (int i = 0; i < (int)mBPModules.size(); i++)
	{
		BPModule::ModuleType mt = mBPModules[i]->GetModuleType();
		if (BPModule::MT_EVENT != mt && BPModule::MT_PARAM != mt)
		{
			mBPModules[i]->PrepareCompile();
		}
	}
}
//----------------------------------------------------------------------------
const std::string &BPFile::Compile()
{
	mCompiledString.clear();

	mCompiledString += "\n";

	// 变量定义
	for (int i = 0; i < (int)mBPModules.size(); i++)
	{
		BPModule::ModuleType moduleType = mBPModules[i]->GetModuleType();

		if (BPModule::MT_PARAM == moduleType)
		{
			mBPModules[i]->PrepareCompile();
			mBPModules[i]->Compile(mCompiledString, 0, true);
			mCompiledString += "\n";
		}
	}

	// 处理函数
	for (int i = 0; i < (int)mBPModules.size(); i++)
	{
		BPModule::ModuleType moduleType = mBPModules[i]->GetModuleType();

		if (BPModule::MT_FUNCTION_START == moduleType)
		{
			_PreCompileOne();

			mBPModules[i]->FunStartOnPreCompile(mCompiledString);

			mBPModules[i]->Compile(mCompiledString, 0, true);

			mBPModules[i]->FunEndOnAfterCompile(mCompiledString);
		}
	}

	mCompiledString += "\n";

	// 调用事件
	for (int i = 0; i < (int)mBPModules.size(); i++)
	{
		BPModule::ModuleType moduleType = mBPModules[i]->GetModuleType();

		if (BPModule::MT_EVENT == moduleType)
		{
			mBPModules[i]->PrepareCompile();
			mBPModules[i]->Compile(mCompiledString, 0, true);
		}
	}

	return mCompiledString;
}
//----------------------------------------------------------------------------
void BPFile::_UpdateModuleColorPos()
{
	mTipText->SetColor(Float3::YELLOW);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 名称支持
//----------------------------------------------------------------------------
Object* BPFile::GetObjectByName(const std::string& name)
{
	Object* found = UIFrame::GetObjectByName(name);
	if (found)
	{
		return found;
	}

	return 0;
}
//----------------------------------------------------------------------------
void BPFile::GetAllObjectsByName(const std::string& name,
	std::vector<Object*>& objects)
{
	UIFrame::GetAllObjectsByName(name, objects);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void BPFile::RegistProperties()
{
	Movable::RegistProperties();

	SetNamePropChangeable(true);

	AddPropertyClass("BPFile");

	AddProperty("Tip", Object::PT_STRING, mTipText->GetText());

	AddProperty("IsEnable", PT_BOOL, IsEnable());

	AddProperty("NumBPModules", Object::PT_INT, GetNumBPModules(), false);
}
//----------------------------------------------------------------------------
void BPFile::OnPropertyChanged(const PropertyObject &obj)
{
	Movable::OnPropertyChanged(obj);

	if ("Name" == obj.Name)
	{
		mNameText->GetText()->SetText(PX2_ANY_AS(obj.Data, std::string));
		_UpdateModuleColorPos();
	}
	else if ("IsEnable" == obj.Name)
	{
		Enable(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("Tip" == obj.Name)
	{
		SetTip(PX2_ANY_AS(obj.Data, std::string));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
BPFile::BPFile(LoadConstructor value) :
UIFrame(value),
mIsEnable(true)
{
}
//----------------------------------------------------------------------------
void BPFile::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadString(mTip);

	int lmSize = 0;
	source.Read(lmSize);
	if (lmSize > 0)
	{
		mBPModules.resize(lmSize);
		source.ReadPointerVV(lmSize, &mBPModules[0]);
	}

	source.ReadPointer(mNameText);
	source.ReadPointer(mTipText);

	source.ReadBool(mIsEnable);

	PX2_END_DEBUG_STREAM_LOAD(BPFile, source);
}
//----------------------------------------------------------------------------
void BPFile::Link(InStream& source)
{
	UIFrame::Link(source);

	int numLms = (int)mBPModules.size();
	for (int i = 0; i < numLms; ++i)
	{
		if (mBPModules[i])
		{
			source.ResolveLink(mBPModules[i]);
		}
	}

	source.ResolveLink(mNameText);
	source.ResolveLink(mTipText);
}
//----------------------------------------------------------------------------
void BPFile::PostLink()
{
	UIFrame::PostLink();

	int numLms = (int)mBPModules.size();
	for (int i = 0; i < numLms; ++i)
	{
		if (mBPModules[i])
		{
			mBPModules[i]->PostLink();
		}
	}

	if (mNameText)
		mNameText->PostLink();

	if (mTipText)
		mTipText->PostLink();

	_UpdateModuleColorPos();
}
//----------------------------------------------------------------------------
bool BPFile::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		int numLms = (int)mBPModules.size();
		for (int i = 0; i < numLms; ++i)
		{
			if (mBPModules[i])
			{
				target.Register(mBPModules[i]);
			}
		}

		target.Register(mNameText);
		target.Register(mTipText);

		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void BPFile::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteString(mTip);

	int numLms = (int)mBPModules.size();
	target.Write(numLms);
	for (int i = 0; i < numLms; ++i)
	{
		if (mBPModules[i])
		{
			target.WritePointer(mBPModules[i]);
		}
		else
		{
			target.WritePointer((BPModule*)0);
		}
	}

	target.WritePointer(mNameText);
	target.WritePointer(mTipText);

	target.WriteBool(mIsEnable);

	PX2_END_DEBUG_STREAM_SAVE(BPFile, target);
}
//----------------------------------------------------------------------------
int BPFile::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_STRINGSIZE(mTip);

	int numLms = (int)mBPModules.size();
	size += sizeof(numLms);
	if (numLms > 0)
	{
		size += numLms*PX2_POINTERSIZE(mBPModules[0]);
	}

	size += PX2_POINTERSIZE(mNameText);
	size += PX2_POINTERSIZE(mTipText);

	size += PX2_BOOLSIZE(mIsEnable);

	return size;
}
//----------------------------------------------------------------------------