// PX2EU_TimeLineFrame.cpp

#include "PX2EU_TimeLineFrame.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2Edit.hpp"
#include "PX2EditorEventType.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_TimeLineFrame);
PX2_IMPLEMENT_STREAM(EU_TimeLineFrame);
PX2_IMPLEMENT_FACTORY(EU_TimeLineFrame);

//----------------------------------------------------------------------------
EU_TimeLineFrame::EU_TimeLineFrame()
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
	float addButPos = 0.0f;
	addButPos = butSize.Width / 2.0f;

	for (int i = 0; i < 12; i++)
	{
		UIButton *but = new0 UIButton();
		mToolFrame->AttachChild(but);
		but->LocalTransform.SetTranslateY(-1.0f);
		but->SetAnchorHor(0.0f, 0.0f);
		but->SetAnchorVer(0.5f, 0.5f);
		but->SetAnchorParamHor(addButPos, 0.0f);
		but->SetSize(butSize);

		addButPos += butSize.Width + butSpace;

		UIFPicBox *fPicBox = new0 UIFPicBox();
		but->AttachChild(fPicBox);
		fPicBox->LocalTransform.SetTranslateY(-1.0f);
		fPicBox->SetAnchorHor(0.0f, 1.0f);
		fPicBox->SetAnchorVer(0.0f, 1.0f);
		fPicBox->SetAnchorParamHor(3.0f, -3.0f);
		fPicBox->SetAnchorParamVer(3.0f, -3.0f);

		fPicBox->SetDoPick(false);

		if (3 == i || 6 == i)
		{
			addButPos += butSpace;
		}

		if (0 == i)
		{
			but->SetName("Fit_Horz");
			fPicBox->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/fit_horz.png");
		}
		else if (1 == i)
		{
			but->SetName("Fit_Vert");
			fPicBox->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/fit_vert.png");
		}
		else if (2 == i)
		{
			but->SetName("Fit_All");
			fPicBox->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/fit_all.png");
		}
		else if (3 == i)
		{
			but->SetName("Fit_Select");
			fPicBox->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/fit_select.png");
		}

		else if (4 == i)
		{
			but->SetName("Pan");
			fPicBox->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/pan.png");
		}
		else if (5 == i)
		{
			but->SetName("Zoom");
			fPicBox->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/zoom.png");
		}

		else if (6 == i)
		{
			but->SetName("Curve_Auto");
			fPicBox->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/curve_auto.png");
		}
		else if (7 == i)
		{
			but->SetName("Curve_Clamped");
			fPicBox->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/curve_clamped.png");
		}
		else if (8 == i)
		{
			but->SetName("Curve_User");
			fPicBox->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/curve_user.png");
		}
		else if (9 == i)
		{
			but->SetName("Curve_Break");
			fPicBox->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/curve_break.png");
		}
		else if (10 == i)
		{
			but->SetName("Curve_Linear");
			fPicBox->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/curve_linear.png");
		}
		else if (11 == i)
		{
			but->SetName("Curve_Constant");
			fPicBox->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/curve_constant.png");
		}

		but->SetMemUICallback(this, (UIFrame::MemUICallback)(&EU_TimeLineFrame
			::_TimeLineCallback));
	}

	mCanvasTimeLineEdit = new0 EU_CanvasTimeLineEdit();
	AttachChild(mCanvasTimeLineEdit);
	mCanvasTimeLineEdit->LocalTransform.SetTranslateY(-1.0f);
	mCanvasTimeLineEdit->SetAnchorHor(0.0f, 1.0f);
	mCanvasTimeLineEdit->SetAnchorVer(0.0f, 1.0f);
	mCanvasTimeLineEdit->SetAnchorParamVer(0.0f, -PX2_UISM.Size_ToolBar);
}
//----------------------------------------------------------------------------
EU_TimeLineFrame::~EU_TimeLineFrame()
{
}
//----------------------------------------------------------------------------
void EU_TimeLineFrame::_TimeLineCallback(UIFrame *frame, UICallType type)
{
	const std::string &name = frame->GetName();
	UIButton *but = DynamicCast<UIButton>(frame);
	if (but && type == UICT_RELEASED)
	{
		if ("Fit_Horz" == name)
		{
			Event *ent = EditorEventSpace::CreateEventX(EditorEventSpace::TimeLine_FitHor);
			PX2_EW.BroadcastingLocalEvent(ent);
		}
		else if ("Fit_Vert" == name)
		{
			Event *ent = EditorEventSpace::CreateEventX(EditorEventSpace::TimeLine_FitVer);
			PX2_EW.BroadcastingLocalEvent(ent);
		}
		else if ("Fit_All" == name)
		{
			Event *ent = EditorEventSpace::CreateEventX(EditorEventSpace::TimeLine_FitAll);
			PX2_EW.BroadcastingLocalEvent(ent);
		}
		else if ("Fit_Select" == name)
		{
			Event *ent = EditorEventSpace::CreateEventX(EditorEventSpace::TimeLine_Fit_Selected);
			PX2_EW.BroadcastingLocalEvent(ent);
		}

		else if ("Pan" == name)
		{
			Event *ent = EditorEventSpace::CreateEventX(EditorEventSpace::TimeLine_Pan);
			PX2_EW.BroadcastingLocalEvent(ent);
		}
		else if ("Zoom" == name)
		{
			Event *ent = EditorEventSpace::CreateEventX(EditorEventSpace::TimeLine_Zoom);
			PX2_EW.BroadcastingLocalEvent(ent);
		}

		else if ("Curve_Auto" == name)
		{
			int mode = ICM_CURVE_AUTO;
			Event *ent = EditorEventSpace::CreateEventX(EditorEventSpace::TimeLine_CurveMode);
			ent->SetData<int>(mode);
			PX2_EW.BroadcastingLocalEvent(ent);
		}
		else if ("Curve_Clamped" == name)
		{
			int mode = ICM_CURVE_AUTOCLAMPED;
			Event *ent = EditorEventSpace::CreateEventX(EditorEventSpace::TimeLine_CurveMode);
			ent->SetData<int>(mode);
			PX2_EW.BroadcastingLocalEvent(ent);
		}
		else if ("Curve_User" == name)
		{
			int mode = ICM_CURVE_USER;
			Event *ent = EditorEventSpace::CreateEventX(EditorEventSpace::TimeLine_CurveMode);
			ent->SetData<int>(mode);
			PX2_EW.BroadcastingLocalEvent(ent);
		}
		else if ("Curve_Break" == name)
		{
			int mode = ICM_CURVE_BREAK;
			Event *ent = EditorEventSpace::CreateEventX(EditorEventSpace::TimeLine_CurveMode);
			ent->SetData<int>(mode);
			PX2_EW.BroadcastingLocalEvent(ent);
		}
		else if ("Curve_Linear" == name)
		{
			int mode = ICM_LINEAR;
			Event *ent = EditorEventSpace::CreateEventX(EditorEventSpace::TimeLine_CurveMode);
			ent->SetData<int>(mode);
			PX2_EW.BroadcastingLocalEvent(ent);
		}
		else if ("Curve_Constant" == name)
		{
			int mode = ICM_CONSTANT;
			Event *ent = EditorEventSpace::CreateEventX(EditorEventSpace::TimeLine_CurveMode);
			ent->SetData<int>(mode);
			PX2_EW.BroadcastingLocalEvent(ent);
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_TimeLineFrame::EU_TimeLineFrame(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_TimeLineFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_TimeLineFrame, source);
}
//----------------------------------------------------------------------------
void EU_TimeLineFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_TimeLineFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_TimeLineFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_TimeLineFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_TimeLineFrame, target);
}
//----------------------------------------------------------------------------
int EU_TimeLineFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------