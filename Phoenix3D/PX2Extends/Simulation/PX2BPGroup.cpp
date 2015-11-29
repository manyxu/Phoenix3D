// PX2BPGroup.cpp

#include "PX2BPGroup.hpp"
#include "PX2BPFile.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, BPGroup);
PX2_IMPLEMENT_STREAM(BPGroup);
PX2_IMPLEMENT_FACTORY(BPGroup);

//----------------------------------------------------------------------------
BPGroup::BPGroup()
{
	SetName("BPGroup");

	mSize = Sizef(1136.0f, 1000.0f);

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
	mTipText->SetColor(Float3::BLUE);
	mTipText->SetText("Use BPGroup to group logics.");

	SetSize(mSize);
}
//----------------------------------------------------------------------------
BPGroup::~BPGroup()
{
}
//----------------------------------------------------------------------------
void BPGroup::Enable(bool enable)
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
void BPGroup::SetTip(const std::string &tip)
{
	mTip = tip;
	mTipText->SetText(tip);
}
//----------------------------------------------------------------------------
void BPGroup::SetSize(const Sizef &size)
{
	mSize = size;

	if (mBackPicBox)
		mBackPicBox->SetSize(mSize);

	if (mNameText)
		mNameText->LocalTransform.SetTranslateZ(4.0f);

	if (mTipText)
	{
		mTipText->SetRectUseage(UIText::RU_ALIGNS);
		mTipText->SetAligns(TEXTALIGN_RIGHT | TEXTALIGN_BOTTOM);
		mTipText->SetRect(Rectf(0.0f, 0.0f, mSize.Width, 20.0f));
		mTipText->LocalTransform.SetTranslateZ(4.0f);
	}
}
//----------------------------------------------------------------------------
int BPGroup::AttachChild(Movable* child)
{
	int ret = UIFrame::AttachChild(child);

	if (-1 != ret)
	{
		BPGroup *logicPackage = DynamicCast<BPGroup>(child);
		if (logicPackage)
		{
			mLogicGroups.push_back(logicPackage);
		}

		BPFile *logicFile = DynamicCast<BPFile>(child);
		if (logicFile)
		{
			mLogicFiles.push_back(logicFile);
		}
	}

	return ret;
}
//----------------------------------------------------------------------------
int BPGroup::DetachChild(Movable* child)
{
	int ret = UIFrame::DetachChild(child);

	if (-1 != ret)
	{
		BPGroup *logicGroup = DynamicCast<BPGroup>(child);
		if (logicGroup)
		{
			std::vector<Pointer0<BPGroup> >::iterator itGroup = mLogicGroups.begin();
			for (; itGroup != mLogicGroups.end(); itGroup++)
			{
				if (*itGroup == logicGroup)
				{
					mLogicGroups.erase(itGroup);
					break;
				}
			}
		}

		BPFile *logicFile = DynamicCast<BPFile>(child);
		if (logicFile)
		{
			std::vector<Pointer0<BPFile> >::iterator itFile = mLogicFiles.begin();
			for (; itFile != mLogicFiles.end(); itFile++)
			{
				if (*itFile == logicFile)
				{
					mLogicFiles.erase(itFile);
					break;
				}
			}
		}
	}

	return ret;
}
//----------------------------------------------------------------------------
bool BPGroup::IsHasLogicGroup(BPGroup *logicPackage)
{
	std::vector<Pointer0<BPGroup> >::iterator it = mLogicGroups.begin();
	for (; it != mLogicGroups.end(); it++)
	{
		if (*it == logicPackage)
		{
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
bool BPGroup::IsHasFile(BPFile *logicFile)
{
	std::vector<Pointer0<BPFile> >::iterator it = mLogicFiles.begin();
	for (; it != mLogicFiles.end(); it++)
	{
		if (*it == logicFile)
		{
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void BPGroup::Compile(std::string &script)
{
	for (int i = 0; i < (int)mLogicGroups.size(); i++)
	{
		mLogicGroups[i]->Compile(script);
	}

	for (int i = 0; i < (int)mLogicFiles.size(); i++)
	{
		mLogicFiles[i]->Compile();
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 名称支持
//----------------------------------------------------------------------------
Object* BPGroup::GetObjectByName(const std::string& name)
{
	Object* found = UIFrame::GetObjectByName(name);
	if (found)
	{
		return found;
	}

	return 0;
}
//----------------------------------------------------------------------------
void BPGroup::GetAllObjectsByName(const std::string& name,
	std::vector<Object*>& objects)
{
	UIFrame::GetAllObjectsByName(name, objects);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void BPGroup::RegistProperties()
{
	Movable::RegistProperties();

	AddPropertyClass("BPGroup");

	AddProperty("Tip", Object::PT_STRING, mTipText->GetText());
	AddProperty("Size", Object::PT_SIZE, mSize);

	AddProperty("IsEanble", PT_BOOL, IsEnable());

	AddProperty("NumLogicGroups", Object::PT_INT, GetNumLogicGroups(), false);
}
//----------------------------------------------------------------------------
void BPGroup::OnPropertyChanged(const PropertyObject &obj)
{
	Movable::OnPropertyChanged(obj);

	if ("Name" == obj.Name)
	{
		mNameText->SetText(PX2_ANY_AS(obj.Data, std::string));
		SetSize(mSize);
	}
	else if ("Tip" == obj.Name)
	{
		SetTip(PX2_ANY_AS(obj.Data, std::string));
	}
	else if ("Size" == obj.Name)
	{
		SetSize(PX2_ANY_AS(obj.Data, Sizef));
	}
	else if ("IsEanble" == obj.Name)
	{
		Enable(PX2_ANY_AS(obj.Data, bool));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
BPGroup::BPGroup(LoadConstructor value)
:
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void BPGroup::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadString(mTip);

	int lgSize = 0;
	source.Read(lgSize);
	if (lgSize > 0)
	{
		mLogicGroups.resize(lgSize);
		source.ReadPointerVV(lgSize, &mLogicGroups[0]);
	}

	int lfSize = 0;
	source.Read(lfSize);
	if (lfSize > 0)
	{
		mLogicFiles.resize(lfSize);
		source.ReadPointerVV(lfSize, &mLogicFiles[0]);
	}

	source.ReadAggregate(mSize);
	source.ReadPointer(mBackPicBox);
	source.ReadPointer(mNameText);
	source.ReadPointer(mTipText);

	source.ReadBool(mIsEnable);

	PX2_END_DEBUG_STREAM_LOAD(BPGroup, source);
}
//----------------------------------------------------------------------------
void BPGroup::Link(InStream& source)
{
	UIFrame::Link(source);

	int numLgs = (int)mLogicGroups.size();
	for (int i = 0; i < numLgs; ++i)
	{
		if (mLogicGroups[i])
		{
			source.ResolveLink(mLogicGroups[i]);
		}
	}

	int numLfs = (int)mLogicFiles.size();
	for (int i = 0; i < numLfs; ++i)
	{
		if (mLogicFiles[i])
		{
			source.ResolveLink(mLogicFiles[i]);
		}
	}

	source.ResolveLink(mBackPicBox);
	source.ResolveLink(mNameText);
	source.ResolveLink(mTipText);
}
//----------------------------------------------------------------------------
void BPGroup::PostLink()
{
	UIFrame::PostLink();

	int numLgs = (int)mLogicGroups.size();
	for (int i = 0; i < numLgs; ++i)
	{
		if (mLogicGroups[i])
		{
			mLogicGroups[i]->PostLink();
		}
	}

	int numLfs = (int)mLogicFiles.size();
	for (int i = 0; i < numLfs; ++i)
	{
		if (mLogicFiles[i])
		{
			mLogicFiles[i]->PostLink();
		}
	}

	if (mBackPicBox)
		mBackPicBox->PostLink();

	if (mNameText)
		mNameText->PostLink();

	if (mTipText)
		mTipText->PostLink();
}
//----------------------------------------------------------------------------
bool BPGroup::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		int numLgs = (int)mLogicGroups.size();
		for (int i = 0; i < numLgs; ++i)
		{
			if (mLogicGroups[i])
			{
				target.Register(mLogicGroups[i]);
			}
		}

		int numLfs = (int)mLogicFiles.size();
		for (int i = 0; i < numLfs; ++i)
		{
			if (mLogicFiles[i])
			{
				target.Register(mLogicFiles[i]);
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
void BPGroup::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteString(mTip);

	int numLgs = (int)mLogicGroups.size();
	target.Write(numLgs);
	for (int i = 0; i < numLgs; ++i)
	{
		if (mLogicGroups[i])
		{
			target.WritePointer(mLogicGroups[i]);
		}
		else
		{
			target.WritePointer((BPGroup*)0);
		}
	}

	int numLfs = (int)mLogicFiles.size();
	target.Write(numLfs);
	for (int i = 0; i < numLfs; ++i)
	{
		if (mLogicFiles[i])
		{
			target.WritePointer(mLogicFiles[i]);
		}
		else
		{
			target.WritePointer((BPFile*)0);
		}
	}

	target.WriteAggregate(mSize);
	target.WritePointer(mBackPicBox);
	target.WritePointer(mNameText);
	target.WritePointer(mTipText);

	target.WriteBool(mIsEnable);

	PX2_END_DEBUG_STREAM_SAVE(BPGroup, target);
}
//----------------------------------------------------------------------------
int BPGroup::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_STRINGSIZE(mTip);

	int numLgs = (int)mLogicGroups.size();
	size += sizeof(numLgs);
	if (numLgs > 0)
	{
		size += numLgs*PX2_POINTERSIZE(mLogicGroups[0]);
	}

	int numLfs = (int)mLogicFiles.size();
	size += sizeof(numLfs);
	if (numLfs > 0)
	{
		size += numLfs*PX2_POINTERSIZE(mLogicFiles[0]);
	}

	size += sizeof(mSize);
	size += PX2_POINTERSIZE(mBackPicBox);
	size += PX2_POINTERSIZE(mNameText);
	size += PX2_POINTERSIZE(mTipText);

	size += PX2_BOOLSIZE(mIsEnable);

	return size;
}
//----------------------------------------------------------------------------