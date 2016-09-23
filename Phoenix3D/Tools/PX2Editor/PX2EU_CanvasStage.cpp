// PX2EU_CanvasStage.cpp

#include "PX2EU_CanvasStage.hpp"
#include "PX2EU_Manager.hpp"
#include "PX2InputEventData.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2SelectionManager.hpp"
#include "PX2Project.hpp"
#include "PX2Edit.hpp"
#include "PX2EditEventType.hpp"
#include "PX2EditorEventType.hpp"
#include "PX2LanguageManager.hpp"
#include "PX2SelectionManager.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2VertexColor4Material.hpp"
#include "PX2Application.hpp"
#include "PX2EU_ProjectEventController.hpp"
#include "PX2Picker.hpp"
#include "PX2GraphicsEventType.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UICanvas, EU_CanvasStage);
PX2_IMPLEMENT_STREAM(EU_CanvasStage);
PX2_IMPLEMENT_FACTORY(EU_CanvasStage);

//----------------------------------------------------------------------------
EU_CanvasStage::EU_CanvasStage():
mViewType(VT_PERSPECTIVE),
mViewDetail(VD_TEXTURED),
mCurCameraMoveSpeed_W(0.0f),
mCurCameraMoveSpeed_S(0.0f),
mCurCameraMoveSpeed_A(0.0f),
mCurCameraMoveSpeed_D(0.0f),
mIsCameraDraging(false)
{
	SetName("EU_CanvasStage");

	SetAnchorHor(0.0f, 1.0f);
	SetAnchorVer(0.0f, 1.0f);

	mStageCameraNodeRoot = new0 Node();
	AttachChild(mStageCameraNodeRoot);
	mStageCameraNodeRoot->WorldTransformIsCurrent = true;

	mStageCameraNode = PX2_CREATER.CreateNode_Camera();
	mStageCameraNode->SetName("MainCameraNode");
	mStageCameraNode->LocalTransform.SetTranslate(-20.0f, -20.0f, 10.0f);
	mStageCameraNode->LookAt(APoint(0.0f, 0.0f, 0.0f));
	mStageCameraNode->GetCamera()->SetClearFlag(false, false, false);
	mStageCameraNodeRoot->AttachChild(mStageCameraNode);	
	Camera *editorCamera = mStageCameraNode->GetCamera();
	float upFovDegrees = 0.0f;
	float aspectRation = 0.0f;
	float dMin = 0.0f;
	float dMax = 0.0f;
	editorCamera->GetFrustum(upFovDegrees, aspectRation, dMin, dMax);
	aspectRation = mSize.Width / mSize.Height;
	editorCamera->SetFrustum(upFovDegrees, aspectRation, dMin, 5000.0f);
	PX2_EDIT.SetEditorCameraNode(mStageCameraNode);
	InterpCurveTranslateController *transCtrl = new0 InterpCurveTranslateController();
	mStageCameraNode->AttachController(transCtrl);
	transCtrl->SetName("Camera_ICTC");
	transCtrl->SetPlaySelfCtrl(true);

	_CreateGridGeometry();
	_CreateSceneCanvas();
	_CreateNodeCtrl();
	_CreateCameraViewCanvas();
	ShowContent(false);

	Enable(false);

	SetOverCameraNode(mStageCameraNode);

	SetActivateSelfCtrled(false);

	SetClearFlag(true, true, true);
	SetClearColor(Float4::MakeColor(0, 162, 232, 255));
	mCamera->SetClearFlag(false, false, false);
}
//----------------------------------------------------------------------------
EU_CanvasStage::~EU_CanvasStage()
{
	if (mSceneNodeCtrl)
		PX2_EW.GoOut(mSceneNodeCtrl);

	if (mBoundCtrl)
		PX2_EW.GoOut(mBoundCtrl);
}
//----------------------------------------------------------------------------
void EU_CanvasStage::SetOverCameraNode(CameraNode *cameraNode)
{
	mStageCameraNode = cameraNode;

	mGridCanvas->SetOverCamera(mStageCameraNode->GetCamera());
	mSceneCanvas->SetOverCamera(mStageCameraNode->GetCamera());
	mSceneNodeCtrlCanvas->SetOverCamera(mStageCameraNode->GetCamera());
}
//----------------------------------------------------------------------------
CameraNode *EU_CanvasStage::GetOverCameraNode()
{
	return mStageCameraNode;
}
//----------------------------------------------------------------------------
void EU_CanvasStage::SetViewType(ViewType viewType)
{
	mViewType = viewType;
}
//----------------------------------------------------------------------------
EU_CanvasStage::ViewType EU_CanvasStage::GetViewType()
{
	return mViewType;
}
//----------------------------------------------------------------------------
void EU_CanvasStage::SetViewDetail(ViewDetail viewDetail)
{
	mViewDetail = viewDetail;
}
//----------------------------------------------------------------------------
EU_CanvasStage::ViewDetail EU_CanvasStage::GetViewDetail() const
{
	return mViewDetail;
}
//----------------------------------------------------------------------------
void EU_CanvasStage::Enable(bool enable)
{
	UICanvas::Enable(enable);
}
//----------------------------------------------------------------------------
void EU_CanvasStage::ShowContent(bool show)
{
	mGridCanvas->Show(show);
	mSceneCanvas->Show(show);
	mSceneNodeCtrlCanvas->Show(show);
}
//----------------------------------------------------------------------------
void EU_CanvasStage::OnEvent(Event *event)
{
	UICanvas::OnEvent(event);

	if (InputEventSpace::IsEqual(event, InputEventSpace::KeyPressed))
	{
		if (!PX2_EDIT.IsLeftMouseDown && !PX2_EDIT.IsRightMouseDown &&
			!PX2_EDIT.IsMidMouseDown)
		{
			InputEventData ieData = event->GetData<InputEventData>();

			if (KC_Q == ieData.KCode)
			{
				PX2_EDIT.SetEditMode(Edit::EM_SELECT);
			}
			else if (KC_W == ieData.KCode)
			{
				PX2_EDIT.SetEditMode(Edit::EM_TRANSLATE);
			}
			else if (KC_E == ieData.KCode)
			{
				PX2_EDIT.SetEditMode(Edit::EM_ROTATE);
			}
			else if (KC_R == ieData.KCode)
			{
				PX2_EDIT.SetEditMode(Edit::EM_SCALE);
			}
		}
	}
	else if (EditorEventSpace::IsEqual(event, EditorEventSpace::SetEditType))
	{
		Edit::EditType editType = PX2_EDIT.GetEditType();
		bool editScene = (Edit::EditType::ET_SCENE == editType ||
			Edit::EditType::ET_TERRAIN == editType);
		Enable(editScene);
	}
	else if (GraphicsES::IsEqual(event, EditES::AddSelect))
	{
		_UpdateCameraCanvas();
	}
	else if (GraphicsES::IsEqual(event, EditES::RemoveSelect))
	{
		_UpdateCameraCanvas();
	}
	else if (GraphicsES::IsEqual(event, EditES::RemoveAllSelects))
	{
		_UpdateCameraCanvas();
	}
}
//----------------------------------------------------------------------------
void EU_CanvasStage::OnSizeChanged()
{
	UICanvas::OnSizeChanged(); 

	Camera *editorCamera = mStageCameraNode->GetCamera();

	if (editorCamera)
	{
		float upFovDegrees = 0.0f;
		float aspectRation = 0.0f;
		float dMin = 0.0f;
		float dMax = 0.0f;
		editorCamera->GetFrustum(upFovDegrees, aspectRation, dMin, dMax);

		aspectRation = mSize.Width / mSize.Height;
		editorCamera->SetFrustum(upFovDegrees, aspectRation, dMin, dMax);
	}

	if (mCameraCanvasView)
	{
		Sizef cameraCanvasSize = mCameraCanvasSize;

		const Sizef &size = GetSize();
		if (size.Width < cameraCanvasSize.Width)
		{
			cameraCanvasSize.Width = size.Width;
		}
		if (cameraCanvasSize.Width < 0.0f)
			cameraCanvasSize.Width = 0.0f;

		if (size.Height < cameraCanvasSize.Height)
		{
			cameraCanvasSize.Height = size.Height;
		}
		if (cameraCanvasSize.Height < 0.0f)
			cameraCanvasSize.Height = 0.0f;

		mCameraCanvasView->SetSize(cameraCanvasSize);
	}
}
//----------------------------------------------------------------------------
void EU_CanvasStage::UpdateWorldData(double applicationTime,
	double elapsedTime)
{
	if ((PX2_EDIT.IsLeftMouseDown || PX2_EDIT.IsRightMouseDown ||
		PX2_EDIT.IsMidMouseDown || PX2_EDIT.IsShiftDown) &&
		!PX2_EDIT.IsCtrlDown)
	{
		float cameraMoveSpeed = PX2_EDIT.GetCameraMoveSpeed();

		bool isDown_W = PX2_EDIT.IsKeyDown_W;
		bool isDown_S = PX2_EDIT.IsKeyDown_S;
		bool isDown_A = PX2_EDIT.IsKeyDown_A;
		bool isDown_D = PX2_EDIT.IsKeyDown_D;

		mCurCameraMoveSpeed_W = 0.0f;
		mCurCameraMoveSpeed_S = 0.0f;
		mCurCameraMoveSpeed_A = 0.0f;
		mCurCameraMoveSpeed_D = 0.0f;

		float speedParam0 = (cameraMoveSpeed / 0.2f);
		float speedParam1 = (cameraMoveSpeed / 0.1f);

		_ProcessKeyMove(isDown_W, cameraMoveSpeed, (float)elapsedTime, mCurCameraMoveSpeed_W);
		_ProcessKeyMove(isDown_S, cameraMoveSpeed, (float)elapsedTime, mCurCameraMoveSpeed_S);
		_ProcessKeyMove(isDown_A, cameraMoveSpeed, (float)elapsedTime, mCurCameraMoveSpeed_A);
		_ProcessKeyMove(isDown_D, cameraMoveSpeed, (float)elapsedTime, mCurCameraMoveSpeed_D);

		float moveValue_W = mCurCameraMoveSpeed_W * (float)elapsedTime;
		float moveValue_S = mCurCameraMoveSpeed_S * (float)elapsedTime;
		float moveValue_A = mCurCameraMoveSpeed_A * (float)elapsedTime;
		float moveValue_D = mCurCameraMoveSpeed_D * (float)elapsedTime;

		float moveValueV = moveValue_W - moveValue_S;
		float moveValueH = moveValue_A - moveValue_D;

		if (VT_PERSPECTIVE == mViewType)
		{
			if (PX2_EDIT.IsAltDown)
			{
				_MoveCamera(moveValueH, moveValueV);
			}
			else
			{
				_ZoomCamera(moveValueV);
				_MoveCamera(moveValueH, 0.0f);
			}
		}
		else if (VT_TOP == mViewType)
		{
			_MoveCamera(moveValueH, moveValueV);
		}
		else if (VT_FRONT == mViewType)
		{
			_MoveCamera(moveValueH, -moveValueV);
		}
	}

	UICanvas::UpdateWorldData(applicationTime, elapsedTime);

	if (mStageCameraNodeRoot)
	{
		mStageCameraNodeRoot->Update(applicationTime, elapsedTime);
	}

	mSceneNodeCtrl->UpdateCtrlTrans();
	mBoundCtrl->UpdateCtrl();
}
//----------------------------------------------------------------------------
void EU_CanvasStage::_UpdateCameraCanvas()
{
	int numObjscts = PX2_SELECTM_E->GetNumObjects();
	if (1 == numObjscts)
	{
		Object *firstSelObj = PX2_SELECTM_E->GetFirstObject();
		CameraNode *camNode = DynamicCast<CameraNode>(firstSelObj);
		if (camNode)
		{
			if (mCameraCanvasView)
			{
				Scene *scene = PX2_PROJ.GetScene();

				mCameraCanvasView->SetOverCamera(camNode->GetCamera());
				mCameraCanvasView->SetRenderNode(scene);
				mCameraCanvasView->Show(true);
			}
		}
		else
		{
			if (mCameraCanvasView)
			{
				mCameraCanvasView->SetOverCamera(0);
				mCameraCanvasView->SetRenderNode(0);
				mCameraCanvasView->Show(false);
			}
		}
	}
	else
	{
		if (mCameraCanvasView)
		{
			mCameraCanvasView->SetOverCamera(0);
			mCameraCanvasView->SetRenderNode(0);
			mCameraCanvasView->Show(false);
		}
	}
}
//----------------------------------------------------------------------------
void EU_CanvasStage::_CreateGridGeometry()
{
	float gridSize = PX2_EDIT.GetEditParams()->GridSize;

	VertexFormat *vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PC);
	StandardMesh sM(vf);
	mGrid = sM.LineRectangleForEditor(81, 81, gridSize, gridSize,
		Float4(0.43f, 0.43f, 0.43f, 0.7f));

	PX2::RenderablePtr mGridAxis;
	PX2::VertexBufferAccessor mGridAxisAssessor;

	VertexBuffer *vb = new0 VertexBuffer(4, vf->GetStride(), Buffer::BU_DYNAMIC);
	VertexBufferAccessor vba;

	vba.ApplyTo(vf, vb);
	vba.Position<Float3>(0) = Float3(-gridSize, 0.0f, 0.0f);
	vba.Color<Float4>(0, 0) = Float4::RED;
	vba.Position<Float3>(1) = Float3(gridSize, 0.0f, 0.0f);
	vba.Color<Float4>(0, 1) = Float4::RED;

	vba.Position<Float3>(2) = Float3(0.0f, -gridSize, 0.0f);
	vba.Color<Float4>(0, 2) = Float4::GREEN;
	vba.Position<Float3>(3) = Float3(0.0f, gridSize, 0.0f);
	vba.Color<Float4>(0, 3) = Float4::GREEN;

	mGridAxis = new0 Polysegment(vf, vb, false);
	mGridAxis->UpdateModelSpace(Renderable::GU_MODEL_BOUND_ONLY);

	VertexColor4MaterialPtr mtl = new0 VertexColor4Material();
	mtl->GetAlphaProperty(0, 0)->BlendEnabled = true;

	mGrid->SetMaterialInstance(mtl->CreateInstance());
	mGridAxis->SetMaterialInstance(mtl->CreateInstance());

	mGridNode = new0 Node();
	mGridNode->AttachChild(mGridAxis);
	mGridNode->AttachChild(mGrid);
	mGridNode->Update(Time::GetTimeInSeconds(), true);

	mGridCanvas = new0 UICanvas();
	AttachChild(mGridCanvas);
	mGridCanvas->SetName("SceneGridCanvas");
	mGridCanvas->SetRenderNode(mGridNode);
	mGridCanvas->SetRenderNodeUpdate(true);
	mGridCanvas->SetAnchorHor(0.0f, 1.0f);
	mGridCanvas->SetAnchorVer(0.0f, 1.0f);
	mGridCanvas->SetClearFlag(false, false, false);
}
//----------------------------------------------------------------------------
void EU_CanvasStage::_CreateSceneCanvas()
{
	mSceneCanvas = new0 UICanvas();
	AttachChild(mSceneCanvas);
	mSceneCanvas->SetName("SceneCanvas");
	mSceneCanvas->SetAnchorHor(0.0f, 1.0f);
	mSceneCanvas->SetAnchorVer(0.0f, 1.0f);
	mSceneCanvas->SetClearFlag(false, false, false);
	mSceneCanvas->SetRenderNodeUpdate(true);

	mSceneCanvas->AttachController(new0 EU_ProjectEventController());
}
//----------------------------------------------------------------------------
void EU_CanvasStage::_CreateCameraViewCanvas()
{
	mCameraCanvasSize = Sizef(150.0f, 100.0f);

	mCameraCanvasView = new0 UICanvas();
	AttachChild(mCameraCanvasView);
	mCameraCanvasView->SetAnchorHor(1.0f, 1.0f);
	mCameraCanvasView->SetAnchorVer(1.0f, 1.0f);
	mCameraCanvasView->SetSize(mCameraCanvasSize);
	mCameraCanvasView->SetPivot(1.0f, 1.0f);
	mCameraCanvasView->RemoveAllCameras();
	mCameraCanvasView->SetClearFlag(true, true, true);
	mCameraCanvasView->SetClearColor(Float4::BLUE);
	mCameraCanvasView->Show(false);
}
//----------------------------------------------------------------------------
void EU_CanvasStage::_CreateNodeCtrl()
{
	mSceneNodeCtrl = new0 SceneNodeCtrl();
	PX2_EW.ComeIn(mSceneNodeCtrl);

	mBoundCtrl = new0 BoundCtrl();
	PX2_EW.ComeIn(mBoundCtrl);

	mCameraCtrl = new0 CameraCtrl();
	PX2_EW.ComeIn(mCameraCtrl);

	mSceneCtrlNode = new0 Node();
	mSceneCtrlNode->AttachChild(mSceneNodeCtrl->GetCtrlsGroup());
	mSceneCtrlNode->AttachChild(mBoundCtrl->GetCtrlsGroup());
	mSceneCtrlNode->AttachChild(mCameraCtrl->GetNode());
	mSceneCtrlNode->Update(Time::GetTimeInSeconds(), true);
	mSceneCtrlNode->AttachChild(PX2_EDIT.GetHelpNode());

	mSceneNodeCtrlCanvas = new0 UICanvas();
	AttachChild(mSceneNodeCtrlCanvas);
	mSceneNodeCtrlCanvas->SetName("SceneCtrlNodeCanvas");
	mSceneNodeCtrlCanvas->SetAnchorHor(0.0f, 1.0f);
	mSceneNodeCtrlCanvas->SetAnchorParamHor(0.0f, 0.0f);
	mSceneNodeCtrlCanvas->SetAnchorVer(0.0f, 1.0f);
	mSceneNodeCtrlCanvas->SetAnchorParamVer(0.0f, 1.0f);
	mSceneNodeCtrlCanvas->SetRenderNode(mSceneCtrlNode);
	mSceneNodeCtrlCanvas->SetRenderNodeUpdate(true);
	mSceneNodeCtrlCanvas->SetClearFlag(false, true, true);
}
//----------------------------------------------------------------------------
void EU_CanvasStage::OnLeftDown(const APoint &worldPos)
{
	if (!IsEnable())
		return;

	const Rectf &rectWorld = GetWorldRect();
	if (!rectWorld.IsInsize(worldPos.X(), worldPos.Z()))
		return;

	UICanvas::OnLeftDown(worldPos);

	mIsSelectMovableTransChanged = false;
	mURDoCommand = 0;

	APoint viewPortPos = WorldPosToViewPortPos(worldPos);
	_ClickSelectPos(viewPortPos);

	Edit::EditType et = PX2_EDIT.GetEditType();
	if (Edit::ET_SCENE == et)
	{
		if (mSceneNodeCtrl)
		{
			Camera *camera = mSceneNodeCtrlCanvas->GetOverCamera();
			mSceneNodeCtrl->OnLeftDown(camera, viewPortPos, GetSize());

			SceneNodeCtrl::DragType dargType = mSceneNodeCtrl->GetDragType();
			if (SceneNodeCtrl::DT_NONE == dargType)
			{
				_ClickSelectScene(viewPortPos);
			}
		}

		mURDoCommand = new0 MovableTransURDo();
	}
	else if (Edit::ET_TERRAIN == et)
	{
		PX2_EDIT.GetTerrainEdit()->GetBrush()->SelectPage();
		PX2_EDIT.GetTerrainEdit()->Apply(true);

		APoint viewPortPos = WorldPosToViewPortPos(worldPos);
		_UpdateBrushPos(viewPortPos);
	}
}
//----------------------------------------------------------------------------
void EU_CanvasStage::_ClickSelectPos(const APoint &scrPos)
{
	if (!Project::GetSingletonPtr()) return;

	Scene *scene = PX2_PROJ.GetScene();
	if (!scene) return;

	Camera *camera = mStageCameraNode->GetCamera();

	APoint origin;
	AVector direction;
	camera->GetPickRay(scrPos.X(), scrPos.Z(), GetSize(), origin, direction);

	Picker picker;
	picker.Execute(scene, origin, direction, 0.0f, Mathf::MAX_REAL);
	if ((int)picker.Records.size() > 0)
	{
		const PickRecord &record = picker.GetClosestToZero();
		mSelectPoint = origin + direction*record.T;
		PX2_EDIT.SetPickPos(mSelectPoint);
	}
	else
	{
		PX2::TriMesh *xyPlane = PX2_GR.GetXYPlane();
		xyPlane->WorldTransform.SetTranslate(APoint::ORIGIN);

		Picker picker;
		picker.Execute(xyPlane, origin, direction, 0.0f, Mathf::MAX_REAL);
		if ((int)picker.Records.size() > 0)
		{
			const PickRecord &rec = picker.GetClosestToZero();
			mSelectPoint = origin + direction*rec.T;
			PX2_EDIT.SetPickPos(mSelectPoint);
		}
	}
}
//----------------------------------------------------------------------------
void EU_CanvasStage::_ClickSelectScene(const APoint &scrPos)
{
	if (!Project::GetSingletonPtr()) return;

	Scene *scene = PX2_PROJ.GetScene();
	if (!scene) return;

	SelectMode mode = SM_SINGLE;
	if (PX2_EDIT.IsCtrlDown) mode = SM_MULTI;
	
	Camera *camera = mStageCameraNode->GetCamera();

	APoint origin;
	AVector direction;
	camera->GetPickRay(scrPos.X(), scrPos.Z(), GetSize(), origin, direction);

	Picker picker;
	picker.Execute(scene, origin, direction, 0.0f, Mathf::MAX_REAL);
	if ((int)picker.Records.size() > 0)
	{
		const PickRecord &record = picker.GetClosestToZero();
		Object *recordObj = record.Intersected;

		mSelectPoint = origin + direction*record.T;
		PX2_EDIT.SetPickPos(mSelectPoint);

		if (SM_SINGLE == mode)
		{
			int numObjs = PX2_SELECTM_E->GetNumObjects();
			if (1 == numObjs && recordObj == PX2_SELECTM_E->GetFirstObject())
			{
				PX2_SELECTM_E->Clear();
			}
			else
			{
				PX2_SELECTM_E->Clear();
				PX2_SELECTM_E->AddObject(recordObj);
			}
		}
		else if (SM_MULTI == mode)
		{
			if (PX2_SELECTM_E->IsObjectIn(recordObj))
			{
				PX2_SELECTM_E->RemoveObject(recordObj);
			}
			else
			{
				PX2_SELECTM_E->AddObject(recordObj);
			}
		}
	}
	else
	{
		if (SM_SINGLE == mode)
		{
			PX2_SELECTM_E->Clear();
		}
	}
}
//----------------------------------------------------------------------------
void EU_CanvasStage::OnLeftUp(const APoint &worldPos)
{
	if (!IsEnable())
		return;

	const Rectf &rectWorld = GetWorldRect();
	if (!rectWorld.IsInsize(worldPos.X(), worldPos.Z()))
		return;

	UICanvas::OnLeftUp(worldPos);

	if (IsCameraDraging())
	{
		SetCameraDraging(false);
	}
	else
	{
		Camera *camera = mSceneNodeCtrlCanvas->GetOverCamera();

		if (mSceneNodeCtrl)
			mSceneNodeCtrl->OnLeftUp(camera, worldPos, GetSize());

		Edit::EditMode em = PX2_EDIT.GetEditMode();
		if ((Edit::EM_TRANSLATE == em || Edit::EM_ROTATE == em
			|| Edit::EM_SCALE == em) && mIsSelectMovableTransChanged)
		{
			PX2_URDOM.PushUnDo(mURDoCommand);
			mIsSelectMovableTransChanged = false;
		}
	}
}
//----------------------------------------------------------------------------
void EU_CanvasStage::OnLeftDClick(const APoint &worldPos)
{
	UICanvas::OnLeftDClick(worldPos);
}
//----------------------------------------------------------------------------
void EU_CanvasStage::OnMiddleDown(const APoint &worldPos)
{
	UICanvas::OnMiddleDown(worldPos);
}
//----------------------------------------------------------------------------
void EU_CanvasStage::OnMiddleUp(const APoint &worldPos)
{
	UICanvas::OnMiddleUp(worldPos);

	if (IsCameraDraging())
	{
		SetCameraDraging(false);
	}
}
//----------------------------------------------------------------------------
void EU_CanvasStage::OnMouseWheel(const APoint &worldPos, float delta)
{
	if (!IsEnable())
		return;

	const Rectf &rectWorld = GetWorldRect();
	if (!rectWorld.IsInsize(worldPos.X(), worldPos.Z()))
		return;

	UICanvas::OnMouseWheel(worldPos, delta);

	float delta1 = delta * 0.003f * PX2_EDIT.GetCameraMoveSpeed();
	_ZoomCamera(delta1);

	if (mSceneNodeCtrl)
	{
		Camera *camera = mSceneNodeCtrlCanvas->GetOverCamera();
		mSceneNodeCtrl->OnMouseWheel(camera, delta1, GetSize());
	}
}
//----------------------------------------------------------------------------
void EU_CanvasStage::OnRightDown(const APoint &worldPos)
{
	if (!IsEnable())
		return;

	const Rectf &rectWorld = GetWorldRect();
	if (!rectWorld.IsInsize(worldPos.X(), worldPos.Z()))
		return;

	UICanvas::OnRightDown(worldPos);

	APoint viewPortPos = WorldPosToViewPortPos(worldPos);
	_ClickSelectPos(viewPortPos);
}
//----------------------------------------------------------------------------
void EU_CanvasStage::OnRightUp(const APoint &worldPos)
{
	if (!IsEnable())
		return;

	const Rectf &rectWorld = GetWorldRect();
	if (!rectWorld.IsInsize(worldPos.X(), worldPos.Z()))
		return;

	Project *proj = Project::GetSingletonPtr();
	if (!proj)
		return;

	if (IsCameraDraging())
	{
		SetCameraDraging(false);
	}
	else
	{
		if (mIsRightPressed)
		{
			CreateMenuOnSelect(worldPos);
		}
	}

	UICanvas::OnRightUp(worldPos);
}
//----------------------------------------------------------------------------
void EU_CanvasStage::OnMotion(const APoint &worldPos)
{
	if (!IsEnable())
		return;

	const Rectf &rectWorld = GetWorldRect();
	if (!rectWorld.IsInsize(worldPos.X(), worldPos.Z()))
		return;

	UICanvas::OnMotion(worldPos);

	if (!Project::GetSingletonPtr()) return;
	if (!PX2_PROJ.GetScene()) return;

	if (mMoveDelta == AVector::ZERO) return;

	Edit::EditType et = PX2_EDIT.GetEditType();
	if (et == Edit::ET_TERRAIN)
	{
		APoint viewPortPos = WorldPosToViewPortPos(worldPos);
		_UpdateBrushPos(viewPortPos);
	}

	APoint curViewPortPos = WorldPosToViewPortPos(mCurPickPos);
	APoint lastViewPortPos = WorldPosToViewPortPos(mLastPickPos);

	AVector delta = mMoveDelta;
	float speedVal = 0.01f;

	SceneNodeCtrl::DragType dt = mSceneNodeCtrl->GetDragType();
	if (dt == SceneNodeCtrl::DT_NONE)
	{
		if (AVector::ZERO != delta && (mIsRightPressed || mIsMiddlePressed))
		{
			SetCameraDraging(true);
		}
	}
	else
	{
		SetCameraDraging(false);
	}

	if (IsCameraDraging())
	{
		if (VT_PERSPECTIVE == mViewType)
		{
			if (PX2_EDIT.IsAltDown)
			{
				if (mIsLeftPressed)
				{
					Movable *mov = DynamicCast<Movable>(PX2_SELECTM_E->GetFirstObject());
					if (mov)
					{
						_RoundCamera(delta.X()*speedVal*0.02f, -delta.Z()*speedVal*0.02f);
					}
					else
					{
						_PanCamera(delta.X()*mPixelToWorld.first*speedVal, delta.Z()*mPixelToWorld.second*speedVal);
					}
				}
				else if (mIsRightPressed)
				{
					_ZoomCamera(-delta.Z()*mPixelToWorld.second*PX2_EDIT.GetCameraMoveSpeed()*speedVal);
				}
			}
			else
			{
				if (mIsRightPressed)
				{
					_RolateCamera(delta.X()*mPixelToWorld.first*speedVal*0.25f, -delta.Z()*mPixelToWorld.second*speedVal);
				}
				else if (mIsMiddlePressed)
				{
					_PanCamera(-delta.X()*mPixelToWorld.first*speedVal, delta.Z()*mPixelToWorld.second*speedVal);
				}
			}
		}
		else
		{
			_PanCamera(-delta.X()*mPixelToWorld.first, delta.Z()*mPixelToWorld.second*speedVal);
		}
	}
	else
	{
		if (mIsLeftPressed)
		{
			PX2_EDIT.GetTerrainEdit()->Apply(false);
		}

		Edit::EditMode em = PX2_EDIT.GetEditMode();
		if ((Edit::EM_TRANSLATE == em || Edit::EM_ROTATE == em || Edit::EM_SCALE == em))
		{
			Camera *camera = mSceneNodeCtrlCanvas->GetOverCamera();

			if (mSceneNodeCtrl)
				mSceneNodeCtrl->OnMotion(mIsLeftPressed, camera, 
				curViewPortPos, lastViewPortPos, GetSize());

			if (mIsLeftPressed)
			{
				mIsSelectMovableTransChanged = true;
			}
		}
	}
}
//----------------------------------------------------------------------------
void EU_CanvasStage::SetCameraDraging(bool drag)
{
	mIsCameraDraging = drag;

	UIAuiBlockFrame *blockFrame = GetFirstParentDerivedFromType<UIAuiBlockFrame>();

	Canvas *topestCanvas = DynamicCast<Canvas>(GetTopestParent());
	if (topestCanvas)
	{
		RenderWindow *rw = topestCanvas->GetRenderWindow();
		if (rw)
		{
				if (mIsCameraDraging)
				{
					SetPickOnlyInSizeRange(false);

					rw->CaptureMouse(true);
					mStartDragingCursorPos = mCurPickPos;

					if (blockFrame)
					{
						blockFrame->Caputure(true);
					}
				}
				else if (!mIsCameraDraging)
				{
					SetPickOnlyInSizeRange(true);

					rw->CaptureMouse(false);

					if (blockFrame)
					{
						blockFrame->Caputure(false);
					}
				}
		}
	}
}
//----------------------------------------------------------------------------
bool EU_CanvasStage::IsCameraDraging() const
{
	return mIsCameraDraging;
}
//----------------------------------------------------------------------------
std::pair<float, float> EU_CanvasStage::CalPixelToWorld()
{
	Rectf viewPort = mViewPort;
	if (viewPort.IsEmpty())
		viewPort = Rectf(0.0f, 0.0f, mSize.Width, mSize.Height);

	std::pair<float, float> pixelToWorld;

	if (mStageCameraNode)
	{
		Camera *camera = mStageCameraNode->GetCamera();

		float rMin = camera->GetRMin();
		float uMin = camera->GetUMin();
		float viewPortWidth = viewPort.Width();
		float viewPortHeight = viewPort.Height();

		float worldW = 2.0f * -rMin;
		float worldH = 2.0f * -uMin;
		
		worldW *= 1000.0f;
		worldH *= 1000.0f;

		pixelToWorld.first = worldW / (float)viewPortWidth;
		pixelToWorld.second = worldH / (float)viewPortHeight;
	}

	mPixelToWorld = pixelToWorld;

	return mPixelToWorld;
}
//----------------------------------------------------------------------------
void EU_CanvasStage::_ProcessKeyMove(bool isPress, float cameraMoveSpeed,
	float elapsedTime, float &speed)
{
	float speedParam0 = (cameraMoveSpeed / 0.2f);
	float speedParam1 = (cameraMoveSpeed / 0.1f);

	if (isPress)
	{
		speed = cameraMoveSpeed;
	}
	else
	{
		float curSpeedTemp = speed;
		curSpeedTemp -= (float)elapsedTime * (speedParam0 + speedParam1*(float)elapsedTime);
		if (curSpeedTemp >= 0.0f)
		{
			speed = curSpeedTemp;
		}
		else
		{
			speed = 0.0f;
		}
	}
}
//----------------------------------------------------------------------------
void EU_CanvasStage::_MoveCamera(float horz, float vert)
{
	if (!Project::GetSingletonPtr()) return;

	Scene *scene = PX2_PROJ.GetScene();
	if (!scene) return;

	if (mStageCameraNode)
	{
		APoint position = mStageCameraNode->LocalTransform.GetTranslate();
		AVector rVector;
		AVector dVector;
		AVector uVector;
		mStageCameraNode->LocalTransform.GetRDUVector(rVector, dVector, uVector);

		if (mViewType == VT_PERSPECTIVE)
		{
			dVector.Z() = 0.0f;
			dVector.Normalize();
			rVector.Z() = 0.0f;
			rVector.Normalize();
			position += dVector * vert;
			position -= rVector * horz;
		}
		else if (mViewType == VT_TOP)
		{
			position.Y() += vert * 1.0f;
			position.X() -= horz * 1.0f;
		}
		else if (mViewType == VT_LEFT)
		{
			position.Z() += vert * 1.0f;
			position.Y() += horz * 1.0f;
		}
		else if (mViewType == VT_FRONT)
		{
			position.Z() += vert * 1.0f;
			position.X() -= horz * 1.0f;
		}

		mStageCameraNode->LocalTransform.SetTranslate(position);
	}
}
//----------------------------------------------------------------------------
void EU_CanvasStage::_PanCamera(const float &horz, const float &vert)
{
	if (!Project::GetSingletonPtr()) return;

	Scene *scene = PX2_PROJ.GetScene();
	if (!scene) return;

	if (mStageCameraNode)
	{
		APoint position = mStageCameraNode->LocalTransform.GetTranslate();
		AVector rVector;
		AVector dVector;
		AVector uVector;
		mStageCameraNode->LocalTransform.GetRDUVector(rVector, dVector, uVector);

		if (mViewType == VT_PERSPECTIVE)
		{
			rVector.Normalize();
			position += rVector * horz * 5.0f;

			uVector.Normalize();
			position -= uVector * vert * 5.0f;
		}
		else if (mViewType == VT_TOP)
		{
			position.Y() += vert * 1.0f;
			position.X() -= horz * 1.0f;
		}
		else if (mViewType == VT_LEFT)
		{
			position.Z() += vert * 1.0f;
			position.Y() += horz * 1.0f;
		}
		else if (mViewType == VT_FRONT)
		{
			position.Z() += vert * 1.0f;
			position.X() -= horz * 1.0f;
		}
		mStageCameraNode->LocalTransform.SetTranslate(position);
	}
}
//----------------------------------------------------------------------------
void EU_CanvasStage::_ZoomCamera(float zoom)
{
	if (!Project::GetSingletonPtr()) return;

	Scene *scene = PX2_PROJ.GetScene();
	if (!scene) return;

	const Sizef &size = GetSize();

	if (mStageCameraNode)
	{
		if (VT_PERSPECTIVE == mViewType)
		{
			APoint position = mStageCameraNode->LocalTransform.GetTranslate();
			AVector rVector;
			AVector dVector;
			AVector uVector;
			mStageCameraNode->LocalTransform.GetRDUVector(rVector, dVector, uVector);
			dVector.Normalize();

			position += dVector*zoom;
			mStageCameraNode->LocalTransform.SetTranslate(position);
		}
		else if (VT_TOP == mViewType)
		{
			float rMin = 0.0f;
			float rMax = 0.0f;
			float uMin = 0.0f;
			float uMax = 0.0f;
			float dMin = 0.0f;
			float dMax = 0.0f;
			float orthoFrustumMin = 1.0f;
			mStageCameraNode->GetCamera()->GetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);
			uMin += (zoom * 1.0f);
			if (uMin > -orthoFrustumMin)
				uMin = -orthoFrustumMin;
			uMax = -uMin;
			rMin = uMin * size.Width / size.Height;
			rMax = -rMin;

			mStageCameraNode->GetCamera()->SetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);
		}
		else if (VT_FRONT == mViewType)
		{
			float rMin = 0.0f;
			float rMax = 0.0f;
			float uMin = 0.0f;
			float uMax = 0.0f;
			float dMin = 0.0f;
			float dMax = 0.0f;
			float orthoFrustumMin = 1.0f;
			mStageCameraNode->GetCamera()->GetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);
			uMin += (zoom * 1.0f);
			if (uMin > -orthoFrustumMin)
				uMin = -orthoFrustumMin;
			uMax = -uMin;
			rMin = uMin * size.Width / size.Height;
			rMax = -rMin;

			mStageCameraNode->GetCamera()->SetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);
		}
	}
}
//----------------------------------------------------------------------------
void EU_CanvasStage::_RolateCamera(float horz, float vert)
{
	Scene *scene = PX2_PROJ.GetScene();

	horz *= 0.4f;
	vert *= 0.25f;

	if (VT_PERSPECTIVE == mViewType)
	{
		AVector rVector;
		AVector dVector;
		AVector uVector;
		mStageCameraNode->LocalTransform.GetRDUVector(rVector, dVector, uVector);

		// horz
		HMatrix incrH(AVector::UNIT_Z, -horz);
		dVector = incrH * dVector;
		uVector = incrH * uVector;
		rVector = incrH * rVector;

		// vert
		Matrix3f kIncrV(rVector, -vert);
		dVector = kIncrV * dVector;
		uVector = kIncrV * uVector;

		dVector.Normalize();
		float dVectorAdj = dVector.Dot(AVector::UNIT_Z);
		float dVectorAdj1 = dVector.Dot(-AVector::UNIT_Z);
		if (dVectorAdj > 0.9f || dVectorAdj1 > 0.9f)
			return;

		AVector::Orthonormalize(dVector, uVector, rVector);
		mStageCameraNode->LocalTransform.SetRotate(HMatrix(rVector, dVector,
			uVector, AVector::ZERO, true));
	}
}
//----------------------------------------------------------------------------
void EU_CanvasStage::_RoundCamera(float horz, float vert)
{
	if (!Project::GetSingletonPtr()) return;

	Scene *scene = PX2_PROJ.GetScene();
	if (!scene) return;

	if (mViewType == VT_PERSPECTIVE)
	{
		PX2::Object *obj = PX2_SELECTM_E->GetFirstObject();

		bool hasTarget = false;
		APoint pos;
		Movable *mov = DynamicCast<Movable>(obj);
		if (mov)
		{
			pos = mov->WorldTransform.GetTranslate();
			hasTarget = true;
		}

		if (hasTarget)
		{
			const APoint &camPos = mStageCameraNode->LocalTransform.GetTranslate();
			AVector rVector;
			AVector dVector;
			AVector uVector;
			mStageCameraNode->LocalTransform.GetRDUVector(rVector, dVector, uVector);

			AVector targetDir = pos - camPos;
			float targetLength = targetDir.Normalize();

			// horz
			HMatrix incrH(AVector::UNIT_Z, -horz*0.1f);
			targetDir = incrH * targetDir;
			dVector = incrH * dVector;
			uVector = incrH * uVector;
			rVector = incrH * rVector;

			HMatrix incrV(rVector, -vert*0.1f);
			targetDir = incrV * targetDir;
			dVector = incrV * dVector;
			uVector = incrV * uVector;

			APoint newPos = pos - targetDir*targetLength;
			mStageCameraNode->LocalTransform.SetTranslate(newPos);

			AVector::Orthonormalize(dVector, uVector, rVector);
			mStageCameraNode->LocalTransform.SetRotate(
				HMatrix(rVector, dVector, uVector, AVector::ZERO, true));
		}
	}
}
//----------------------------------------------------------------------------
void EU_CanvasStage::_UpdateBrushPos(const APoint &scrPos)
{
	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	Scene *scene = proj->GetScene();
	if (!scene) return;

	Terrain *terrain = scene->GetTerrain();
	if (!terrain) return;

	Movable *pickObject = terrain;

#ifdef _DEBUG
	pickObject = PX2_GR.GetXYPlane();
	pickObject->WorldTransform.SetTranslate(APoint::ORIGIN);
#endif

	Camera *camera = mStageCameraNode->GetCamera();
	APoint origin;
	AVector direction;
	camera->GetPickRay(scrPos.X(), scrPos.Z(), GetSize(), origin, direction);

	Picker picker;
	picker.Execute(pickObject, origin, direction, 0.0f, Mathf::MAX_REAL);
	if ((int)picker.Records.size() > 0)
	{
		PX2::PickRecord pickRecord = picker.GetClosestToZero();
		PX2::APoint dest = origin + direction*pickRecord.T;

		PX2_EDIT.GetTerrainEdit()->GetBrush()->SetPos(dest);
	}
}
//----------------------------------------------------------------------------
void EU_CanvasStage::CreateMenuOnSelect(const APoint &pos)
{
	PX2EU_MAN.CreateEditMenu(pos, EU_Manager::EMT_SCENE);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_CanvasStage::EU_CanvasStage(LoadConstructor value) :
UICanvas(value)
{
}
//----------------------------------------------------------------------------
void EU_CanvasStage::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UICanvas::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_CanvasStage, source);
}
//----------------------------------------------------------------------------
void EU_CanvasStage::Link(InStream& source)
{
	UICanvas::Link(source);
}
//----------------------------------------------------------------------------
void EU_CanvasStage::PostLink()
{
	UICanvas::PostLink();
}
//----------------------------------------------------------------------------
bool EU_CanvasStage::Register(OutStream& target) const
{
	if (UICanvas::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_CanvasStage::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UICanvas::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_CanvasStage, target);
}
//----------------------------------------------------------------------------
int EU_CanvasStage::GetStreamingSize(Stream &stream) const
{
	int size = UICanvas::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------