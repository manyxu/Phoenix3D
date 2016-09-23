// EU_ResFrame.cpp

#include "PX2EU_ResFrame.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UIComboBox.hpp"
#include "PX2EditorEventType.hpp"
#include "PX2SelectResData.hpp"
#include "PX2Edit.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_ResFrame);
PX2_IMPLEMENT_STREAM(EU_ResFrame);
PX2_IMPLEMENT_FACTORY(EU_ResFrame);

//----------------------------------------------------------------------------
EU_ResFrame::EU_ResFrame()
{
	ComeInEventWorld();

	mToolFrame = new0 UIFrame();
	AttachChild(mToolFrame);
	mToolFrame->LocalTransform.SetTranslateY(-1.0f);
	UIPicBox *picBox = mToolFrame->CreateAddBackgroundPicBox();
	picBox->SetColor(PX2_UISM.Color_ToolBar);
	mToolFrame->SetAnchorHor(0.0f, 1.0f);
	mToolFrame->SetAnchorVer(1.0f, 1.0f);
	mToolFrame->SetPivot(0.5f, 1.0f);
	mToolFrame->SetSize(Sizef(0.0f, PX2_UISM.Size_ToolBar));
	mToolFrame->SetUIChildPickOnlyInSizeRange(false);

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
	butRefresh->SetMemUICallback(this,
		(UIFrame::MemUICallback)(&EU_ResFrame::_RefreshUICallback));

	addButPos1 -= 10.0f;
	addButPos1 -= (butSize.Width* 0.5f + butSpace);
	UIButton *butDown = new0 UIButton();
	mToolFrame->AttachChild(butDown);
	butDown->SetName("Down");
	butDown->CreateAddText("Dwn");
	butDown->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butDown->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);
	butDown->LocalTransform.SetTranslateY(-1.0f);
	butDown->SetAnchorHor(1.0f, 1.0f);
	butDown->SetAnchorVer(0.5f, 0.5f);
	butDown->SetAnchorParamHor(addButPos1, 0.0f);
	butDown->SetSize(butSize);
	butDown->SetMemUICallback(this,
		(UIFrame::MemUICallback)(&EU_ResFrame::_RefreshUICallback));

	addButPos1 -= (butSize.Width + butSpace);
	UIButton *butUp = new0 UIButton();
	mToolFrame->AttachChild(butUp);
	butUp->SetName("Up");
	butUp->CreateAddText("Up");
	butUp->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butUp->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);
	butUp->LocalTransform.SetTranslateY(-1.0f);
	butUp->SetAnchorHor(1.0f, 1.0f);
	butUp->SetAnchorVer(0.5f, 0.5f);
	butUp->SetAnchorParamHor(addButPos1, 0.0f);
	butUp->SetSize(butSize);
	butUp->SetMemUICallback(this,
		(UIFrame::MemUICallback)(&EU_ResFrame::_RefreshUICallback));

	addButPos1 -= (butSize.Width*2.0f + butSpace);
	UIComboBox *comboBox = new0 UIComboBox();
	mToolFrame->AttachChild(comboBox);
	comboBox->LocalTransform.SetTranslateY(-1.0f);
	comboBox->SetAnchorHor(1.0f, 1.0f);
	comboBox->SetAnchorVer(0.5f, 0.5f);
	comboBox->SetPivot(0.5f, 0.5f);
	comboBox->SetAnchorParamHor(addButPos1, 0.0f);
	comboBox->SetSize(butSize.Width*3.0f, butSize.Height);
	comboBox->SetName("ShowType");
	comboBox->AddChooseStr("Detail");
	comboBox->AddChooseStr("List");
	comboBox->Choose(0);
	comboBox->SetChooseListHeightSameWithChooses();
	comboBox->SetTextColor(Float3::MakeColor(220, 220, 220));
	comboBox->GetSelectButton()->GetText()->SetFontScale(0.65f);

	float dirWidth = 150.0f;
	mTreeCanvas = new0 UICanvas();
	AttachChild(mTreeCanvas);
	mTreeCanvas->LocalTransform.SetTranslateY(-1.0f);
	mTreeCanvas->SetName("ResTreeCanvas");
	mTreeCanvas->SetPivot(0.5f, 0.5f);
	mTreeCanvas->SetSize(dirWidth, 0.0f);
	mTreeCanvas->SetAnchorHor(0.0f, 0.0f);
	mTreeCanvas->SetAnchorParamHor(dirWidth / 2.0f, 0.0f);
	mTreeCanvas->SetAnchorVer(0.0f, 1.0f);
	mTreeCanvas->SetAnchorParamVer(0.0f, -PX2_UISM.Size_ToolBar);

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
	mSplitter->SetUIChildPickOnlyInSizeRange(false);
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
	mResCanvas->SetAnchorHor(0.0f, 1.0f);
	mResCanvas->SetAnchorParamHor(dirWidth + spSize, 0.0f);
	mResCanvas->SetAnchorVer(0.0f, 1.0f);
	mResCanvas->SetAnchorParamVer(0.0f, -PX2_UISM.Size_ToolBar);

	//mResList = new0 EU_ResList();
	//mResCanvas->AttachChild(mResList);
	//mResList->SetAnchorHor(0.0f, 1.0f);
	//mResList->SetAnchorVer(0.0f, 1.0f);

	float resFramePickInfoHeight = 50.0f;

	mResGrid = new0 EU_ResGridFrame();
	mResCanvas->AttachChild(mResGrid);
	mResGrid->SetAnchorHor(0.0f, 1.0f);
	mResGrid->SetAnchorVer(0.0f, 1.0f);
	mResGrid->SetAnchorParamVer(resFramePickInfoHeight, 0.0f);

	mResFramePickInfo = new0 UIFrame();
	mResCanvas->AttachChild(mResFramePickInfo);
	mResFramePickInfo->LocalTransform.SetTranslateY(-1.0f);
	mResFramePickInfo->SetAnchorHor(0.0f, 1.0f);
	mResFramePickInfo->SetAnchorVer(0.0f, 0.0f);
	mResFramePickInfo->SetSize(0.0f, resFramePickInfoHeight);
	mResFramePickInfo->SetPivot(0.5f, 0.0f);
	UIPicBox *picBoxBack = mResFramePickInfo->CreateAddBackgroundPicBox(true);
	picBoxBack->SetColor(Float3::MakeColor(80, 80, 80));

	mResFramePickInfoText = new0 UIFText();
	mResFramePickInfo->AttachChild(mResFramePickInfoText);
	mResFramePickInfoText->LocalTransform.SetTranslateY(-1.0f);
	mResFramePickInfoText->SetAnchorHor(0.0f, 1.0f);
	mResFramePickInfoText->SetAnchorParamHor(5.0f, -5.0f);
	mResFramePickInfoText->SetAnchorVer(0.0f, 1.0f);
	mResFramePickInfoText->SetAnchorParamVer(5.0f, -5.0f);
	mResFramePickInfoText->GetText()->SetFontScale(0.7f);
	mResFramePickInfoText->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_TOP);
	mResFramePickInfoText->GetText()->SetFontColor(Float3::WHITE);
	mResFramePickInfoText->GetText()->SetText("");

	_SetFramePos();
}
//----------------------------------------------------------------------------
EU_ResFrame::~EU_ResFrame()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void EU_ResFrame::_RefreshUICallback(UIFrame *frame, UICallType type)
{
	const std::string &name = frame->GetName();

	UIButton *but = DynamicCast<UIButton>(frame);
	if (but)
	{
		if (UICT_RELEASED == type)
		{
			if ("Refresh" == name)
			{
				Event *ent = EditorEventSpace::CreateEventX(
					EditorEventSpace::RefreshRes);
				PX2_EW.BroadcastingLocalEvent(ent);
			}
			else if ("Up" == name)
			{
				Event *ent = EditorEventSpace::CreateEventX(
					EditorEventSpace::UpRes);
				PX2_EW.BroadcastingLocalEvent(ent);
			}
			else if ("Down" == name)
			{
				Event *ent = EditorEventSpace::CreateEventX(
					EditorEventSpace::DownRes);
				PX2_EW.BroadcastingLocalEvent(ent);
			}
		}
	}
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

	if (mSplitter)
	{
		if (mSize.Width <= mSplitter->GetAnchorParamHor()[0])
		{
			mSplitter->SetAnchorParamHor(mSize.Width, 0.0f);
			_SetFramePos();
		}
	}
}
//----------------------------------------------------------------------------
void EU_ResFrame::OnEvent(Event *ent)
{
	if (EditorEventSpace::IsEqual(ent, EditorEventSpace::SelectRes))
	{
		const SelectResData &selectResData = PX2_EDIT.GetSelectedResource();
		mResFramePickInfoText->GetText()->SetText(selectResData.ResPathname);
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