// PX2Edit.cpp

#include "PX2Edit.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2EditEventType.hpp"
#include "PX2SelectionManager.hpp"
#include "PX2Creater.hpp"
#include "PX2RedoUndo.hpp"
#include "PX2Project.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2EU_Manager.hpp"
#include "PX2EditorEventType.hpp"
#include "PX2Animation.hpp"
#include "PX2EditorToLua.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2LuaPlusContext.hpp"
#include "PX2InputEvent.hpp"
#include "PX2InputEventData.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
int Edit::msEditorID = 6001;
//----------------------------------------------------------------------------
Edit::Edit() :
mEditType(ET_SCENE),
mEditAxisMode(EAM_WORLD),
mEditMode(EM_NONE),
mGeoObjFactory(0),
mEditParams(0),
IsAltDown(false),
IsCtrlDown(false),
IsShiftDown(false),
IsKeyDown_W(false),
IsKeyDown_S(false),
IsKeyDown_A(false),
IsKeyDown_D(false),
IsLeftMouseDown(false),
IsRightMouseDown(false),
IsMidMouseDown(false),
mCameraMoveLevel(1),
mCameraMoveSpeedBase(20.0f),
mEU_Man(0)
{
	mTimeLineEidt = new0 TimeLineEdit();
	mTerrainEdit = new0 TerrainEdit();

	mHelpNode = new0 Node();
	mHelpNode->AttachChild(mTerrainEdit->GetBrush()->GetRenderable());

	LuaPlusContext *lpc = (LuaPlusContext*)PX2_SC_LUA;
	lua_State *luaState = (lua_State*)lpc->GetLuaState();

	luaopen_PX2Editor(luaState);

	PX2_SC_LUA->SetUserTypePointer("PX2_EDIT", "Edit", Edit::GetSingletonPtr());

	PX2_EW.ComeIn(this);
}
//----------------------------------------------------------------------------
Edit::~Edit()
{
	PX2_EW.GoOut(this);

	delete0(mTimeLineEidt);
	delete0(mTerrainEdit);

	PX2_EW.GoOut(mEditEventHandler);
	mEditEventHandler = 0;
}
//----------------------------------------------------------------------------
bool Edit::Initlize()
{
	mGeoObjFactory = new0 GeoObjFactory();
	mEditParams = new0 EditParams();

	PX2_GR.SetInEditor(true);
	mEU_Man = new0 EU_Manager();
	mEU_Man->Initlize();

	mEditEventHandler = new0 EditEventHandler();
	PX2_EW.ComeIn(mEditEventHandler);
	
	return true;
}
//----------------------------------------------------------------------------
bool Edit::LoadEditorTheme()
{
	bool loaded = mEditParams->Load("DataEditor/config/editconfig.xml");
	mEditParams->SetCurTheme(mEditParams->GetCurThemeTypeStr());

	return loaded;
}
//----------------------------------------------------------------------------
bool Edit::Terminate()
{
	mEditCameraNode = 0;

	mCopyObject = 0;

	mEditEventHandler = 0;

	if (mEditParams)
		mEditParams->Save("DataEditor/config/editconfig.xml");

	if (mEU_Man)
	{
		mEU_Man->Terminate();
		delete0(mEU_Man);
	}

	if (mGeoObjFactory)
	{
		delete0(mGeoObjFactory);
	}

	mEditParams = 0;
	
	return true;
}
//----------------------------------------------------------------------------
void Edit::Reset()
{
	mCopyObject = 0;
	mSelectPath_ChildFilenames.clear();
	mSelectPath_ChildPaths.clear();

	SetEditType(ET_SCENE);
	SetEditMode(EM_NONE);
}
//----------------------------------------------------------------------------
void Edit::SetEditType(EditType type)
{
	mEditType = type;

	Event *ent = EditorEventSpace::CreateEventX(
		EditorEventSpace::SetEditType);
	EventWorld::GetSingleton().BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void Edit::SetEditAxisMode(EditAxisMode mode)
{
	mEditAxisMode = mode;

	Event *ent = EditorEventSpace::CreateEventX(
		EditorEventSpace::SetEditAxisMode);
	EventWorld::GetSingleton().BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void Edit::SetEditMode(EditMode mode)
{
	mEditMode = mode;

	Event *ent = EditorEventSpace::CreateEventX(EditorEventSpace::SetEditMode);
	EventWorld::GetSingleton().BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void Edit::SetEditorCameraNode(CameraNode *editCameraNode)
{
	mEditCameraNode = editCameraNode;
}
//----------------------------------------------------------------------------
CameraNode *Edit::GetEditorCameraNode()
{
	return mEditCameraNode;
}
//----------------------------------------------------------------------------
int Edit::GetEditID()
{
	return msEditorID++;
}
//----------------------------------------------------------------------------
void Edit::SetPickPos(const APoint &pos)
{
	mPickPos = pos;

	Event *ent = EditorEventSpace::CreateEventX(EditorEventSpace::SetPickPos);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
APoint Edit::GetPickLocalPos(Movable *nodeParent)
{
	Transform inversTrans = nodeParent->WorldTransform.InverseTransform();
	APoint localPos = inversTrans * mPickPos;
	return localPos;
}
//----------------------------------------------------------------------------
void Edit::FocusOnSelection()
{
	APoint pos;
	Bound bound;
	
	int numMovable = 0;
	int numObjscts = PX2_SELECTM_E->GetNumObjects();
	if (numObjscts > 0)
	{
		int firstBound = true;

		for (int i = 0; i < numObjscts; i++)
		{
			Object *obj = PX2_SELECTM_E->GetObjectAt(i);
			Movable *movable = DynamicCast<Movable>(obj);
			CameraNode *camNode = DynamicCast<CameraNode>(obj);

			if (movable)
			{
				pos += movable->WorldTransform.GetTranslate();

				if (0.0f != movable->WorldBound.GetRadius())
				{
					if (firstBound)
					{
						bound = movable->WorldBound;
						firstBound = false;
					}
					else
					{
						bound.GrowToContain(movable->WorldBound);
					}
				}

				numMovable++;
			}
		}

		pos = pos / (float)numObjscts;
	}

	float radius = bound.GetRadius();
	if (0.0f == radius)
	{
		radius = 10.0f;
	}

	if (0==numMovable)
		return;

	CameraNode *editorCameraNode = PX2_EDIT.GetEditorCameraNode();
	const APoint &curPos = editorCameraNode->WorldTransform.GetTranslate();
	AVector dir = editorCameraNode->GetCamera()->GetDVector();
	dir.Normalize();
	float upFovDegrees = 0.0f;
	float aspectRatio = 0.0f;
	float dMin = 0.0f;
	float dMax = 0.0f;
	editorCameraNode->GetCamera()->GetFrustum(upFovDegrees, aspectRatio, dMin, dMax);
	APoint toPos = pos;
	toPos -= dir * (radius + dMin * 10.0f);

	InterpCurveTranslateController *transCtrl = 
		DynamicCast<InterpCurveTranslateController>(
		editorCameraNode->GetControllerByName("Camera_ICTC"));
	if (transCtrl)
	{
		transCtrl->SetWorld(true);
		transCtrl->mValues.Reset();
		transCtrl->mValues.AddPoint(0.0f, curPos, Float3::ZERO, Float3::ZERO,
			ICM_CURVE_AUTO);
		transCtrl->mValues.AddPoint(0.5f, toPos, Float3::ZERO, Float3::ZERO, 
			ICM_CURVE_AUTO);
		transCtrl->MaxTime = 0.5f;
		transCtrl->SetMaxPlayTime(0.5f);
		transCtrl->ResetPlay();
	}	
}
//----------------------------------------------------------------------------
bool Edit::DeleteSelection()
{
	bool deleted = false;

	int numObjs = PX2_SELECTM_E->GetNumObjects();
	for (int i = 0; i < numObjs; i++)
	{
		Object *obj = PX2_SELECTM_E->GetObjectAt(i);
		if (PX2_CREATER.RemoveObject(obj))
		{
			deleted = true;
		}
	}

	if (deleted)
	{
		PX2_SELECTM_E->Clear();
	}

	return deleted;
}
//----------------------------------------------------------------------------
void Edit::ChangeSelectResDir(const std::string &path, ChangeDirType cdt)
{
	mSelectResDir = path;

	Event *ent = EditorEventSpace::CreateEventX(
		EditorEventSpace::ChangeResDir);
	ent->SetData<Edit::ChangeDirType>(cdt);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void Edit::SetSelectedResource(const SelectResData &data)
{
	mSelectResData = data;

	Event *ent = EditorEventSpace::CreateEventX(EditorEventSpace::SelectRes);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void Edit::PasteCopyedObject()
{
	if (!mCopyObject) return;

	Movable *mov = DynamicCast<Movable>(mCopyObject);
	Node *parentNode = DynamicCast<Node>(PX2_SELECTM_E->GetFirstObject());

	if (mov && parentNode)
	{
		MovablePtr newMov = DynamicCast<Movable>(PX2_RM.ShareCopy(mov, true, true, false));	
		PX2_CREATER.AddObject(parentNode, newMov);
	}
}
//----------------------------------------------------------------------------
void Edit::CloneSelectedObject()
{
	int numObjects = PX2_SELECTM_E->GetNumObjects();
	if (1 == numObjects)
	{
		SetCopyObject(PX2_SELECTM_E->GetFirstObject());
	}
	else
	{
		SetCopyObject(0);
	}

	for (int i = 0; i < numObjects; i++)
	{
		Movable *mov = DynamicCast<Movable>(PX2_SELECTM_E->GetObjectAt(i));
		if (mov)
		{
			Node *nodeParent = DynamicCast<Node>(mov->GetParent());
			if (mov && nodeParent)
			{
				MovablePtr newMov = DynamicCast<Movable>(PX2_RM.ShareCopy(mov, false, false, false));
				PX2_CREATER.AddObject(nodeParent, newMov);
			}
		}
	}
}
//----------------------------------------------------------------------------
bool Edit::Import(const char *pathname)
{
	int numObjs = PX2_SELECTM_E->GetNumObjects();
	if (1 != numObjs) return false;

	Object *selectObj = PX2_SELECTM_E->GetFirstObject();

	InStream inStream;
	if (inStream.Load(pathname))
	{
		Object *obj = inStream.GetObjectAt(0);
		PX2_CREATER.AddObject(selectObj, obj);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool Edit::Export(PX2::Object *obj, const char *pathname)
{
	if (!obj) return false;

	Controller *ctrl = DynamicCast<Controller>(obj);
	Movable *mov = DynamicCast<Movable>(obj);

	Controlledable *ctrlAble = 0;
	Node *node = 0;

	if (ctrl)
	{
		ctrlAble = ctrl->GetControlledable();
		ctrlAble->DetachController(ctrl);
	}
	else if (mov)
	{
		node = DynamicCast<Node>(mov->GetParent());
		if (node) node->DetachChild(mov);
	}

	OutStream outStream;
	outStream.Insert(obj);
	if (outStream.Save(pathname))
	{
		if (ctrl && ctrlAble)
		{
			ctrlAble->AttachController(ctrl);
		}
		else if (mov && node)
		{
			node->AttachChild(mov);
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Edit::AnimPlay()
{
	Object *obj = PX2_SELECTM_E->GetFirstObject();
	Controller *ctrl = DynamicCast<Controller>(obj);
	Controlledable *ctrlable = DynamicCast<Controlledable>(obj);
	Animation *anim = DynamicCast<Animation>(obj);

	if (ctrl)
	{
		ctrl->Play();
	}
	else if (ctrlable)
	{
		ctrlable->Play();
	}
	else if (anim)
	{
		anim->LetCharacterPlay();
	}
}
//----------------------------------------------------------------------------
void Edit::AnimResetPlay()
{
	Object *obj = PX2_SELECTM_E->GetFirstObject();
	Controller *ctrl = DynamicCast<Controller>(obj);
	Controlledable *ctrlable = DynamicCast<Controlledable>(obj);
	Animation *anim = DynamicCast<Animation>(obj);

	if (ctrl)
	{
		ctrl->ResetPlay();
	}
	else if (ctrlable)
	{
		ctrlable->ResetPlay();
	}
	else if (anim)
	{
		anim->LetCharacterPlay();
	}
}
//----------------------------------------------------------------------------
void Edit::AnimStop()
{
	Object *obj = PX2_SELECTM_E->GetFirstObject();
	Controller *ctrl = DynamicCast<Controller>(obj);
	Controlledable *ctrlable = DynamicCast<Controlledable>(obj);
	Animation *anim = DynamicCast<Animation>(obj);

	if (ctrl)
	{
		ctrl->Stop();
	}
	else if (ctrlable)
	{
		ctrlable->Stop();
	}
	else if (anim)
	{
		anim->LetCharacterPlay();
	}
}
//----------------------------------------------------------------------------
void Edit::AnimReset()
{
	Object *obj = PX2_SELECTM_E->GetFirstObject();
	Controller *ctrl = DynamicCast<Controller>(obj);
	Controlledable *ctrlable = DynamicCast<Controlledable>(obj);

	if (ctrl)
	{
		ctrl->Reset();
	}
	else if (ctrlable)
	{
		ctrlable->Reset();
	}
}
//----------------------------------------------------------------------------
bool Edit::IsAnimPlaying()
{
	Object *obj = PX2_SELECTM_E->GetFirstObject();
	Controller *ctrl = DynamicCast<Controller>(obj);
	Controlledable *ctrlable = DynamicCast<Controlledable>(obj);

	if (ctrl)
	{
		return ctrl->IsPlaying();
	}
	else if (ctrlable)
	{
		ctrlable->IsPlaying();
	}

	return false;
}
//----------------------------------------------------------------------------
void Edit::AnimPlayStop()
{
	if (IsAnimPlaying())
		AnimStop();
	else
		AnimPlay();
}
//----------------------------------------------------------------------------
void Edit::MakeSelectTimeLineEdit()
{
	PX2::Object *obj = PX2_SELECTM_E->GetFirstObject();

	EffectModule *eftModule = DynamicCast<EffectModule>(obj);
	InterpCurveController *interpCurve = DynamicCast<InterpCurveController>(obj);

	if (eftModule || interpCurve)
	{
		if (!mTimeLineEidt->HasCurveGroup(obj))
		{
			mTimeLineEidt->CreateAddCurveGroup(obj);
		}
	}
}
//----------------------------------------------------------------------------
void Edit::TimeLine_UIGroup_Delete()
{
	UICurveGroup *group = PX2_EDIT.GetTimeLineEdit()->GetSelectedUICurveGroup();

	if (group)
	{
		PX2_EDIT.GetTimeLineEdit()->ClearGroup(group);
		PX2_EDIT.GetTimeLineEdit()->SetSelectedUICurveGroup(0);
	}
}
//----------------------------------------------------------------------------
void Edit::TimeLine_UIGroup_DeleteAll()
{
	PX2_EDIT.GetTimeLineEdit()->ClearAllGroups();
	PX2_EDIT.GetTimeLineEdit()->SetSelectedUICurveGroup(0);
}
//----------------------------------------------------------------------------
void Edit::OnFindSelectInProjTree()
{
	Object *obj = PX2_SELECTM_E->GetFirstObject();
	if (obj)
	{
		Event *ent = EditorEventSpace::CreateEventX(EditorEventSpace::FindActorInProjectTree);
		PX2_EW.BroadcastingLocalEvent(ent);
	}
}
//----------------------------------------------------------------------------
void Edit::OnFindSelectInResTree()
{
	Object *obj = PX2_SELECTM_E->GetFirstObject();
	if (obj)
	{
		Event *ent = EditorEventSpace::CreateEventX(EditorEventSpace::FindActorInResTree);
		PX2_EW.BroadcastingLocalEvent(ent);
	}
}
//----------------------------------------------------------------------------
void Edit::SetPreViewObject(Object *obj)
{
	mPreObject = obj;

	Event *ent = EditorEventSpace::CreateEventX(EditorEventSpace::SetPreViewObject);
	ent->SetData<PX2::Object*>(obj);
	EventWorld::GetSingleton().BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void Edit::CreateBPPackage()
{
	Object *obj = PX2_SELECTM_E->GetFirstObject();
	Node *parent = DynamicCast<Node>(obj);
	if (parent)
	{
		BPPackage *bpPackage = PX2_CREATER.CreateBPPackage();
		parent->AttachChild(bpPackage);
		APoint localPos = GetPickLocalPos(parent);
		localPos.Y() = -1.0f;
		bpPackage->LocalTransform.SetTranslate(localPos);
	}
}
//----------------------------------------------------------------------------
void Edit::CreateBPFile()
{
	Object *obj = PX2_SELECTM_E->GetFirstObject();
	Node *parent = DynamicCast<Node>(obj);
	if (parent)
	{
		BPFile *bpFile = PX2_CREATER.CreateBPFile();
		parent->AttachChild(bpFile);
		APoint localPos = GetPickLocalPos(parent);
		localPos.Y() = -1.0f;
		bpFile->LocalTransform.SetTranslate(localPos);
	}
}
//----------------------------------------------------------------------------
void Edit::CreateBPModule(const std::string &className, 
	const std::string &funName)
{
	Object *obj = PX2_SELECTM_E->GetFirstObject();
	Node *parent = DynamicCast<Node>(obj);
	if (parent)
	{
		if (className.empty())
		{
			if ("FunctionStart" == funName)
			{
				BPModule *bpModule = PX2_CREATER.CreateBPModuleFunctionStart();
				parent->AttachChild(bpModule);
				APoint localPos = GetPickLocalPos(parent);
				localPos.Y() = -1.0f;
				bpModule->LocalTransform.SetTranslate(localPos);
			}
			else
			{
				BPModule *bpModule = PX2_CREATER.CreateBPModuleGeneral(funName);
				parent->AttachChild(bpModule);
				APoint localPos = GetPickLocalPos(parent);
				localPos.Y() = -1.0f;
				bpModule->LocalTransform.SetTranslate(localPos);
			}
		}
		else
		{
			BPModule *bpModule = PX2_CREATER.CreateBPModule(className, funName);
			parent->AttachChild(bpModule);
			APoint localPos = GetPickLocalPos(parent);
			localPos.Y() = -1.0f;
			bpModule->LocalTransform.SetTranslate(localPos);
		}
	}
}
//----------------------------------------------------------------------------
void Edit::CreateBPEvent(const std::string &spaceName, 
	const std::string &eventName)
{
	Object *obj = PX2_SELECTM_E->GetFirstObject();
	Node *parent = DynamicCast<Node>(obj);
	if (parent)
	{
		BPModule *bpModule = PX2_CREATER.CreateBPEvent(spaceName, eventName);
		parent->AttachChild(bpModule);
		APoint localPos = GetPickLocalPos(parent);
		localPos.Y() = -1.0f;
		bpModule->LocalTransform.SetTranslate(localPos);
	}
}
//----------------------------------------------------------------------------
void Edit::CreateBPOption(const std::string &optionName)
{
	Object *obj = PX2_SELECTM_E->GetFirstObject();
	Node *parent = DynamicCast<Node>(obj);
	if (parent)
	{
		BPModule *bpModule = PX2_CREATER.CreateBPOption(optionName);
		parent->AttachChild(bpModule);
		APoint localPos = GetPickLocalPos(parent);
		localPos.Y() = -1.0f;
		bpModule->LocalTransform.SetTranslate(localPos);
	}
}
//----------------------------------------------------------------------------
void Edit::CreateBPOperator(const std::string &operatorName)
{
	Object *obj = PX2_SELECTM_E->GetFirstObject();
	Node *parent = DynamicCast<Node>(obj);
	if (parent)
	{
		BPModule *bpModule = PX2_CREATER.CreateBPOperator(operatorName);
		parent->AttachChild(bpModule);
		APoint localPos = GetPickLocalPos(parent);
		localPos.Y() = -1.0f;
		bpModule->LocalTransform.SetTranslate(localPos);
	}
}
//----------------------------------------------------------------------------
void Edit::CreateBPParam(const std::string &paramName)
{
	Object *obj = PX2_SELECTM_E->GetFirstObject();
	Node *parent = DynamicCast<Node>(obj);
	if (parent)
	{
		BPModule *bpModule = PX2_CREATER.CreateBPParam(paramName);
		parent->AttachChild(bpModule);
		APoint localPos = GetPickLocalPos(parent);
		localPos.Y() = -1.0f;
		bpModule->LocalTransform.SetTranslate(localPos);
	}
}
//----------------------------------------------------------------------------
void Edit::CompileBP()
{
	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	const std::string &projName = proj->GetName();

	Object *selectObj = PX2_SELECTM_E->GetFirstObject();
	BPPackage *bpPackage = DynamicCast<BPPackage>(selectObj);
	BPFile *bpFile = DynamicCast<BPFile>(selectObj);

	if (bpPackage)
	{
	}
	else if (bpFile)
	{
		std::string bpFileName = bpFile->GetName();
		std::transform(bpFileName.begin(), bpFileName.end(), bpFileName.begin(), tolower);

		std::string outFilename = "Data/" + projName + "/scripts/lua/bp/" + bpFileName + ".lua";
		PX2_BPEDIT.CompileBPFile(outFilename, bpFile);
	}
}
//----------------------------------------------------------------------------
void Edit::DisconnectParam()
{
	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	Object *selectObj = PX2_SELECTM_E->GetFirstObject();
	BPParam *bpParam = DynamicCast<BPParam>(selectObj);

	if (bpParam)
	{
		if (bpParam->IsIn())
		{
			for (int i = 0; i < bpParam->GetNumLinkMeParams(); i++)
			{
				BPParam *linkMeParam = bpParam->GetLinkMeParam(i);
				if (linkMeParam)
				{
					linkMeParam->RemoveLinkToParam(bpParam);
				}
			}
		}
		else
		{
			bpParam->RemoveAllLinkToParams();
		}
	}
}
//----------------------------------------------------------------------------
void Edit::OnEvent(Event *event)
{
	if (InputEventSpace::IsEqual(event, InputEventSpace::KeyPressed))
	{
		InputEventData ied = event->GetData<InputEventData>();
		if (KeyCode::KC_F == ied.KCode)
		{
			FocusOnSelection();
		}
	}
}
//----------------------------------------------------------------------------