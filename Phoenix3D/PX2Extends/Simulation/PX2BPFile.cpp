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
	SetName("BPFile");

	mIsEnable = true;

	mSize = Sizef(1024.0f, 400.0f);

	mBackPicBox = new0 UIPicBox();
	AttachChild(mBackPicBox);
	mBackPicBox->SetPivot(0.0f, 1.0f);
	mBackPicBox->SetPicBoxType(UIPicBox::PBT_NINE);
	mBackPicBox->SetTexCornerSize(10.0f, 10.0f);
	mBackPicBox->SetTexture("Data/engine/white.png");

	mNameText = new0 UIText();
	AttachChild(mNameText);
	mNameText->SetFontScale(0.65f);
	mNameText->SetColor(Float3::WHITE);
	mNameText->SetText(GetName());

	mTipText = new0 UIText();
	AttachChild(mTipText);
	mTipText->SetFontScale(0.5f);
	mTipText->SetColor(Float3::YELLOW);
	mTipText->SetText("BPFile contains functions.");

	BPModulePtr startModule = new0 BPModule(BPModule::MT_FUNCTION_START);
	startModule->RegistFunObj(*PX2_BPM.GetStartGF());
	startModule->LocalTransform.SetTranslate(APoint(5.0f, -1.0f, -5.0f));
	AttachChild(startModule);

	SetSize(mSize);
}
//----------------------------------------------------------------------------
BPFile::~BPFile()
{
}
//----------------------------------------------------------------------------
void BPFile::Enable(bool enable)
{
	mIsEnable = enable;

	if (mBackPicBox)
	{
		if (mIsEnable)
		{
			mBackPicBox->SetColor(Float3::WHITE);
		}
		else
		{
			mBackPicBox->SetColor(Float3(0.3f, 0.3f, 0.3f));
		}
	}
}
//----------------------------------------------------------------------------
void BPFile::SetTip(const std::string &tip)
{
	mTip = tip;
	mTipText->SetText(tip);
}
//----------------------------------------------------------------------------
void BPFile::SetSize(const Sizef &size)
{
	mSize = size;

	if (mBackPicBox)
		mBackPicBox->SetSize(mSize);

	if (mNameText)
		mNameText->LocalTransform.SetTranslateZ(4.0f);

	if (mTipText)
		mTipText->LocalTransform.SetTranslateZ(4.0f);

	_UpdateModuleColorPos();
}
//----------------------------------------------------------------------------
int BPFile::AttachChild(Movable* child)
{
	int ret = UIFrame::AttachChild(child);

	if (-1 != ret)
	{
		BPModule *logicModule = DynamicCast<BPModule>(child);
		if (logicModule)
		{
			mBPModules.push_back(logicModule);
		}
	}

	return ret;
}
//----------------------------------------------------------------------------
int BPFile::DetachChild(Movable* child)
{
	int ret = UIFrame::DetachChild(child);

	if (-1 != ret)
	{
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

	return ret;
}
//----------------------------------------------------------------------------
bool BPFile::IsHasLogicModule(BPModule *logicModule)
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

	mCompiledString += "\n\n";

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

	mCompiledString += "\n\n";

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

	float width = 0.0f;
	float height = 0.0f;
	PX2_FM.GetDefaultFont()->GetTextExtent(mNameText->GetText().c_str(), 
		width, height);
	width *= mNameText->GetFontScale();

	if (mNameText)
	{
		mNameText->LocalTransform.SetTranslateZ(4.0f);
	}

	if (mTipText)
	{
		mTipText->SetRectUseage(UIText::RU_NONE);
		mTipText->LocalTransform.SetTranslateX(width + 5.0f);
		mTipText->LocalTransform.SetTranslateZ(4.0f);
	}
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

	AddPropertyClass("BPFile");

	AddProperty("Tip", Object::PT_STRING, mTipText->GetText());
	AddProperty("Size", Object::PT_SIZE, mSize);

	AddProperty("IsEnable", PT_BOOL, IsEnable());

	AddProperty("NumLogicModules", Object::PT_INT, GetNumLogicModules(), false);
}
//----------------------------------------------------------------------------
void BPFile::OnPropertyChanged(const PropertyObject &obj)
{
	Movable::OnPropertyChanged(obj);

	if ("Name" == obj.Name)
	{
		mNameText->SetText(PX2_ANY_AS(obj.Data, std::string));
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
	else if ("Size" == obj.Name)
	{
		SetSize(PX2_ANY_AS(obj.Data, Sizef));
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

	source.ReadAggregate(mSize);
	source.ReadPointer(mBackPicBox);
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

	source.ResolveLink(mBackPicBox);
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

	if (mBackPicBox)
		mBackPicBox->PostLink();

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

		target.Register(mBackPicBox);
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

	target.WriteAggregate(mSize);
	target.WritePointer(mBackPicBox);
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

	size += sizeof(mSize);
	size += PX2_POINTERSIZE(mBackPicBox);
	size += PX2_POINTERSIZE(mNameText);
	size += PX2_POINTERSIZE(mTipText);

	size += PX2_BOOLSIZE(mIsEnable);

	return size;
}
//----------------------------------------------------------------------------