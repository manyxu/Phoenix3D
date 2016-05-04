// PX2BPPackage.cpp

#include "PX2BPPackage.hpp"
#include "PX2BPFile.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, BPPackage);
PX2_IMPLEMENT_STREAM(BPPackage);
PX2_IMPLEMENT_FACTORY(BPPackage);

//----------------------------------------------------------------------------
BPPackage::BPPackage()
{
	EnableAnchorLayout(false);
	SetPivot(0.0f, 1.0f);

	SetName("BPPackage");

	mNameText = new0 UIFText();
	AttachChild(mNameText);
	mNameText->LocalTransform.SetTranslateY(-1.0f);
	mNameText->GetText()->SetFontScale(0.65f);
	mNameText->GetText()->SetColor(Float3::WHITE);
	mNameText->GetText()->SetAligns(TEXTALIGN_LEFT);
	mNameText->GetText()->SetText(GetName());
	mNameText->SetAnchorHor(0.0f, 0.5f);
	mNameText->SetAnchorVer(1.0f, 1.0f);
	mNameText->SetAnchorParamVer(5.0f, 0.0f);

	mTipText = new0 UIFText();
	AttachChild(mTipText);
	mTipText->LocalTransform.SetTranslateY(-1.0f);
	mTipText->GetText()->SetFontScale(0.5f);
	mTipText->GetText()->SetColor(Float3::BLUE);
	mTipText->GetText()->SetAligns(TEXTALIGN_RIGHT);
	mTipText->GetText()->SetText("Use BPPackage to group logics.");
	mTipText->SetAnchorHor(0.5f, 1.0f);
	mTipText->SetAnchorVer(1.0f, 1.0f);
	mTipText->SetAnchorParamVer(5.0f, 0.0f);

	mSize = Sizef(1000.0f, 600.0f);
	SetSize(mSize);

	UIPicBox *backPicBox = CreateAddBackgroundPicBox();
	backPicBox->SetAlpha(0.5f);
}
//----------------------------------------------------------------------------
BPPackage::~BPPackage()
{
}
//----------------------------------------------------------------------------
void BPPackage::Enable(bool enable)
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
void BPPackage::SetTip(const std::string &tip)
{
	mTip = tip;
	mTipText->GetText()->SetText(tip);
}
//----------------------------------------------------------------------------
void BPPackage::OnChildAttached(Movable *child)
{
	UIFrame::OnChildAttached(child);

	BPPackage *logicPackage = DynamicCast<BPPackage>(child);
	if (logicPackage)
	{
		mBPPackages.push_back(logicPackage);
	}

	BPFile *logicFile = DynamicCast<BPFile>(child);
	if (logicFile)
	{
		mBPFiles.push_back(logicFile);
	}
}
//----------------------------------------------------------------------------
void BPPackage::OnChildDetach(Movable *child)
{
	UIFrame::OnChildDetach(child);

	BPPackage *logicGroup = DynamicCast<BPPackage>(child);
	if (logicGroup)
	{
		std::vector<PointerRef<BPPackage> >::iterator itGroup = mBPPackages.begin();
		for (; itGroup != mBPPackages.end(); itGroup++)
		{
			if (*itGroup == logicGroup)
			{
				mBPPackages.erase(itGroup);
				break;
			}
		}
	}

	BPFile *logicFile = DynamicCast<BPFile>(child);
	if (logicFile)
	{
		std::vector<PointerRef<BPFile> >::iterator itFile = mBPFiles.begin();
		for (; itFile != mBPFiles.end(); itFile++)
		{
			if (*itFile == logicFile)
			{
				mBPFiles.erase(itFile);
				break;
			}
		}
	}
}
//----------------------------------------------------------------------------
bool BPPackage::IsHasBPPackage(BPPackage *logicPackage)
{
	std::vector<PointerRef<BPPackage> >::iterator it = mBPPackages.begin();
	for (; it != mBPPackages.end(); it++)
	{
		if (*it == logicPackage)
		{
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
bool BPPackage::IsHasFile(BPFile *logicFile)
{
	std::vector<PointerRef<BPFile> >::iterator it = mBPFiles.begin();
	for (; it != mBPFiles.end(); it++)
	{
		if (*it == logicFile)
		{
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void BPPackage::Compile(std::string &script)
{
	for (int i = 0; i < (int)mBPPackages.size(); i++)
	{
		mBPPackages[i]->Compile(script);
	}

	for (int i = 0; i < (int)mBPFiles.size(); i++)
	{
		mBPFiles[i]->Compile();
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 名称支持
//----------------------------------------------------------------------------
Object* BPPackage::GetObjectByName(const std::string& name)
{
	Object* found = UIFrame::GetObjectByName(name);
	if (found)
	{
		return found;
	}

	return 0;
}
//----------------------------------------------------------------------------
void BPPackage::GetAllObjectsByName(const std::string& name,
	std::vector<Object*>& objects)
{
	UIFrame::GetAllObjectsByName(name, objects);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void BPPackage::RegistProperties()
{
	Movable::RegistProperties();

	AddPropertyClass("BPPackage");

	AddProperty("Tip", Object::PT_STRING, mTipText->GetText());

	AddProperty("IsEanble", PT_BOOL, IsEnable());

	AddProperty("NumBPPackages", Object::PT_INT, GetNumBPPackages(), false);
}
//----------------------------------------------------------------------------
void BPPackage::OnPropertyChanged(const PropertyObject &obj)
{
	Movable::OnPropertyChanged(obj);

	if ("Name" == obj.Name)
	{
		mNameText->GetText()->SetText(PX2_ANY_AS(obj.Data, std::string));
		SetSize(mSize);
	}
	else if ("Tip" == obj.Name)
	{
		SetTip(PX2_ANY_AS(obj.Data, std::string));
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
BPPackage::BPPackage(LoadConstructor value)
:
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void BPPackage::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadString(mTip);

	int lgSize = 0;
	source.Read(lgSize);
	if (lgSize > 0)
	{
		mBPPackages.resize(lgSize);
		source.ReadPointerVV(lgSize, &mBPPackages[0]);
	}

	int lfSize = 0;
	source.Read(lfSize);
	if (lfSize > 0)
	{
		mBPFiles.resize(lfSize);
		source.ReadPointerVV(lfSize, &mBPFiles[0]);
	}

	source.ReadPointer(mNameText);
	source.ReadPointer(mTipText);

	source.ReadBool(mIsEnable);

	PX2_END_DEBUG_STREAM_LOAD(BPPackage, source);
}
//----------------------------------------------------------------------------
void BPPackage::Link(InStream& source)
{
	UIFrame::Link(source);

	int numLgs = (int)mBPPackages.size();
	for (int i = 0; i < numLgs; ++i)
	{
		if (mBPPackages[i])
		{
			source.ResolveLink(mBPPackages[i]);
		}
	}

	int numLfs = (int)mBPFiles.size();
	for (int i = 0; i < numLfs; ++i)
	{
		if (mBPFiles[i])
		{
			source.ResolveLink(mBPFiles[i]);
		}
	}

	source.ResolveLink(mNameText);
	source.ResolveLink(mTipText);
}
//----------------------------------------------------------------------------
void BPPackage::PostLink()
{
	UIFrame::PostLink();

	int numLgs = (int)mBPPackages.size();
	for (int i = 0; i < numLgs; ++i)
	{
		if (mBPPackages[i])
		{
			mBPPackages[i]->PostLink();
		}
	}

	int numLfs = (int)mBPFiles.size();
	for (int i = 0; i < numLfs; ++i)
	{
		if (mBPFiles[i])
		{
			mBPFiles[i]->PostLink();
		}
	}

	if (mNameText)
		mNameText->PostLink();

	if (mTipText)
		mTipText->PostLink();
}
//----------------------------------------------------------------------------
bool BPPackage::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		int numLgs = (int)mBPPackages.size();
		for (int i = 0; i < numLgs; ++i)
		{
			if (mBPPackages[i])
			{
				target.Register(mBPPackages[i]);
			}
		}

		int numLfs = (int)mBPFiles.size();
		for (int i = 0; i < numLfs; ++i)
		{
			if (mBPFiles[i])
			{
				target.Register(mBPFiles[i]);
			}
		}

		target.Register(mNameText);
		target.Register(mTipText);

		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void BPPackage::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteString(mTip);

	int numLgs = (int)mBPPackages.size();
	target.Write(numLgs);
	for (int i = 0; i < numLgs; ++i)
	{
		if (mBPPackages[i])
		{
			target.WritePointer(mBPPackages[i]);
		}
		else
		{
			target.WritePointer((BPPackage*)0);
		}
	}

	int numLfs = (int)mBPFiles.size();
	target.Write(numLfs);
	for (int i = 0; i < numLfs; ++i)
	{
		if (mBPFiles[i])
		{
			target.WritePointer(mBPFiles[i]);
		}
		else
		{
			target.WritePointer((BPFile*)0);
		}
	}

	target.WritePointer(mNameText);
	target.WritePointer(mTipText);

	target.WriteBool(mIsEnable);

	PX2_END_DEBUG_STREAM_SAVE(BPPackage, target);
}
//----------------------------------------------------------------------------
int BPPackage::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_STRINGSIZE(mTip);

	int numLgs = (int)mBPPackages.size();
	size += sizeof(numLgs);
	if (numLgs > 0)
	{
		size += numLgs*PX2_POINTERSIZE(mBPPackages[0]);
	}

	int numLfs = (int)mBPFiles.size();
	size += sizeof(numLfs);
	if (numLfs > 0)
	{
		size += numLfs*PX2_POINTERSIZE(mBPFiles[0]);
	}

	size += PX2_POINTERSIZE(mNameText);
	size += PX2_POINTERSIZE(mTipText);

	size += PX2_BOOLSIZE(mIsEnable);

	return size;
}
//----------------------------------------------------------------------------