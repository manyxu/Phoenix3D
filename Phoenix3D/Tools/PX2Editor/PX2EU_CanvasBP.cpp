// PX2EU_CanvasBP.cpp

#include "PX2EU_CanvasBP.hpp"
#include "PX2EU_Manager.hpp"
#include "PX2InputEvent.hpp"
#include "PX2InputEventData.hpp"
#include "PX2Scene.hpp"
#include "PX2SelectionManager.hpp"
#include "PX2Project.hpp"
#include "PX2Edit.hpp"
#include "PX2LanguageManager.hpp"
#include "PX2LogicManager.hpp"
#include "PX2Project.hpp"
#include "PX2BPEdit.hpp"
#include "PX2BPManager.hpp"
#include "PX2Picker.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2Creater.hpp"
#include "PX2UIEditBox.hpp"
#include "PX2UITree.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2ScriptManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UICanvas, EU_CanvasBP);

//----------------------------------------------------------------------------
EU_CanvasBP::EU_CanvasBP() :
mIsInitSizeChanged(false)
{
	RemoveAllCameras();

	ComeInEventWorld();

	Camera *camera = new0 Camera(false);
	camera->SetFrame(APoint(0.0f, msUICameraY, 0.0f), AVector::UNIT_Y,
		AVector::UNIT_Z, AVector::UNIT_X);
	camera->SetClearFlag(false, true, true);
	camera->SetClearColor(Float4::RED);

	mBPOverCameraNode = new0 CameraNode(camera);
	mBPOverCameraNode->SetName("DefaultCameraNode");
	AttachChild(mBPOverCameraNode);

	SetOverCamera(camera);
}
//----------------------------------------------------------------------------
EU_CanvasBP::~EU_CanvasBP()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void EU_CanvasBP::SetInitPackageSize(const Sizef &packageSize)
{
	mInitPackageSize = packageSize;
}
//----------------------------------------------------------------------------
void EU_CanvasBP::OnSizeChanged()
{
	UICanvas::OnSizeChanged();

	const Sizef &size = GetSize();

	if (!mIsInitSizeChanged)
	{
		Camera *editorCamera = mBPOverCameraNode->GetCamera();
		if (editorCamera)
		{
			float helfWidth = size.Width / 2.0f;
			float helfHeight = size.Height / 2.0f;
			editorCamera->SetFrustum(0.1f, Mathf::FAbs(msUICameraY) + 1000.0f,
				-helfHeight, helfHeight, -helfWidth, helfWidth);
		}

		float posX = mInitPackageSize.Width / 2.0f;
		float posZ = mInitPackageSize.Height / 2.0f;
		APoint pos(posX, msUICameraY, -posZ);
		mBPOverCameraNode->LocalTransform.SetTranslate(pos);

		mIsInitSizeChanged = true;
	}

	_ZoomCamera(0.0f);
}
//----------------------------------------------------------------------------
void EU_CanvasBP::OnLeftDown(const APoint &worldPos)
{
	if (!PX2EU_MAN.IsSelectBP())
		return;

	UICanvas::OnLeftDown(worldPos);

	APoint viewPortPos = WorldPosToViewPortPos(worldPos);
	Pick(viewPortPos, 1, 1);

	if (mBluePrintRenderWindow)
	{
		mBluePrintRenderWindow->Show(false);
	}
}
//----------------------------------------------------------------------------
void EU_CanvasBP::OnLeftUp(const APoint &worldPos)
{
	if (!PX2EU_MAN.IsSelectBP())
		return;

	UICanvas::OnLeftUp(worldPos);

	APoint viewPortPos = WorldPosToViewPortPos(worldPos);
	Pick(viewPortPos, 1, 2);

	if (mBluePrintRenderWindow)
	{
		mBluePrintRenderWindow->Show(false);
	}
}
//----------------------------------------------------------------------------
void EU_CanvasBP::OnRightDown(const APoint &worldPos)
{
	if (!PX2EU_MAN.IsSelectBP())
		return;

	UICanvas::OnRightDown(worldPos);

	APoint viewPortPos = WorldPosToViewPortPos(worldPos);
	Pick(viewPortPos, 2, 1);

	if (mBluePrintRenderWindow)
	{
		mBluePrintRenderWindow->Show(false);
	}
}
//----------------------------------------------------------------------------
void EU_CanvasBP::OnRightUp(const APoint &worldPos)
{
	if (!PX2EU_MAN.IsSelectBP())
		return;

	if (mIsRightPressed && !mIsMoved)
	{
		Object *object = PX2_SELECTM_E->GetFirstObject();
		BPPackage *package = DynamicCast<BPPackage>(object);
		BPFile *file = DynamicCast<BPFile>(object);
		BPModule *module = DynamicCast<BPModule>(object);
		BPParam *param = DynamicCast<BPParam>(object);
		
		const Rectf &canvasWorldRect = GetWorldRect();
		float posSizePercentWdith = (worldPos[0] - canvasWorldRect.Left) / canvasWorldRect.Width();
		float posSizePercentHeight = (worldPos[2] - canvasWorldRect.Bottom) / canvasWorldRect.Height();
		Camera *overCamera = mBPOverCameraNode->GetCamera();

		APoint origin;
		AVector direction;
		mBPOverCameraNode->GetCamera()->GetPickRay(posSizePercentWdith, posSizePercentHeight,
			origin, direction);

		PX2_EDIT.SetPickPos(origin);

		if (package)
		{
			CreateEditMenuOnBPPackage(worldPos);
		}
		else if (file)
		{
			APoint contentPos = worldPos;
			contentPos.X() += 2.0f;
			_ShowBPContentFrame(contentPos);
		}
		else if (module)
		{
			CreateEditMenuOnBPModule(worldPos);
		}
		else if (param)
		{
			if (param->GetNumLinkToParams() > 0 || param->GetNumLinkMeParams() > 0)
			{
				CreateEditMenuOnBPParam(worldPos);
			}
		}
	}

	UICanvas::OnRightUp(worldPos);
}
//----------------------------------------------------------------------------
void _BPTreeCallBack(UIFrame *frame, UICallType type)
{
	UICheckButton *checkBut = DynamicCast<UICheckButton>(frame);
	if (checkBut)
	{
		UIItem *item = checkBut->GetUserData<UIItem*>("Item");

		if (UICT_CHECKED == type)
		{
			item->Expand(false);
		}
		else if (UICT_DISCHECKED == type)
		{
			item->Expand(true);
		}
	}
}
void CreateCheckButton(UIItem *item)
{
	UICheckButton *cb = item->CreateButArrow();

	cb->AddUICallback(_BPTreeCallBack);
	cb->SetUserData("Item", item);

	cb->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture(
		"DataNIRVANA2/images/icons/tree/tree_expanded.png");
	cb->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetTexture(
		"DataNIRVANA2/images/icons/tree/tree_collapsed.png");
}
//----------------------------------------------------------------------------
void RefreshTreeItemOnFunObject(FunObject *funObject, UITree *tree,
	UIItem *parentItem)
{
	UIItem *classItem = tree->AddItem(parentItem, funObject->ClassName,
		funObject->ClassName);
	classItem->Expand(false);
	CreateCheckButton(classItem);

	bool isClassCatalogue = funObject->IsClassCatalogue;
	const std::string &className = funObject->ClassName;
	const std::string &funName = funObject->FunName;

	std::vector<Pointer0<FunObject> > funObjectVec;

	funObjectVec = funObject->mChildFunObjectVec_Class;
	for (int i = 0; i < (int)funObjectVec.size(); i++)
	{
		FunObject *childFunObj = funObjectVec[i];
		RefreshTreeItemOnFunObject(childFunObj, tree, classItem);
	}

	funObjectVec = funObject->mChildFunObjectVec;
	for (int i = 0; i < (int)funObjectVec.size(); i++)
	{
		FunObject *funObj = funObjectVec[i];
		UIItem *sItem = tree->AddItem(classItem, funObj->FunName, funObj->FunName);
		sItem->SetUserData("ClassName", className);
		sItem->SetUserData("FunName", funObj->FunName);
		sItem->SetUserData("Tag", std::string("ObjectFun"));
	}
}
//----------------------------------------------------------------------------
void EU_CanvasBP::_ShowBPContentFrame(const APoint &worldPos)
{
	if (!mBluePrintRenderWindow)
	{
		RenderWindow *mainRW = PX2_GR.GetMainWindow();

		mBluePrintRenderWindow = PX2EU_MAN.CreateUIWindow(
			mainRW, std::string("BluePrintTreeWindow"), std::string("BluePrint"), worldPos,
			Sizef(400.0f, 300.0f), true);
		Canvas *canvas = mBluePrintRenderWindow->GetMainCanvas();
		UIFrame *contentFrame = DynamicCast<UIFrame>(
			canvas->GetObjectByName("CanvasContentFrame"));
		contentFrame->DetachAllChildren();

		UIPicBox *picBox = contentFrame->CreateAddBackgroundPicBox();
		picBox->SetColor(PX2_UISM.Color_ContentBackground);

		UIFrame *frame = CreateBPContentFrame();
		contentFrame->AttachChild(frame);
	}
	else
	{
		mBluePrintRenderWindow->SetPosition(worldPos);
		mBluePrintRenderWindow->Show(true);
	}
}
//----------------------------------------------------------------------------
void _BPContentCallBack(UIFrame *frame, UICallType type)
{
	UITree *tree = DynamicCast<UITree>(frame);
	if (tree)
	{
		if (UICT_TREE_DOUBLE_SELECTED == type)
		{
			RenderWindow *rw = PX2_GR.GetRenderWindow("BluePrintTreeWindow");
			if (rw)
			{
				rw->Show(false);
			}

			UIItem *selectItem = tree->GetSelectedItem();
			if (selectItem)
			{
				const std::string &selectItemText = selectItem->GetFText()->GetText()->GetText();

				bool classNameValied = false;
				std::string className;
				className = selectItem->GetUserData<std::string>("ClassName", &classNameValied);
				bool funNameValied = false;
				std::string funName;
				funName = selectItem->GetUserData<std::string>("FunName", &funNameValied);
				bool tagValied = false;
				std::string tag;
				tag = selectItem->GetUserData<std::string>("Tag", &tagValied);

				if (classNameValied && funNameValied && tagValied)
				{
					std::string script;
					
					if ("Event" == tag)
						script = "n_CreateBPEvent('" + className + "', '" + funName + "')";
					else if ("Param" == tag)
						script = "n_CreateBPParam('" + funName + "')";
					else if ("GlobalFun" == tag)
						script = "n_CreateBPModule(nil, '" + funName + "')";
					else if ("ObjectFun" == tag)
						script = "n_CreateBPModule('" + className + "', '" + funName + "')";
					else if ("Option" == tag)
						script = "n_CreateBPOption('" + funName + "')";
					else if ("Operator" == tag)
						script = "n_CreateBPOperator('" + funName + "')";
					
					if (!script.empty())
					{
						PX2_SC_LUA->CallString(script);
					}
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
UIFrame *EU_CanvasBP::CreateBPContentFrame()
{
	UIFrame *frame = new0 UIFrame();
	frame->SetAnchorHor(0.0f, 1.0f);
	frame->SetAnchorVer(0.0f, 1.0f);

	UIFText *text = new0 UIFText();
	frame->AttachChild(text);
	text->LocalTransform.SetTranslateY(-1.0f);
	text->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	text->GetText()->SetFontColor(Float3::MakeColor(50, 212, 255));
	text->GetText()->SetFontScale(0.8f);
	text->GetText()->SetText("All BluePrints Functions");
	text->GetText()->SetDrawStyle(FD_SHADOW);
	text->SetAnchorHor(0.0f, 1.0f);
	text->SetAnchorVer(1.0f, 1.0f);
	text->SetPivot(0.5f, 1.0f);
	text->SetSize(0.0f, PX2_UISM.Size_SearchBar);

	UIEditBox *searchEditBox = new0 UIEditBox();
	frame->AttachChild(searchEditBox);
	searchEditBox->LocalTransform.SetTranslateY(-1.0f);
	searchEditBox->SetAnchorHor(0.0f, 1.0f);
	searchEditBox->SetAnchorVer(1.0f, 1.0f);
	searchEditBox->SetPivot(0.5f, 1.0f);
	searchEditBox->SetSize(0.0f, PX2_UISM.Size_SearchBar);
	searchEditBox->SetAnchorParamVer(-PX2_UISM.Size_SearchBar, 0.0f);

	UITree *tree = new0 UITree();
	frame->AttachChild(tree);
	tree->LocalTransform.SetTranslateY(-1.0f);
	tree->SetAnchorHor(0.0f, 1.0f);
	tree->SetAnchorVer(0.0f, 1.0f);
	tree->ShowRootItem(false);
	tree->SetAnchorParamVer(0.0f, -PX2_UISM.Size_SearchBar * 2.0f);

	UIItem *rootItem = tree->GetRootItem();
	CreateCheckButton(rootItem);

	UIItem *eventsItem = tree->AddItem(rootItem, "Events");
	const std::vector<std::string> &eventClasses = PX2_LOGICM.GetEventClass();
	const std::map<std::string, std::vector<std::string> > &eventMap = PX2_LOGICM.GetEvents();
	for (int i = 0; i < (int)eventClasses.size(); i++)
	{
		const std::string &spaceName = eventClasses[i];

		UIItem *spaceItem = tree->AddItem(eventsItem, spaceName, spaceName);
		auto it = eventMap.find(spaceName);
		if (it != eventMap.end())
		{
			const std::vector<std::string> &eventVecs = it->second;
			for (int j = 0; j < (int)eventVecs.size(); j++)
			{
				const std::string &eName = eventVecs[j];
				UIItem *sItem = tree->AddItem(spaceItem, eName, eName);
				sItem->SetUserData("ClassName", spaceName);
				sItem->SetUserData("FunName", eName);
				sItem->SetUserData("Tag", std::string("Event"));
			}
		}
		spaceItem->Expand(false);
		CreateCheckButton(spaceItem);
	}
	eventsItem->Expand(false);
	CreateCheckButton(eventsItem);

	UIItem *paramsItem = tree->AddItem(rootItem, "Params");
	const std::vector<std::string> &params = PX2_LOGICM.GetParams();
	for (int i = 0; i < (int)params.size(); i++)
	{
		const std::string &name = params[i];
		UIItem *sItem = tree->AddItem(paramsItem, name, name);
		sItem->SetUserData("ClassName", std::string(""));
		sItem->SetUserData("FunName", name);
		sItem->SetUserData("Tag", std::string("Param"));
	}
	paramsItem->Expand(false);
	CreateCheckButton(paramsItem);

	UIItem *functionsItem = tree->AddItem(rootItem, "Functions");
	functionsItem->Expand(false);
	CreateCheckButton(functionsItem);

	UIItem *sItemFS = tree->AddItem(functionsItem, "FunctionStart", "FunctionStart");
	sItemFS->SetUserData("ClassName", std::string(""));
	sItemFS->SetUserData("FunName", std::string("FunctionStart"));
	sItemFS->SetUserData("Tag", std::string("GlobalFun"));

	UIItem *globalFunsItem = tree->AddItem(functionsItem, "GlobalFuns", "GlobalFuns");
	const std::vector<std::string> &gfs = PX2_LOGICM.GetGFs();
	for (int i = 0; i < (int)gfs.size(); i++)
	{
		const std::string &gfName = gfs[i];
		UIItem *sItem = tree->AddItem(globalFunsItem, gfName, gfName);
		sItem->SetUserData("ClassName", std::string(""));
		sItem->SetUserData("FunName", gfName);
		sItem->SetUserData("Tag", std::string("GlobalFun"));
	}
	globalFunsItem->Expand(false);
	CreateCheckButton(globalFunsItem);

	UIItem *objectFunsItem = tree->AddItem(functionsItem, "ObjectFuns", "ObjectFuns");
	FunObject *funObject = FunObjectManager::GetSingleton().GetObjectFunObject();	
	RefreshTreeItemOnFunObject(funObject, tree, objectFunsItem);
	objectFunsItem->Expand(false);
	CreateCheckButton(objectFunsItem);

	UIItem *optionItem = tree->AddItem(rootItem, "Options", "Options");
	const std::vector<std::string> &optionStrs = PX2_LOGICM.GetOptions();
	for (int i = 0; i < (int)optionStrs.size(); i++)
	{
		const std::string &optionStr = optionStrs[i];
		UIItem *sItem = tree->AddItem(optionItem, optionStr, optionStr);
		sItem->SetUserData("ClassName", std::string(""));
		sItem->SetUserData("FunName", optionStr);
		sItem->SetUserData("Tag", std::string("Option"));
	}
	optionItem->Expand(false);
	CreateCheckButton(optionItem);

	UIItem *operatorItem = tree->AddItem(rootItem, "Operators", "Operators");
	const std::vector<std::string> &operatorStrs = PX2_LOGICM.GetOperators();
	for (int i = 0; i < (int)operatorStrs.size(); i++)
	{
		const std::string &operatorStr = operatorStrs[i];
		UIItem *sItem = tree->AddItem(optionItem, operatorStr, operatorStr);
		sItem->SetUserData("ClassName", std::string(""));
		sItem->SetUserData("FunName", operatorStr);
		sItem->SetUserData("Tag", std::string("Operator"));
	}
	operatorItem->Expand(false);
	CreateCheckButton(operatorItem);

	tree->AddUICallback(_BPContentCallBack);

	return frame;
}
//----------------------------------------------------------------------------
void EU_CanvasBP::OnMotion(const APoint &worldPos)
{
	if (!PX2EU_MAN.IsSelectBP())
		return;

	UICanvas::OnMotion(worldPos);

	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	AVector delta = mMoveDelta;

	if (mIsMiddlePressed)
	{
		_PanCamera(delta.X()*mPixelToWorld.first, delta.Z()*mPixelToWorld.second);
	}

	APoint viewPortPos = WorldPosToViewPortPos(worldPos);

	if (mIsMoved)
	{
		if (mIsLeftPressed)
		{
			BPEdit *bpEdit = BPEdit::GetSingletonPtr();
			if (bpEdit)
			{
				if (BPEdit::LS_NONE == bpEdit->GetLinkingState())
				{
					ProcessSelectLeftMove(mMoveDelta.X(), mMoveDelta.Z());
				}
				else
				{
					Pick(viewPortPos, 1, 3);

					UpdateLinkingPos(worldPos);
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void EU_CanvasBP::OnMouseWheel(const APoint &worldPos, float delta)
{
	if (!PX2EU_MAN.IsSelectBP())
		return;

	if (!IsEnable())
		return;

	const Rectf &rectWorld = GetWorldRect();
	if (!rectWorld.IsInsize(worldPos.X(), worldPos.Z()))
		return;

	UICanvas::OnMouseWheel(worldPos, delta);

	_ZoomCamera(delta * mPixelToWorld.second);
}
//----------------------------------------------------------------------------
void EU_CanvasBP::Pick(const APoint &scrPos, int type, int down_up_moving)
{
	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	BPPackage *bpPackage = PX2_PROJ.GetBPPackage();
	BPParam *linkingParam = PX2_BPEDIT.GetLinkingParam();
	BPEdit::LinkingState linkingState = PX2_BPEDIT.GetLinkingState();
	Object *curSelectObject = PX2_SELECTM_E->GetFirstObject();

	Camera *camera = mBPOverCameraNode->GetCamera();

	APoint origin;
	AVector direction;
	camera->GetPickRay(scrPos.X(), scrPos.Z(), GetSize(), origin, direction);

	Picker picker;
	MovablePtr pickedMove = 0;
	picker.Execute(bpPackage, origin, direction, 0.0f, Mathf::MAX_REAL);
	if (0 == (int)picker.Records.size())
	{
		if (BPEdit::LS_NONE == linkingState)
			PX2_SELECTM_E->Clear();
	}
	else
	{
		const PickRecord &rec = picker.GetClosestNonnegative();
		pickedMove = rec.Intersected;

		BPPackage *bpPackage = 0;
		BPFile *bpFile = 0;
		BPModule *bpModule = 0;
		BPParam *bpParam = 0;
		Movable *mov = JudgePickedObject(pickedMove, bpPackage, bpFile, 
			bpModule, bpParam);

		if (mov && mov == curSelectObject)
		{
			if (3 == down_up_moving)
				return;
		}
		else
		{
			if (BPEdit::LS_NONE == linkingState)
			{
				PX2_SELECTM_E->Clear();

				if (mov)
					PX2_SELECTM_E->AddObject(mov);

				if (bpParam)
				{
					if (1 == type && 1 == down_up_moving && !bpParam->IsIn())
					{
						PX2_BPEDIT.SetLinkingParam(bpParam);
						PX2_BPEDIT.SetLinkingState(BPEdit::LS_LINKING);
					}
				}
			}
			else if (BPEdit::LS_LINKING == linkingState)
			{
				if (2 == down_up_moving)
				{
					PX2_BPEDIT.SetLinkingState(BPEdit::LS_NONE);
				}
				else if (3 == down_up_moving)
				{
					if (bpParam)
					{
						if (PX2_BPM.CanLinkParam(linkingParam, bpParam))
						{
							PX2_BPEDIT.SetLinkingState(BPEdit::LS_LINKING_CANPUT);
						}
					}
				}
			}
			else if (BPEdit::LS_LINKING_CANPUT == linkingState)
			{
				if (2 == down_up_moving)
				{
					if (bpParam)
					{
						PX2_BPEDIT.SetLinkedParam(bpParam);
						PX2_BPEDIT.SetLinkingState(BPEdit::LS_NONE);
						PX2_SELECTM_E->Clear();
					}
				}
				else if (3 == down_up_moving)
				{
					if (bpParam && PX2_BPM.CanLinkParam(linkingParam, bpParam))
					{
						// keep
					}
					else
					{
						PX2_BPEDIT.SetLinkingState(BPEdit::LS_LINKING);
					}
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
Movable *EU_CanvasBP::JudgePickedObject(Movable *pickedMov,
	BPPackage* &lp, BPFile* &lg, BPModule* &lm, BPParam *&lmp)
{
	Movable *pickedMovePar = pickedMov->GetParent();
	UIButton *but = DynamicCast<UIButton>(pickedMovePar);

	if (but)
	{
		Movable *butParent = but->GetParent();
		lmp = DynamicCast<BPParam>(butParent);
		if (lmp) return lmp;
	}
	else
	{
		lp = DynamicCast<BPPackage>(pickedMovePar);
		if (lp) return lp;

		lg = DynamicCast<BPFile>(pickedMovePar);
		if (lg) return lg;

		lm = DynamicCast<BPModule>(pickedMovePar);
		if (lm) return lm;
	}

	return 0;
}
//----------------------------------------------------------------------------
void EU_CanvasBP::ProcessSelectLeftMove(float horzPixel, float vertPixel)
{
	Object *selObj = PX2_SELECTM_E->GetFirstObject();
	Movable *mov = DynamicCast<Movable>(selObj);
	BPPackage *bpPackage = DynamicCast<BPPackage>(selObj);
	BPFile *bpFile = DynamicCast<BPFile>(selObj);
	BPModule *bpModule = DynamicCast<BPModule>(selObj);

	if (bpPackage || bpFile || bpModule)
	{
		APoint localPos = mov->LocalTransform.GetTranslate();

		float uiCamRMax = mBPOverCameraNode->GetCamera()->GetRMax();
		float uiCamUMax = mBPOverCameraNode->GetCamera()->GetUMax();
		float uiPercentR = (uiCamRMax*2.0f) / mSize.Width;
		float uiPercentU = (uiCamUMax*2.0f) / mSize.Height;

		localPos.X() += horzPixel*uiPercentR;
		localPos.Z() += vertPixel*uiPercentU;

		mov->LocalTransform.SetTranslate(localPos);
	}
}
//----------------------------------------------------------------------------
void EU_CanvasBP::UpdateLinkingPos(const APoint &worldPos)
{
	Camera *camera = mBPOverCameraNode->GetCamera();

	const Rectf &canvasWorldRect = GetWorldRect();
	float posSizePercentWdith = (worldPos[0] - canvasWorldRect.Left) / canvasWorldRect.Width();
	float posSizePercentHeight = (worldPos[2] - canvasWorldRect.Bottom) / canvasWorldRect.Height();
	Camera *overCamera = mBPOverCameraNode->GetCamera();

	APoint origin;
	AVector direction;
	mBPOverCameraNode->GetCamera()->GetPickRay(posSizePercentWdith, posSizePercentHeight,
		origin, direction);
	origin.Y() = 0.0f;

	// picking
	BPParam *bpParam = DynamicCast<BPParam>(PX2_BPEDIT.GetLinkingParam());
	if (bpParam)
	{
		PX2_BPEDIT.SetLinkingParam(bpParam);
		PX2_BPEDIT.SetLinkingWorldPos(origin);
	}
}
//----------------------------------------------------------------------------
void EU_CanvasBP::CreateEditMenuOnBPPackage(const APoint &pos)
{
	PX2EU_MAN.Menu_Edit_Begin("BP", "BPPackage");
	PX2EU_MAN.Menu_Edit_AddSubItem("BP", "BPPackage", "BluePrint", "Create");

	PX2EU_MAN.Menu_Edit_AddItem("BP", "BPPackageBluePrint", "BPPackage", "BPPackage", "n_CreateBPPackage()");
	PX2EU_MAN.Menu_Edit_AddItem("BP", "BPPackageBluePrint", "BPFile", "BPFile", "n_CreateBPFile()");

	PX2EU_MAN.Menu_Edit_EndPopUp("BP", pos);
}
//----------------------------------------------------------------------------
void RefreshMenuOnFunObject(FunObject *funObject, std::string &parentName)
{
	bool isClassCatalogue = funObject->IsClassCatalogue;
	const std::string &className = funObject->ClassName;
	const std::string &funName = funObject->FunName;

	if (isClassCatalogue)
	{
		PX2EU_MAN.Menu_Edit_AddSubItem("BP", parentName, className, className);
		parentName += className;
	}
	else
	{
		std::string script = "n_CreateBPModule('" + funObject->ClassName + "', '"
			+ funObject->FunName + "')";
		PX2EU_MAN.Menu_Edit_AddItem("BP", parentName, funName, funName, script);
		return;
	}

	std::vector<Pointer0<FunObject> > funObjectVec = funObject->mChildFunObjectVec;
	for (int i = 0; i < (int)funObjectVec.size(); i++)
	{
		FunObject *funObjectChild = funObjectVec[i];
		RefreshMenuOnFunObject(funObjectChild, parentName);
	}

	funObjectVec = funObject->mChildFunObjectVec_Class;
	if ((int)funObjectVec.size() > 0)
	{
		PX2EU_MAN.Menu_Edit_AddItemSeparater("BP", parentName);
		for (int i = 0; i < (int)funObjectVec.size(); i++)
		{
			FunObject *funObjectChild = funObjectVec[i];
			RefreshMenuOnFunObject(funObjectChild, parentName);
		}
	}
}
//----------------------------------------------------------------------------
void EU_CanvasBP::CreateEditMenuOnBPModule(const APoint &pos)
{
	PX2EU_MAN.Menu_Edit_Begin("BP", "BPModule");

	PX2EU_MAN.Menu_Edit_AddItem("BP", "BPModule", "Delete", PX2_LM.V("Delete"), "n_DeleteSelection()");

	PX2EU_MAN.Menu_Edit_EndPopUp("BP", pos);
}
//----------------------------------------------------------------------------
void EU_CanvasBP::CreateEditMenuOnBPParam(const APoint &pos)
{
	PX2EU_MAN.Menu_Edit_Begin("BP", "BPParam");

	PX2EU_MAN.Menu_Edit_AddItem("BP", "BPParam", "Disconnect", PX2_LM.V("Disconnect"), "n_DisconnectParam()");

	PX2EU_MAN.Menu_Edit_EndPopUp("BP", pos);
}
//----------------------------------------------------------------------------
void EU_CanvasBP::_PanCamera(const float &horz, const float &vert)
{
	if (!Project::GetSingletonPtr()) return;

	BPPackage *package = PX2_PROJ.GetBPPackage();
	if (!package) return;

	if (mBPOverCameraNode)
	{
		APoint position = mBPOverCameraNode->LocalTransform.GetTranslate();

		position.Z() -= vert * 1.0f;
		position.X() -= horz * 1.0f;

		mBPOverCameraNode->LocalTransform.SetTranslate(position);
	}
}
//----------------------------------------------------------------------------
void EU_CanvasBP::_ZoomCamera(float zoom)
{
	if (!Project::GetSingletonPtr()) return;

	BPPackage *package = PX2_PROJ.GetBPPackage();
	if (!package) return;

	const Sizef &size = GetSize();

	if (mBPOverCameraNode)
	{
		float rMin = 0.0f;
		float rMax = 0.0f;
		float uMin = 0.0f;
		float uMax = 0.0f;
		float dMin = 0.0f;
		float dMax = 0.0f;
		float orthoFrustumMin = 1.0f;
		mBPOverCameraNode->GetCamera()->GetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);
		uMin += (zoom * 1.0f);
		if (uMin > -orthoFrustumMin)
			uMin = -orthoFrustumMin;
		uMax = -uMin;
		rMin = uMin * size.Width / size.Height;
		rMax = -rMin;

		mBPOverCameraNode->GetCamera()->SetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);
	}
}
//----------------------------------------------------------------------------