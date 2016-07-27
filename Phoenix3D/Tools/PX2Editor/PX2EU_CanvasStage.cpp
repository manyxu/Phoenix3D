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

	mStageCameraNode = PX2_CREATER.CreateNode_Camera();
	mStageCameraNode->SetName("MainCameraNode");
	mStageCameraNode->LocalTransform.SetTranslate(-20.0f, -20.0f, 10.0f);
	mStageCameraNode->LookAt(APoint(0.0f, 0.0f, 0.0f));
	mStageCameraNode->GetCamera()->SetClearFlag(false, false, false);

	AttachChild(mStageCameraNode);

	_CreateGridGeometry();
	_CreateSceneCanvas();
	_CreateNodeCtrl();
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

	mGridCanvas->Show(enable);
	mSceneCanvas->Show(enable);
	mSceneNodeCtrlCanvas->Show(enable);
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
		//Edit::EditType editType = PX2_EDIT.GetEditType();
		//Enable(editType == Edit::ET_SCENE);

		//mSceneNodeCtrl->GetCtrlsGroup()->Show(editType == Edit::ET_SCENE);
		//mBoundCtrl->GetCtrlsGroup()->Show(editType == Edit::ET_SCENE);
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

	mSceneNodeCtrl->UpdateCtrlTrans();
	mBoundCtrl->UpdateCtrl();
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
	mGridCanvas->AttachChild(mGridNode);
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

	mSceneCanvas->AttachController(new0 EU_ProjectEventController());
}
//----------------------------------------------------------------------------
void EU_CanvasStage::_CreateNodeCtrl()
{
	mSceneNodeCtrl = new0 SceneNodeCtrl();
	PX2_EW.ComeIn(mSceneNodeCtrl);

	mBoundCtrl = new0 BoundCtrl();
	PX2_EW.ComeIn(mBoundCtrl);

	mSceneCtrlNode = new0 Node();
	mSceneCtrlNode->AttachChild(mSceneNodeCtrl->GetCtrlsGroup());
	mSceneCtrlNode->AttachChild(mBoundCtrl->GetCtrlsGroup());
	mSceneCtrlNode->Update(Time::GetTimeInSeconds(), true);

	mSceneCtrlNode->AttachChild(PX2_EDIT.GetHelpNode());

	mSceneNodeCtrlCanvas = new0 UICanvas();
	AttachChild(mSceneNodeCtrlCanvas);
	mSceneNodeCtrlCanvas->SetName("SceneCtrlNodeCanvas");
	mSceneNodeCtrlCanvas->SetAnchorHor(0.0f, 1.0f);
	mSceneNodeCtrlCanvas->SetAnchorParamHor(0.0f, 0.0f);
	mSceneNodeCtrlCanvas->SetAnchorVer(0.0f, 1.0f);
	mSceneNodeCtrlCanvas->SetAnchorParamVer(0.0f, 1.0f);
	mSceneNodeCtrlCanvas->AttachChild(mSceneCtrlNode);
	mSceneNodeCtrlCanvas->SetClearFlag(false, true, true);
}
//----------------------------------------------------------------------------
void EU_CanvasStage::OnLeftDown(const APoint &worldPos)
{
	const Rectf &rectWorld = GetWorldRect();
	if (!rectWorld.IsInsize(worldPos.X(), worldPos.Z()))
		return;

	UICanvas::OnLeftDown(worldPos);

	mIsSelectMovableTransChanged = false;
	mURDoCommand = 0;

	Camera *camera = mSceneNodeCtrlCanvas->GetOverCamera();

	APoint viewPortPos = WorldPosToViewPortPos(worldPos);

	_ClickSelectPos(viewPortPos);

	Edit::EditType et = PX2_EDIT.GetEditType();
	if (Edit::ET_SCENE == et)
	{
		if (mSceneNodeCtrl)
		{
			mSceneNodeCtrl->OnLeftDown(camera, viewPortPos, GetSize());

			SceneNodeCtrl::DragType dargType = mSceneNodeCtrl->GetDragType();
			if (SceneNodeCtrl::DT_NONE == dargType)
			{
				_ClickSelectScene(viewPortPos);
			}
		}

		mURDoCommand = new0 MovableTransURDo();
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
	const Rectf &rectWorld = GetWorldRect();
	if (!rectWorld.IsInsize(worldPos.X(), worldPos.Z()))
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
	const Rectf &rectWorld = GetWorldRect();
	if (!rectWorld.IsInsize(worldPos.X(), worldPos.Z()))
		return;

	UICanvas::OnMotion(worldPos);

	if (!Project::GetSingletonPtr()) return;
	if (!PX2_PROJ.GetScene()) return;

	if (mMoveDelta == AVector::ZERO) return;

	APoint curViewPortPos = WorldPosToViewPortPos(mCurPickPos);
	APoint lastViewPortPos = WorldPosToViewPortPos(mLastPickPos);

	AVector delta = mMoveDelta;
	float speedVal = 0.01f;

	SceneNodeCtrl::DragType dt = mSceneNodeCtrl->GetDragType();
	if (dt == SceneNodeCtrl::DT_NONE)
	{
		if (AVector::ZERO != delta && (mIsLeftPressed || mIsRightPressed || mIsMiddlePressed))
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
		APoint position = mStageCameraNode->LocalTransform.GetTranslate();
		AVector rVector;
		AVector dVector;
		AVector uVector;
		mStageCameraNode->LocalTransform.GetRDUVector(rVector, dVector, uVector);
		dVector.Normalize();

		if (VT_PERSPECTIVE == mViewType)
		{
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
void EU_CanvasStage::CreateMenuOnSelect(const APoint &pos)
{
	Object *obj = PX2_SELECTM_E->GetFirstObject();
	Movable *movable = DynamicCast<Movable>(obj);
	Node *node = DynamicCast<Node>(obj);

	PX2EU_MAN.Menu_Edit_Begin("Stage", "Edit");

	if (node)
	{
		// add create menu
		PX2EU_MAN.Menu_Edit_AddSubItem("Stage", "Edit", "Create", PX2_LMVAL("Create"));

		PX2EU_MAN.Menu_Edit_AddSubItem("Stage", "EditCreate", "Geometry", PX2_LMVAL("Geometry"));
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreateGeometry", "Plane", PX2_LMVAL("Plane"), "n_Create_Geometry_Plane()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreateGeometry", "Box", PX2_LMVAL("Box"), "n_Create_Geometry_Box()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreateGeometry", "Sphere", PX2_LMVAL("Sphere"), "n_Create_Geometry_Sphere()");

		PX2EU_MAN.Menu_Edit_AddSubItem("Stage", "EditCreate", "Effect", PX2_LMVAL("Effect"));
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreateEffect", "Billboard", PX2_LMVAL("Billboard"), "n_Create_Effect_Billboard()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreateEffect", "Particle", PX2_LMVAL("Particle"), "n_Create_Effect_Particle()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreateEffect", "Beam", PX2_LMVAL("Beam"), "n_Create_Effect_Beam()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreateEffect", "Ribbon", PX2_LMVAL("Ribbon"), "n_Create_Effect_Ribbon()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreateEffect", "Sound", PX2_LMVAL("Sound"), "n_Create_Effect_Sound()");
		PX2EU_MAN.Menu_Edit_AddItemSeparater("Stage", "EditCreate");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreate", "Node", PX2_LMVAL("Node"), "n_Create_Effect_Node()");
		PX2EU_MAN.Menu_Edit_AddItemSeparater("Stage", "EditCreate");
		PX2EU_MAN.Menu_Edit_AddSubItem("Stage", "EditCreate", "Controller", PX2_LMVAL("Controller"));
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreateController", "ColorController", PX2_LMVAL("ColorController"), "n_Create_Controller_ColorController()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreateController", "BrightnessController", PX2_LMVAL("BrightnessController"), "n_Create_Controller_BrightnessController()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreateController", "AlphaController", PX2_LMVAL("AlphaController"), "n_Create_Controller_AlphaController()");
		PX2EU_MAN.Menu_Edit_AddItemSeparater("Stage", "EditCreateController");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreateController", "UniformScaleController", PX2_LMVAL("UniformScaleController"), "n_Create_Controller_UniformScaleController()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreateController", "ScaleController", PX2_LMVAL("ScaleController"), "n_Create_Controller_ScaleController()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreateController", "RotateController", PX2_LMVAL("RotateController"), "n_Create_Controller_RotateController()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreateController", "TranslateController", PX2_LMVAL("TranslateController"), "n_Create_Controller_TranslateController()");

		PX2EU_MAN.Menu_Edit_AddItemSeparater("Stage", "Edit");
	}
	else
	{
		// add create menu
		PX2EU_MAN.Menu_Edit_AddSubItem("Stage", "Edit", "Create", PX2_LMVAL("Create"));

		PX2EU_MAN.Menu_Edit_AddSubItem("Stage", "EditCreate", "Geometry", PX2_LMVAL("Geometry"));
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreateGeometry", "Plane", PX2_LMVAL("Plane"), "n_CreateActor_Plane()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreateGeometry", "Box", PX2_LMVAL("Box"), "n_CreateActor_Box()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreateGeometry", "Sphere", PX2_LMVAL("Sphere"), "n_CreateActor_Sphere()");

		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreate", "Actor", "Actor", "n_CreateActor_Actor()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreate", "Character", "Character", "n_CreateActor_Character()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreate", "Camera", PX2_LMVAL("Camera"), "n_CreateActor_Camera()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreate", "Light", PX2_LMVAL("Light"), "n_CreateActor_Light()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreate", "Trigger", PX2_LMVAL("Trigger"), "n_CreateActor_Trigger()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreate", "Terrain", PX2_LMVAL("Terrain"), "n_CreateActor_Terrain()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "EditCreate", "Sky", PX2_LMVAL("Sky"), "n_CreateActor_Sky()");

		PX2EU_MAN.Menu_Edit_AddItemSeparater("Stage", "Edit");
	}

	if (movable)
	{
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "Edit", "Copy", PX2_LMVAL("Copy"), "n_Copy()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "Edit", "Paste", PX2_LMVAL("Paste"), "n_Paste()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "Edit", "Delete", PX2_LMVAL("Delete"), "n_DeleteSelection()");
		PX2EU_MAN.Menu_Edit_AddItemSeparater("Stage", "Edit");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "Edit", "ResetPlay", PX2_LMVAL("ResetPlay"), "e_AnimResetPlay()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "Edit", "Play", PX2_LMVAL("Play"), "e_AnimPlayStop()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "Edit", "Pause", PX2_LMVAL("Pause"), "e_AnimStop()");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "Edit", "Reset", PX2_LMVAL("Reset"), "e_AnimReset()");
		PX2EU_MAN.Menu_Edit_AddItemSeparater("Stage", "Edit");
	}

	// add add menu
	PX2EU_MAN.Menu_Edit_AddItem("Stage", "Edit", "ImportSelected", PX2_LMVAL("ImportSelected"), "");
	PX2EU_MAN.Menu_Edit_AddItem("Stage", "Edit", "Import", PX2_LMVAL("Import"), "");
	if (node)
	{
		PX2EU_MAN.Menu_Edit_AddItemSeparater("Stage", "Edit");
	}

	if (node)
	{
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "Edit", "FindInProjectTree", PX2_LMVAL("FindInProjectTree"), "");
		PX2EU_MAN.Menu_Edit_AddItem("Stage", "Edit", "FindInResTree", PX2_LMVAL("FindInResTree"), "");
	}

	PX2EU_MAN.Menu_Edit_EndPopUp("Stage", pos);
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