// EU_ResFrame.cpp

#include "PX2EU_ResFrame.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UIComboBox.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_ResFrame);
PX2_IMPLEMENT_STREAM(EU_ResFrame);
PX2_IMPLEMENT_FACTORY(EU_ResFrame);

//----------------------------------------------------------------------------
EU_ResFrame::EU_ResFrame()
{
	mToolFrame = new0 UIFrame();
	AttachChild(mToolFrame);
	mToolFrame->LocalTransform.SetTranslateY(-1.0f);
	UIPicBox *picBox = mToolFrame->CreateAddBackgroundPicBox();
	picBox->SetColor(PX2_UISM.Color_ToolBar);
	mToolFrame->SetAnchorHor(0.0f, 1.0f);
	mToolFrame->SetAnchorVer(1.0f, 1.0f);
	mToolFrame->SetPivot(0.5f, 1.0f);
	mToolFrame->SetSize(Sizef(0.0f, PX2_UISM.Size_ToolBar));

	Sizef butSize = Sizef(PX2_UISM.Size_ToolBarBut, PX2_UISM.Size_ToolBarBut);
	float butSpace = 2.0f;
	Sizef spliterSize = Sizef(4.0f,
		PX2_UISM.Size_ToolBar - 2);

	float addButPos = 0.0f;
	float addButPos1 = 0.0f;

	// edit type
	addButPos = butSize.Width / 2.0f;
	UIButton *butRefresh = new0 UIButton();
	mToolFrame->AttachChild(butRefresh);
	butRefresh->LocalTransform.SetTranslateY(-1.0f);
	butRefresh->SetAnchorHor(0.0f, 0.0f);
	butRefresh->SetAnchorVer(0.5f, 0.5f);
	butRefresh->SetAnchorParamHor(addButPos, 0.0f);
	butRefresh->SetSize(butSize);
	butRefresh->SetName("Refresh");
	butRefresh->CreateAddText("Refh");
	butRefresh->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butRefresh->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);

	addButPos1 -= 10.0f;
	addButPos1 = -butSize.Width*1.5f;
	UIComboBox *comboBox = new0 UIComboBox();
	mToolFrame->AttachChild(comboBox);
	comboBox->LocalTransform.SetTranslateY(-1.0f);
	comboBox->SetAnchorHor(1.0f, 1.0f);
	comboBox->SetAnchorVer(0.5f, 0.5f);
	comboBox->SetAnchorParamHor(addButPos1, 0.0f);
	comboBox->SetSize(butSize.Width*3.0f, butSize.Height);
	comboBox->SetName("ShowType");
	comboBox->AddChooseStr("Item");
	comboBox->AddChooseStr("Icon2");
	comboBox->AddChooseStr("Icon4");
	comboBox->SetChooseListHeightSameWithChooses();

	float dirWidth = 150.0f;
	mTreeCanvas = new0 UICanvas();
	AttachChild(mTreeCanvas);
	mTreeCanvas->SetName("ResTreeCanvas");
	mTreeCanvas->SetPivot(0.5f, 0.5f);
	mTreeCanvas->SetSize(dirWidth, 0.0f);
	mTreeCanvas->SetAnchorHor(0.0f, 0.0f);
	mTreeCanvas->SetAnchorParamHor(dirWidth / 2.0f, 0.0f);
	mTreeCanvas->SetAnchorVer(0.0f, 1.0f);
	mTreeCanvas->SetAnchorParamVer(0.0f, -PX2_UISM.Size_ToolBar);
	mTreeCanvas->SetBeforeDrawClear(false, false, true);

	mTreeDirs = new0 EU_ResTree(EU_ResTree::RTT_DIR);
	mTreeCanvas->AttachChild(mTreeDirs);
	mTreeDirs->LocalTransform.SetTranslateY(-1.0f);
	mTreeDirs->SetAnchorHor(0.0f, 1.0f);
	mTreeDirs->SetAnchorParamHor(0.0f, 0.0f);
	mTreeDirs->SetAnchorVer(0.0f, 1.0f);
	mTreeDirs->SetAnchorParamVer(0.0f, 0.0f);

	float spSize = 2.0f;
	mSplitter = new0 UISplitterFrame(false);
	AttachChild(mSplitter);
	mSplitter->SetPivot(0.0f, 0.5f);
	mSplitter->SetSize(spSize, 0.0f);
	mSplitter->LocalTransform.SetTranslateY(-10.0f);
	mSplitter->SetAnchorHor(0.0f, 0.0f);
	mSplitter->SetAnchorParamHor(dirWidth, 0.0f);
	mSplitter->SetAnchorVer(0.0f, 1.0f);
	mSplitter->SetAnchorParamVer(0.0f, -PX2_UISM.Size_ToolBar);
	mSplitter->SetMemUICallback(this,
		(UIFrame::MemUICallback)(&EU_ResFrame::_SpliterDragingCallback));

	mResCanvas = new0 UICanvas();
	AttachChild(mResCanvas);
	mResCanvas->SetBeforeDrawClear(false, false, true);
	mResCanvas->SetAnchorHor(0.0f, 1.0f);
	mResCanvas->SetAnchorParamHor(dirWidth + spSize, 0.0f);
	mResCanvas->SetAnchorVer(0.0f, 1.0f);
	mResCanvas->SetAnchorParamVer(0.0f, -PX2_UISM.Size_ToolBar);

	//mResList = new0 EU_ResList();
	//mResCanvas->AttachChild(mResList);
	//mResList->SetAnchorHor(0.0f, 1.0f);
	//mResList->SetAnchorVer(0.0f, 1.0f);

	mResGrid = new0 EU_GridFrame();
	mResCanvas->AttachChild(mResGrid);
	mResGrid->SetAnchorHor(0.0f, 1.0f);
	mResGrid->SetAnchorVer(0.0f, 1.0f);

	_SetFramePos();
}
//----------------------------------------------------------------------------
void EU_ResFrame::_SpliterDragingCallback(UIFrame *frame, UICallType type)
{
	if (UICT_SPLITTER_DRAGING == type)
	{
		_SetFramePos();
	}
}
//----------------------------------------------------------------------------
void EU_ResFrame::OnSizeChanged()
{
	UIFrame::OnSizeChanged();

	if (mSize.Width <= mSplitter->GetAnchorParamHor()[0])
	{
		mSplitter->SetAnchorParamHor(mSize.Width, 0.0f);
		_SetFramePos();
	}
}
//----------------------------------------------------------------------------
void EU_ResFrame::_SetFramePos()
{
	float anchorHor = mSplitter->GetAnchorHor()[0];
	float anchorParam = mSplitter->GetAnchorParamHor()[0];

	float leftWidth = anchorParam;

	mTreeCanvas->SetSize(leftWidth, 0.0f);
	mTreeCanvas->SetAnchorHor(0.0f, 0.0f);
	mTreeCanvas->SetAnchorParamHor(leftWidth / 2.0f, 0.0f);

	mResCanvas->SetAnchorHor(0.0f, 1.0f);
	mResCanvas->SetAnchorParamHor(leftWidth + mSplitter->GetWidth(), 0.0f);
}
//----------------------------------------------------------------------------
EU_ResFrame::~EU_ResFrame()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_ResFrame::EU_ResFrame(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_ResFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_ResFrame, source);
}
//----------------------------------------------------------------------------
void EU_ResFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_ResFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_ResFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_ResFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_ResFrame, target);
}
//----------------------------------------------------------------------------
int EU_ResFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------