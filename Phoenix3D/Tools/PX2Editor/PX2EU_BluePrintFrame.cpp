// PX2EU_BluePrintFrame.cpp

#include "PX2EU_BluePrintFrame.hpp"
#include "PX2EU_BPFrame.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UICanvas.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2Project.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_BluePrintFrame);
PX2_IMPLEMENT_STREAM(EU_BluePrintFrame);
PX2_IMPLEMENT_FACTORY(EU_BluePrintFrame);

//----------------------------------------------------------------------------
EU_BluePrintFrame::EU_BluePrintFrame()
{
	mToolFrame = new0 UIFrame();
	AttachChild(mToolFrame);
	mToolFrame->LocalTransform.SetTranslateY(-2.0f);
	mToolFrame->SetSize(0.0f, PX2_UISM.Size_ToolBar);
	mToolFrame->SetAnchorHor(0.0f, 1.0f);
	mToolFrame->SetAnchorVer(1.0f, 1.0f);
	mToolFrame->SetPivot(0.5f, 1.0f);
	UIPicBox *picBox = mToolFrame->CreateAddBackgroundPicBox();
	picBox->SetColor(PX2_UISM.Color_ToolBar);

	mTableFrame = new0 UITabFrame();
	AttachChild(mTableFrame);
	mTableFrame->LocalTransform.SetTranslateY(-1.0f);
	mTableFrame->SetLayoutPos(UITabFrame::LPT_TOP);
	mTableFrame->SetTabWidth(80.0f);
	mTableFrame->SetSkinAui(false);
	mTableFrame->SetAnchorHor(0.0f, 1.0f);
	mTableFrame->SetAnchorVer(0.0f, 1.0f);
	mTableFrame->SetAnchorParamVer(0.0f, -PX2_UISM.Size_ToolBar);
	UIPicBox *backPicBox = mTableFrame->CreateAddBackgroundPicBox();
	backPicBox->SetColor(Float3::MakeColor(195, 195, 195));

	ComeInEventWorld();
}
//----------------------------------------------------------------------------
EU_BluePrintFrame::~EU_BluePrintFrame()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void EU_BluePrintFrame::OnEvent(PX2::Event *event)
{
	if (ProjectES::IsEqual(event, ProjectES::NewBP))
	{
		mTableFrame->RemoveAllTabs();

		BPPackage *bpPackage = PX2_PROJ.GetBPPackage();
		if (bpPackage)
		{
			AddBPFrame(bpPackage->GetName(), bpPackage);
			mTableFrame->SetActiveTab(bpPackage->GetName());
		}
	}
	else if (ProjectES::IsEqual(event, ProjectES::CloseBP))
	{
		mTableFrame->RemoveAllTabs();
	}
}
//----------------------------------------------------------------------------
EU_BPFrame *EU_BluePrintFrame::AddBPFrame(const std::string &name,
	UIFrame *bpObject)
{
	EU_BPFrame *bpFrame = 0;

	if (!mTableFrame->IsHasTab(name))
	{
		bpFrame = new0 EU_BPFrame();
		mTableFrame->AddTab(name, name, bpFrame);
		bpFrame->LocalTransform.SetTranslateY(-1.0f);
		bpFrame->SetAnchorHor(0.0f, 1.0f);
		bpFrame->SetAnchorVer(0.0f, 1.0f);
		bpFrame->SetName(name);
		UICanvas *canvas = bpFrame->CreateChildUICanvas();
		canvas->AttachChild(bpObject);
		EU_CanvasBP *canvasBP = DynamicCast<EU_CanvasBP>(canvas);
		if (canvasBP)
		{
			canvasBP->SetInitPackageSize(bpObject->GetSize());
		}
	}

	return bpFrame;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// �־û�֧��
//----------------------------------------------------------------------------
EU_BluePrintFrame::EU_BluePrintFrame(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_BluePrintFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_BluePrintFrame, source);
}
//----------------------------------------------------------------------------
void EU_BluePrintFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_BluePrintFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_BluePrintFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_BluePrintFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_BluePrintFrame, target);
}
//----------------------------------------------------------------------------
int EU_BluePrintFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------