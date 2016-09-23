// PX2EU_InspectorFrameRes.cpp

#include "PX2EU_InspectorFrameRes.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UISizeExtendControl.hpp"
#include "PX2EditorEventType.hpp"
#include "PX2SelectResData.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2Edit.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_InspectorFrameRes);
PX2_IMPLEMENT_STREAM(EU_InspectorFrameRes);
PX2_IMPLEMENT_FACTORY(EU_InspectorFrameRes);

//----------------------------------------------------------------------------
EU_InspectorFrameRes::EU_InspectorFrameRes()
{
	float showHeight = 250.0f;

	mPropertyGridRes = new0 EU_PropertyGridRes();
	AttachChild(mPropertyGridRes);

	mPropertyGridRes->SetAnchorHor(0.0f, 1.0f);
	mPropertyGridRes->SetAnchorVer(0.0f, 1.0f);

	mPropertyGridRes->SetAnchorParamHor(0.0f, 0.0f);
	mPropertyGridRes->SetAnchorParamVer(showHeight, 0.0f);

	mShowFrame = new0 UIFrame();
	AttachChild(mShowFrame);
	mShowFrame->SetAnchorHor(0.0f, 1.0f);
	mShowFrame->SetAnchorVer(0.0f, 0.0f);
	mShowFrame->SetSize(0.0f, showHeight);
	mShowFrame->SetPivot(0.5f, 0.0f);

	mShowCanvas = new0 UICanvas();
	mShowFrame->AttachChild(mShowCanvas);
	mShowCanvas->SetAnchorHor(0.0f, 1.0f);
	mShowCanvas->SetAnchorVer(0.0f, 1.0f);
	mShowCanvas->GetCameraNode()->GetCamera()->SetClearFlag(true, true, true);
	mShowCanvas->GetCameraNode()->GetCamera()->SetClearColor(
		Float4::MakeColor(80, 80, 80, 255));

	mShowPicBox = new0 UIFPicBox();
	mShowCanvas->AttachChild(mShowPicBox);
	mShowPicBox->SetAnchorHor(0.0f, 1.0f);
	mShowPicBox->SetAnchorVer(0.0f, 1.0f);

	mUISizeExtendControl = new0 UISizeExtendControl();
	mShowPicBox->AttachController(mUISizeExtendControl);
	mUISizeExtendControl->ResetPlay();

	ComeInEventWorld();
}
//----------------------------------------------------------------------------
EU_InspectorFrameRes::~EU_InspectorFrameRes()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void EU_InspectorFrameRes::OnEvent(Event *event)
{
	if (EditorEventSpace::IsEqual(event, EditorEventSpace::SelectRes))
	{
		const SelectResData &selectResData = PX2_EDIT.GetSelectedResource();

		const std::string &resPathName = selectResData.ResPathname;

		std::string outPath;
		std::string outBaseName;
		std::string outExtention;
		
		StringHelp::SplitFullFilename(resPathName, outPath, outBaseName, 
			outExtention);

		if ("png" == outExtention || "PNG" == outExtention)
		{
			Texture2D *tex2D = DynamicCast<Texture2D>(
				PX2_RM.BlockLoad(resPathName));
			if (tex2D)
			{
				float texWidth = tex2D->GetWidth();
				float texHeight = tex2D->GetHeight();

				mShowPicBox->GetUIPicBox()->SetTexture(tex2D);

				mUISizeExtendControl->SetWidthOverHeightPercent(
					texWidth / texHeight);
				mUISizeExtendControl->SetOriginHeight(texHeight);

				mShowPicBox->Show(true);
			}
		}
		else
		{
			if (mShowPicBox)
			{
				mShowPicBox->Show(false);
			}
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_InspectorFrameRes::EU_InspectorFrameRes(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_InspectorFrameRes::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_InspectorFrameRes, source);
}
//----------------------------------------------------------------------------
void EU_InspectorFrameRes::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_InspectorFrameRes::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_InspectorFrameRes::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_InspectorFrameRes::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_InspectorFrameRes, target);
}
//----------------------------------------------------------------------------
int EU_InspectorFrameRes::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------