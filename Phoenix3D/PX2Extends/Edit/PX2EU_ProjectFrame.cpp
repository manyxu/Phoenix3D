// EU_ProjectFrame.cpp

#include "PX2EU_ProjectFrame.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2EditEventType.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_ProjectFrame);
PX2_IMPLEMENT_STREAM(EU_ProjectFrame);
PX2_IMPLEMENT_FACTORY(EU_ProjectFrame);

//----------------------------------------------------------------------------
EU_ProjectFrame::EU_ProjectFrame()
{
	_CreateToolFrame();

	mSearchFrame = new0 UIFrame();
	AttachChild(mSearchFrame);
	mSearchFrame->LocalTransform.SetTranslateY(-1.0f);
	UIPicBox *picBoxSearch = mSearchFrame->CreateAddBackgroundPicBox();
	picBoxSearch->SetColor(PX2_UISM.Color_ToolBar);
	mSearchFrame->SetAnchorHor(0.0f, 1.0f);
	mSearchFrame->SetAnchorVer(1.0f, 1.0f);
	mSearchFrame->SetAnchorParamVer(-PX2_UISM.Size_ToolBar, 0.0f);
	mSearchFrame->SetPivot(0.5f, 1.0f);
	mSearchFrame->SetSize(Sizef(0.0f, PX2_UISM.Size_SearchBar));
	UIEditBox *eb = new0 UIEditBox();
	mSearchFrame->AttachChild(eb);
	eb->LocalTransform.SetTranslateY(-1.0f);
	eb->SetAnchorHor(0.0f, 1.0f);
	eb->SetAnchorVer(0.0f, 1.0f);
	eb->GetInputText()->SetFontScale(0.6f);

	mTreeProject = new0 EU_ProjectTree();
	AttachChild(mTreeProject);
	mTreeProject->LocalTransform.SetTranslateY(-2.0f);
	mTreeProject->SetAnchorHor(0.0f, 1.0f);
	mTreeProject->SetAnchorVer(0.0f, 1.0f);
	mTreeProject->SetAnchorParamVer(0.0f,
		-PX2_UISM.Size_ToolBar - PX2_UISM.Size_SearchBar);
}
//----------------------------------------------------------------------------
EU_ProjectFrame::~EU_ProjectFrame()
{
}
//----------------------------------------------------------------------------
void EU_ProjectFrame::_CreateToolFrame()
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

	// edit type
	addButPos = butSize.Width / 2.0f;
	UIButton *butShow_General = new0 UIButton();
	mToolFrame->AttachChild(butShow_General);
	butShow_General->LocalTransform.SetTranslateY(-1.0f);
	butShow_General->SetAnchorHor(0.0f, 0.0f);
	butShow_General->SetAnchorVer(0.5f, 0.5f);
	butShow_General->SetAnchorParamHor(addButPos, 0.0f);
	butShow_General->SetSize(butSize);
	butShow_General->SetName("ButShow_General");
	butShow_General->AddVisitor(this);
	butShow_General->CreateAddText("Gen");
	butShow_General->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butShow_General->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);

	addButPos += butSpace + butSize.Width;
	UIButton *butShow_Children = new0 UIButton();
	mToolFrame->AttachChild(butShow_Children);
	butShow_Children->LocalTransform.SetTranslateY(-1.0f);
	butShow_Children->SetAnchorHor(0.0f, 0.0f);
	butShow_Children->SetAnchorVer(0.5f, 0.5f);
	butShow_Children->SetAnchorParamHor(addButPos, 0.0f);
	butShow_Children->SetSize(butSize);
	butShow_Children->SetName("ButShow_Children");
	butShow_Children->AddVisitor(this);
	butShow_Children->CreateAddText("Chil");
	butShow_Children->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butShow_Children->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);

	addButPos += butSpace + butSize.Width;
	UIButton *butShow_Ctrls = new0 UIButton();
	mToolFrame->AttachChild(butShow_Ctrls);
	butShow_Ctrls->LocalTransform.SetTranslateY(-1.0f);
	butShow_Ctrls->SetAnchorHor(0.0f, 0.0f);
	butShow_Ctrls->SetAnchorVer(0.5f, 0.5f);
	butShow_Ctrls->SetAnchorParamHor(addButPos, 0.0f);
	butShow_Ctrls->SetSize(butSize);
	butShow_Ctrls->SetName("ButShow_Ctrls");
	butShow_Ctrls->AddVisitor(this);
	butShow_Ctrls->CreateAddText("Ctrl");
	butShow_Ctrls->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butShow_Ctrls->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);

	addButPos += butSpace + butSize.Width;
	UIButton *butShow_Mtls = new0 UIButton();
	mToolFrame->AttachChild(butShow_Mtls);
	butShow_Mtls->LocalTransform.SetTranslateY(-1.0f);
	butShow_Mtls->SetAnchorHor(0.0f, 0.0f);
	butShow_Mtls->SetAnchorVer(0.5f, 0.5f);
	butShow_Mtls->SetAnchorParamHor(addButPos, 0.0f);
	butShow_Mtls->SetSize(butSize);
	butShow_Mtls->SetName("ButShow_Mtls");
	butShow_Mtls->AddVisitor(this);
	butShow_Mtls->CreateAddText("Mtl");
	butShow_Mtls->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butShow_Mtls->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);

	addButPos += butSpace + butSize.Width;
	UIButton *butShow_Detail = new0 UIButton();
	mToolFrame->AttachChild(butShow_Detail);
	butShow_Detail->LocalTransform.SetTranslateY(-1.0f);
	butShow_Detail->SetAnchorHor(0.0f, 0.0f);
	butShow_Detail->SetAnchorVer(0.5f, 0.5f);
	butShow_Detail->SetAnchorParamHor(addButPos, 0.0f);
	butShow_Detail->SetSize(butSize);
	butShow_Detail->SetName("ButShow_Detail");
	butShow_Detail->AddVisitor(this);
	butShow_Detail->CreateAddText("Dta");
	butShow_Detail->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butShow_Detail->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);
}
//----------------------------------------------------------------------------
void EU_ProjectFrame::Visit(Object *obj, int info)
{
	const std::string &name = obj->GetName();
	UIButton *but = DynamicCast<UIButton>(obj);
	if (but)
	{
		if (UICT_PRESSED == info)
		{

		}
		else if (UICT_RELEASED == info)
		{
			if ("ButShow_General" == name)
			{
				mTreeProject->SetShowType(ST_GENERAL);
			}
			if ("ButShow_Children" == name)
			{
				mTreeProject->SetShowType(ST_CHILDREN);
			}
			if ("ButShow_Ctrls" == name)
			{
				mTreeProject->SetShowType(ST_CONTROLS);
			}
			if ("ButShow_Mtls" == name)
			{
				mTreeProject->SetShowType(ST_MATERIAL);
			}
			if ("ButShow_Detail" == name)
			{
				mTreeProject->SetShowType(ST_DETAIL);
			}
			else if ("SimuBut" == name)
			{
				Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_Simu);
				ent->SetData<int>(1);
				PX2_EW.BroadcastingLocalEvent(ent);
			}
			else if ("PlayBut" == name)
			{
				Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_Simu);
				ent->SetData<int>(2);
				PX2_EW.BroadcastingLocalEvent(ent);
			}
			else if ("PlayInWindowBut" == name)
			{
				Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_Simu);
				ent->SetData<int>(3);
				PX2_EW.BroadcastingLocalEvent(ent);

			}
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_ProjectFrame::EU_ProjectFrame(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_ProjectFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_ProjectFrame, source);
}
//----------------------------------------------------------------------------
void EU_ProjectFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_ProjectFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_ProjectFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_ProjectFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_ProjectFrame, target);
}
//----------------------------------------------------------------------------
int EU_ProjectFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------