// PX2EU_StageFrame.cpp

#include "PX2EU_StageFrame.hpp"
#include "PX2EU_BPFrame.hpp"
#include "PX2EU_Manager.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UICanvas.hpp"
#include "PX2Creater.hpp"
#include "PX2Project.hpp"
#include "PX2Edit.hpp"
#include "PX2UIFPicBox.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_StageFrame);
PX2_IMPLEMENT_STREAM(EU_StageFrame);
PX2_IMPLEMENT_FACTORY(EU_StageFrame);

//----------------------------------------------------------------------------
EU_StageFrame::EU_StageFrame()
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
	Sizef spliterSize = Sizef(4.0f, PX2_UISM.Size_ToolBar - 2);

	float addButPos = 0.0f;

	// edit type
	addButPos = butSize.Width / 2.0f;
	UIButton *butET_SCENE = new0 UIButton();
	mToolFrame->AttachChild(butET_SCENE);
	butET_SCENE->LocalTransform.SetTranslateY(-1.0f);
	butET_SCENE->SetAnchorHor(0.0f, 0.0f);
	butET_SCENE->SetAnchorVer(0.5f, 0.5f);
	butET_SCENE->SetAnchorParamHor(addButPos, 0.0f);
	butET_SCENE->SetSize(butSize);
	butET_SCENE->SetName("ButET_Scene");
	butET_SCENE->AddVisitor(this);
	butET_SCENE->CreateAddText("Sce");
	butET_SCENE->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butET_SCENE->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);

	addButPos += butSpace + butSize.Width;
	UIButton *butET_UI = new0 UIButton();
	mToolFrame->AttachChild(butET_UI);
	butET_UI->LocalTransform.SetTranslateY(-1.0f);
	butET_UI->SetAnchorHor(0.0f, 0.0f);
	butET_UI->SetAnchorVer(0.5f, 0.5f);
	butET_UI->SetAnchorParamHor(addButPos, 0.0f);
	butET_UI->SetSize(butSize);
	butET_UI->SetName("ButET_UI");
	butET_UI->AddVisitor(this);
	butET_UI->CreateAddText("UI");
	butET_UI->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butET_UI->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);

	addButPos += (butSize.Width / 2.0f + butSpace + spliterSize.Width / 2.0f);
	UIFPicBox *spliter = new0 UIFPicBox();
	mToolFrame->AttachChild(spliter);
	spliter->GetUIPicBox()->SetTexture("Data/engine/white.png");
	spliter->GetUIPicBox()->SetColor(PX2_UISM.Color_ToolBarSpliter);
	spliter->LocalTransform.SetTranslateY(-1.0f);
	spliter->SetAnchorHor(0.0f, 0.0f);
	spliter->SetAnchorVer(0.5f, 0.5f);
	spliter->SetAnchorParamHor(addButPos, 0.0f);
	spliter->SetSize(spliterSize);

	// edit mode
	addButPos += (spliterSize.Width / 2.0f + butSpace + butSize.Width / 2.0f);
	UIButton *butEM_Select = new0 UIButton();
	mToolFrame->AttachChild(butEM_Select);
	butEM_Select->LocalTransform.SetTranslateY(-1.0f);
	butEM_Select->SetAnchorHor(0.0f, 0.0f);
	butEM_Select->SetAnchorVer(0.5f, 0.5f);
	butEM_Select->SetAnchorParamHor(addButPos, 0.0f);
	butEM_Select->SetSize(butSize);
	butEM_Select->SetName("ButEM_Select");
	butEM_Select->AddVisitor(this);
	butEM_Select->CreateAddText("Sel");
	butEM_Select->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butEM_Select->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);

	addButPos += butSize.Width + butSpace;
	UIButton *butEM_Trans = new0 UIButton();
	mToolFrame->AttachChild(butEM_Trans);
	butEM_Trans->LocalTransform.SetTranslateY(-1.0f);
	butEM_Trans->SetAnchorHor(0.0f, 0.0f);
	butEM_Trans->SetAnchorVer(0.5f, 0.5f);
	butEM_Trans->SetAnchorParamHor(addButPos, 0.0f);
	butEM_Trans->SetSize(butSize);
	butEM_Trans->SetName("ButEM_Trans");
	butEM_Trans->AddVisitor(this);
	butEM_Trans->CreateAddText("Tra");
	butEM_Trans->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butEM_Trans->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);

	addButPos += butSize.Width + butSpace;
	UIButton *butEM_Rotate = new0 UIButton();
	mToolFrame->AttachChild(butEM_Rotate);
	butEM_Rotate->LocalTransform.SetTranslateY(-1.0f);
	butEM_Rotate->SetAnchorHor(0.0f, 0.0f);
	butEM_Rotate->SetAnchorVer(0.5f, 0.5f);
	butEM_Rotate->SetAnchorParamHor(addButPos, 0.0f);
	butEM_Rotate->SetSize(butSize);
	butEM_Rotate->SetName("ButEM_Rotate");
	butEM_Rotate->AddVisitor(this);
	butEM_Rotate->CreateAddText("Rot");
	butEM_Rotate->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butEM_Rotate->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);

	addButPos += butSize.Width + butSpace;
	UIButton *butEM_Scale = new0 UIButton();
	mToolFrame->AttachChild(butEM_Scale);
	butEM_Scale->LocalTransform.SetTranslateY(-1.0f);
	butEM_Scale->SetAnchorHor(0.0f, 0.0f);
	butEM_Scale->SetAnchorVer(0.5f, 0.5f);
	butEM_Scale->SetAnchorParamHor(addButPos, 0.0f);
	butEM_Scale->SetSize(butSize);
	butEM_Scale->SetName("ButEM_Scale");
	butEM_Scale->AddVisitor(this);
	butEM_Scale->CreateAddText("Sle");
	butEM_Scale->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butEM_Scale->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);

	AddStage();
	PX2_EDIT.SetEditType(Edit::ET_SCENE);

	ComeInEventWorld();

	SetActivateSelfCtrled(false);
}
//----------------------------------------------------------------------------
EU_StageFrame::~EU_StageFrame()
{
	RemoveStage();

	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void EU_StageFrame::Visit(Object *obj, int info)
{
	const std::string &name = obj->GetName();

	UIButton *but = DynamicCast<UIButton>(obj);
	if (but)
	{
		if (UICT_RELEASED == info)
		{
			if (name == "ButEM_Select")
			{
				PX2_EDIT.SetEditMode(Edit::EM_SELECT);
			}
			else if (name == "ButEM_Trans")
			{
				PX2_EDIT.SetEditMode(Edit::EM_TRANSLATE);
			}
			else if (name == "ButEM_Rotate")
			{
				PX2_EDIT.SetEditMode(Edit::EM_ROTATE);
			}
			else if (name == "ButEM_Scale")
			{
				PX2_EDIT.SetEditMode(Edit::EM_SCALE);
			}
			else if (name == "ButET_Scene")
			{
				PX2_EDIT.SetEditType(Edit::ET_SCENE);
			}
			else if (name == "ButET_UI")
			{
				PX2_EDIT.SetEditType(Edit::ET_UI);
			}
		}
	}
}
//----------------------------------------------------------------------------
void EU_StageFrame::AddStage()
{
	mCanvasStage = new0 EU_CanvasStage();
	mCanvasStage->SetName("EU_CanvasStage");
	mCanvasStage->LocalTransform.SetTranslateY(-1.0f);
	AttachChild(mCanvasStage);
	mCanvasStage->SetAnchorHor(0.0f, 1.0f);
	mCanvasStage->SetAnchorVer(0.0f, 1.0f);
	mCanvasStage->SetAnchorParamVer(0.0f, -PX2_UISM.Size_ToolBar);
}
//----------------------------------------------------------------------------
void EU_StageFrame::RemoveStage()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_StageFrame::EU_StageFrame(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_StageFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_StageFrame, source);
}
//----------------------------------------------------------------------------
void EU_StageFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_StageFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_StageFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_StageFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_StageFrame, target);
}
//----------------------------------------------------------------------------
int EU_StageFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------