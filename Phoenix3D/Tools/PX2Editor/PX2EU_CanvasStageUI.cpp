// PX2EU_CanvasStageUI.cpp

#include "PX2EU_CanvasStageUI.hpp"
#include "PX2Project.hpp"
#include "PX2EditEventType.hpp"
#include "PX2Edit.hpp"
using namespace PX2;

//PX2_IMPLEMENT_RTTI(PX2, EngineCanvas, EU_CanvasStageUI);
//PX2_IMPLEMENT_STREAM(EU_CanvasStageUI);
//PX2_IMPLEMENT_FACTORY(EU_CanvasStageUI);
//
////----------------------------------------------------------------------------
//EU_CanvasStageUI::EU_CanvasStageUI() :
//mIsRightPressed(false),
//mIsMoved(false)
//{
//	SetCanvasAddType(EngineCanvas::CAT_UI);
//
//	_CreateGridGeometry();
//	_CreateNodeCtrl();
//
//	Enable(false);
//
//	SetActivateSelfCtrled(false);
//}
////----------------------------------------------------------------------------
//EU_CanvasStageUI::~EU_CanvasStageUI()
//{
//}
////----------------------------------------------------------------------------
//void EU_CanvasStageUI::OnEvent(Event *event)
//{
//	EngineCanvas::OnEvent(event);
//
//	if (ProjectES::IsEqual(event, ProjectES::NewProject) || 
//		ProjectES::IsEqual(event, ProjectES::LoadedProject))
//	{
//
//	}
//	else if (ProjectES::IsEqual(event, ProjectES::CloseProject))
//	{
//
//	}
//	else if (ProjectES::IsEqual(event, ProjectES::NewUI))
//	{
//		CreateAndAddCanvasController();
//		_UpdateProjectRange();
//	}
//	else if (ProjectES::IsEqual(event, ProjectES::CloseUI))
//	{
//		DestoryCanvasController();
//	}
//	else if (EditEventSpace::IsEqual(event, EditEventSpace::SetEditType))
//	{
//		Edit::EditType editType = PX2_EDIT.GetEditType();
//		Enable(editType == Edit::ET_UI);
//		Show(editType == Edit::ET_UI);
//	}
//}
////----------------------------------------------------------------------------
//void EU_CanvasStageUI::_CreateGridGeometry()
//{
//	VertexFormat *vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PC);
//
//	VertexBuffer *vertexBuffer = new0 VertexBuffer(5, vf->GetStride());
//	mProjRangeSegment = new0 Polysegment(vf, vertexBuffer, true);
//	mProjRangeSegment->SetNumSegments(4);
//	mProjRangeSegment->SetMaterialInstance(VertexColor4Material::CreateUniqueInstance());
//	mProjRangeSegment->Culling = Movable::CULL_NEVER;
//	mProjRangeSegment->GetMaterialInstance()->GetMaterial()
//		->GetAlphaProperty(0, 0)->BlendEnabled = true;
//
//	_UpdateProjectRange();
//
//	mRangeNode = new0 Node();
//	AttachChild(mRangeNode);
//	mRangeNode->AttachChild(mProjRangeSegment);
//}
////----------------------------------------------------------------------------
//void EU_CanvasStageUI::_CreateNodeCtrl()
//{
//	mUIObjectCtrl = new0 UIObjectCtrl();
//	PX2_EW.ComeIn(mUIObjectCtrl);
//
//	mUIObjectNode = new0 Node();
//	AttachChild(mUIObjectNode);
//	mUIObjectNode->AttachChild(mUIObjectCtrl->GetCtrlsGroup());
//}
////----------------------------------------------------------------------------
//void EU_CanvasStageUI::_UpdateProjectRange()
//{
//	Project *proj = Project::GetSingletonPtr();
//	if (!proj) return;
//
//	float width = proj->GetWidth();
//	float height = proj->GetHeight();
//
//	VertexBufferAccessor vba(mProjRangeSegment);
//	vba.Position<Float3>(0) = Float3(-1.0f, 0.0f, -1.0f);
//	vba.Position<Float3>(1) = Float3(width, 0.0f, -1.0f);
//	vba.Position<Float3>(2) = Float3(width, 0.0f, height);
//	vba.Position<Float3>(3) = Float3(-1.0f, 0.0f, height);
//	vba.Position<Float3>(4) = Float3(-1.0f, 0.0f, -1.0f);
//	Float4 color = Float4::MakeColor(50, 50, 50, 165);
//	vba.Color<Float4>(0, 0) = color;
//	vba.Color<Float4>(0, 1) = color;
//	vba.Color<Float4>(0, 2) = color;
//	vba.Color<Float4>(0, 3) = color;
//	vba.Color<Float4>(0, 4) = color;
//
//	mProjRangeSegment->UpdateModelSpace(Renderable::GU_MODEL_BOUND_ONLY);
//	Renderer::UpdateAll(mProjRangeSegment->GetVertexBuffer());
//
//	float helfWidth = width / 2.0f;
//	float helfHeight = height / 2.0f;
//	mCameraNode->LocalTransform.SetTranslate(helfWidth, msUICameraY,
//		helfHeight);
//}
////----------------------------------------------------------------------------
//void EU_CanvasStageUI::_AdjustCameraPercent()
//{
//	Project *proj = Project::GetSingletonPtr();
//	if (!proj) return;
//
//	float uiCameraPercent = proj->GetEdit_UICameraPercent();
//	Sizef size = Sizef(mSize.Width*uiCameraPercent, 
//		mSize.Height*uiCameraPercent);
//}
////----------------------------------------------------------------------------
//
////----------------------------------------------------------------------------
//// 持久化支持
////----------------------------------------------------------------------------
//EU_CanvasStageUI::EU_CanvasStageUI(LoadConstructor value) :
//EngineCanvas(value)
//{
//}
////----------------------------------------------------------------------------
//void EU_CanvasStageUI::Load(InStream& source)
//{
//	PX2_BEGIN_DEBUG_STREAM_LOAD(source);
//
//	EngineCanvas::Load(source);
//	PX2_VERSION_LOAD(source);
//
//	PX2_END_DEBUG_STREAM_LOAD(EU_CanvasStageUI, source);
//}
////----------------------------------------------------------------------------
//void EU_CanvasStageUI::Link(InStream& source)
//{
//	EngineCanvas::Link(source);
//}
////----------------------------------------------------------------------------
//void EU_CanvasStageUI::PostLink()
//{
//	EngineCanvas::PostLink();
//}
////----------------------------------------------------------------------------
//bool EU_CanvasStageUI::Register(OutStream& target) const
//{
//	if (EngineCanvas::Register(target))
//	{
//		return true;
//	}
//
//	return false;
//}
////----------------------------------------------------------------------------
//void EU_CanvasStageUI::Save(OutStream& target) const
//{
//	PX2_BEGIN_DEBUG_STREAM_SAVE(target);
//
//	EngineCanvas::Save(target);
//	PX2_VERSION_SAVE(target);
//
//	PX2_END_DEBUG_STREAM_SAVE(EU_CanvasStageUI, target);
//}
////----------------------------------------------------------------------------
//int EU_CanvasStageUI::GetStreamingSize(Stream &stream) const
//{
//	int size = EngineCanvas::GetStreamingSize(stream);
//	size += PX2_VERSION_SIZE(mVersion);
//
//	return size;
//}
////----------------------------------------------------------------------------