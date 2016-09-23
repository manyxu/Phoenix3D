// PX2UIBlueBlock.cpp

#include "PX2UIBlueBlock.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIBlueblock);
PX2_IMPLEMENT_STREAM(UIBlueblock);
PX2_IMPLEMENT_FACTORY(UIBlueblock);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, UIBlueblock);

//----------------------------------------------------------------------------
UIBlueblock::UIBlueblock() :
mLogicModule(0)
{
	SetColorSelfCtrled(true);

	SetSize(200.0f, 30.0f);

	mUIFText = new0 UIFText();
	AttachChild(mUIFText);
	mUIFText->LocalTransform.SetTranslateY(-1.0f);
	mUIFText->SetAnchorHor(0.0f, 1.0f);
	mUIFText->SetAnchorVer(0.0f, 1.0f);
	mUIFText->SetAnchorParamHor(5.0f, 0.0f);
	mUIFText->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	mUIFText->GetText()->SetFontScale(0.7f);
	mUIFText->GetText()->SetFontColor(Float3::WHITE);
	mUIFText->GetText()->SetDrawStyle(FD_SHADOW);

	mBlockFrame = new0 UIFrame();
	AttachChild(mBlockFrame);
	mBlockFrame->SetAnchorHor(0.0f, 1.0f);
	mBlockFrame->SetAnchorVer(0.0f, 1.0f);

	mBlockButton = new0 UIButton();
	mBlockFrame->AttachChild(mBlockButton);
	mBlockButton->SetAnchorHor(0.0f, 1.0f);
	mBlockButton->SetAnchorVer(0.0f, 1.0f);
	mBlockButton->GetPicBoxAtState(UIButtonBase::BS_NORMAL);
}
//----------------------------------------------------------------------------
UIBlueblock::~UIBlueblock()
{
}
//----------------------------------------------------------------------------
void UIBlueblock::SetActivate(bool act)
{
	UIFrame::SetActivate(act);
}
//----------------------------------------------------------------------------
UIFText *UIBlueblock::GetText()
{
	return mUIFText;
}
//----------------------------------------------------------------------------
void UIBlueblock::SetLogicModule(LogicModule *module)
{
	mLogicModule = module;
}
//----------------------------------------------------------------------------
LogicModule *UIBlueblock::GetLogicModule()
{
	return mLogicModule;
}
//----------------------------------------------------------------------------
void UIBlueblock::SetShape(BlockShapeType type)
{
	mBlockShapeType = type;

	UIPicBox *picBox = mBlockButton->GetPicBoxAtState(UIButtonBase::BS_NORMAL);

	if (BlockShapeType::BST_BARRE == mBlockShapeType)
	{
		picBox->SetTexture("Data/DogGait/images/functionwhite.png");
		picBox->SetPicBoxType(UIPicBox::PicBoxType::PBT_NINE);
		picBox->SetTexCornerSize(Sizef(45.0f, 15.0f), Sizef(12.0f, 12.0f));
		picBox->SetColor(Float3::MakeColor(74, 108, 213));

		SetSize(180.0f, 29.0f);
	}
	else if (BlockShapeType::BST_CAPSULE == mBlockShapeType)
	{
		picBox->SetTexture("Data/DogGait/images/capsule.png");
		picBox->SetPicBoxType(UIPicBox::PicBoxType::PBT_NINE);
		picBox->SetTexCornerSize(Sizef(88.0f, 25.0f), Sizef(13.0f, 23.0f));
		picBox->SetColor(Float3::WHITE);

		SetSize(103.0f, 36.0f);
	}
	else if (BlockShapeType::BST_NAND == mBlockShapeType)
	{
		picBox->SetTexture("Data/DogGait/images/nand.png");
		picBox->SetPicBoxType(UIPicBox::PicBoxType::PBT_NINE);
		picBox->SetTexCornerSize(Sizef(32.0f, 16.0f), Sizef(32.0f, 16.0f));
		picBox->SetColor(Float3::WHITE);

		mUIFText->SetAnchorParamHor(15.0f, 1.0f);

		SetSize(170.0f, 31.0f);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
UIBlueblock::UIBlueblock(LoadConstructor value) :
UIFrame(value),
mLogicModule(0)
{
}
//----------------------------------------------------------------------------
void UIBlueblock::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIBlueblock, source);
}
//----------------------------------------------------------------------------
void UIBlueblock::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UIBlueblock::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIBlueblock::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void UIBlueblock::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIBlueblock, target);
}
//----------------------------------------------------------------------------
int UIBlueblock::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------