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
using namespace PX2;

//PX2_IMPLEMENT_RTTI(PX2, UICanvas, EU_CanvasBP);
//
////----------------------------------------------------------------------------
//EU_CanvasBP::EU_CanvasBP()
//{
//	ComeInEventWorld();
//}
////----------------------------------------------------------------------------
//EU_CanvasBP::~EU_CanvasBP()
//{
//	GoOutEventWorld();
//}
////----------------------------------------------------------------------------
//void EU_CanvasBP::OnEvent(Event *event)
//{
//	UICanvas::OnEvent(event);
//}
////----------------------------------------------------------------------------
//void EU_CanvasBP::OnLeftDown(const APoint &worldPos)
//{
//	UICanvas::OnLeftDown(worldPos);
//
//	Pick(mLeftPressedPos, 1, 1);
//}
////----------------------------------------------------------------------------
//void EU_CanvasBP::OnLeftUp(const APoint &worldPos)
//{
//	UICanvas::OnLeftUp(worldPos);
//
//	Pick(mLeftReleasedPos, 1, 2);
//}
////----------------------------------------------------------------------------
//void EU_CanvasBP::OnRightDown(const APoint &worldPos)
//{
//	UICanvas::OnRightDown(worldPos);
//
//	Pick(mRightPressedPos, 2, 1);
//}
////----------------------------------------------------------------------------
//void EU_CanvasBP::OnRightUp(const APoint &worldPos)
//{
//	if (mIsRightPressed && !mIsMoved)
//	{
//		Object *object = PX2_SELECTM_E->GetFirstObject();
//		BPPackage *package = DynamicCast<BPPackage>(object);
//		BPFile *file = DynamicCast<BPFile>(object);
//		BPModule *module = DynamicCast<BPModule>(object);
//		BPParam *param = DynamicCast<BPParam>(object);
//
//		APoint pos = ScreenPosToCameraPos(worldPos);
//		PX2_EDIT.SetPickPos(pos);
//
//		if (package)
//			CreateEditMenuOnBPPackage(worldPos);
//		else if (file)
//			CreateEditMenuOnBPFile(worldPos);
//		else if (module)
//			CreateEditMenuOnBPModule(worldPos);
//		else if (param)
//		{
//			if (param->GetNumLinkToParams() > 0 || param->GetNumLinkMeParams() > 0)
//			{
//				CreateEditMenuOnBPParam(worldPos);
//			}
//		}
//	}
//
//	UICanvas::OnRightUp(worldPos);
//}
////----------------------------------------------------------------------------
//void EU_CanvasBP::OnMotion(const APoint &worldPos)
//{
//	UICanvas::OnMotion(worldPos);
//
//	Project *proj = Project::GetSingletonPtr();
//	if (!proj) return;
//
//	if (mIsMoved)
//	{
//		if (mIsLeftPressed)
//		{
//			if (BPEdit::LS_NONE == PX2_BPEDIT.GetLinkingState())
//			{
//				ProcessSelectLeftMove(mMoveDelta.X(), mMoveDelta.Z());
//			}
//			else
//			{
//				Pick(worldPos, 1, 3);
//
//				UpdateLinkingPos(worldPos);
//			}
//		}
//	}
//}
////----------------------------------------------------------------------------
//void EU_CanvasBP::Pick(const APoint &screenPos, int type, int down_up_moving)
//{
//	Project *proj = Project::GetSingletonPtr();
//	if (!proj) return;
//
//	BPPackage *bpPackage = PX2_PROJ.GetBPPackage();
//	BPParam *linkingParam = PX2_BPEDIT.GetLinkingParam();
//	BPEdit::LinkingState linkingState = PX2_BPEDIT.GetLinkingState();
//	Object *curSelectObject = PX2_SELECTM_E->GetFirstObject();
//
//	APoint origin;
//	AVector direction;
//	GetPickRay(screenPos.X(), screenPos.Z(), origin, direction);
//
//	PX2::Picker picker;
//	PX2::MovablePtr pickedMove = 0;
//	picker.Execute(bpPackage, origin, direction, 0.0f, Mathf::MAX_REAL);
//	if (0 == (int)picker.Records.size())
//	{
//		if (BPEdit::LS_NONE == linkingState)
//			PX2_SELECTM_E->Clear();
//	}
//	else
//	{
//		const PickRecord &rec = picker.GetClosestNonnegative();
//		pickedMove = rec.Intersected;
//
//		BPPackage *bpPackage = 0;
//		BPFile *bpFile = 0;
//		BPModule *bpModule = 0;
//		BPParam *bpParam = 0;
//		Movable *mov = JudgePickedObject(pickedMove, bpPackage, bpFile, 
//			bpModule, bpParam);
//
//		if (mov && mov == curSelectObject)
//		{
//			if (3 == down_up_moving)
//				return;
//		}
//		else
//		{
//			if (BPEdit::LS_NONE == linkingState)
//			{
//				PX2_SELECTM_E->Clear();
//
//				if (mov)
//					PX2_SELECTM_E->AddObject(mov);
//
//				if (bpParam)
//				{
//					if (1 == type && 1 == down_up_moving && !bpParam->IsIn())
//					{
//						PX2_BPEDIT.SetLinkingParam(bpParam);
//						PX2_BPEDIT.SetLinkingState(BPEdit::LS_LINKING);
//					}
//				}
//			}
//			else if (BPEdit::LS_LINKING == linkingState)
//			{
//				if (2 == down_up_moving)
//				{
//					PX2_BPEDIT.SetLinkingState(BPEdit::LS_NONE);
//				}
//				else if (3 == down_up_moving)
//				{
//					if (bpParam)
//					{
//						if (PX2_BPM.CanLinkParam(linkingParam, bpParam))
//						{
//							PX2_BPEDIT.SetLinkingState(BPEdit::LS_LINKING_CANPUT);
//						}
//					}
//				}
//			}
//			else if (BPEdit::LS_LINKING_CANPUT == linkingState)
//			{
//				if (2 == down_up_moving)
//				{
//					if (bpParam)
//					{
//						PX2_BPEDIT.SetLinkedParam(bpParam);
//						PX2_BPEDIT.SetLinkingState(BPEdit::LS_NONE);
//						PX2_SELECTM_E->Clear();
//					}
//				}
//				else if (3 == down_up_moving)
//				{
//					if (bpParam && PX2_BPM.CanLinkParam(linkingParam, bpParam))
//					{
//						// keep
//					}
//					else
//					{
//						PX2_BPEDIT.SetLinkingState(BPEdit::LS_LINKING);
//					}
//				}
//			}
//		}
//	}
//}
////----------------------------------------------------------------------------
//Movable *EU_CanvasBP::JudgePickedObject(Movable *pickedMov,
//	BPPackage* &lp, BPFile* &lg, BPModule* &lm, BPParam *&lmp)
//{
//	Movable *pickedMovePar = pickedMov->GetParent();
//	UIButton *but = DynamicCast<UIButton>(pickedMovePar);
//
//	if (but)
//	{
//		Movable *butParent = but->GetParent();
//		lmp = DynamicCast<BPParam>(butParent);
//		if (lmp) return lmp;
//	}
//	else
//	{
//		lp = DynamicCast<BPPackage>(pickedMovePar);
//		if (lp) return lp;
//
//		lg = DynamicCast<BPFile>(pickedMovePar);
//		if (lg) return lg;
//
//		lm = DynamicCast<BPModule>(pickedMovePar);
//		if (lm) return lm;
//	}
//
//	return 0;
//}
////----------------------------------------------------------------------------
//void EU_CanvasBP::ProcessSelectLeftMove(float horzPixel, float vertPixel)
//{
//	Object *selObj = PX2_SELECTM_E->GetFirstObject();
//	Movable *mov = DynamicCast<Movable>(selObj);
//	BPPackage *bpPackage = DynamicCast<BPPackage>(selObj);
//	BPFile *bpFile = DynamicCast<BPFile>(selObj);
//	BPModule *bpModule = DynamicCast<BPModule>(selObj);
//
//	if (bpPackage || bpFile || bpModule)
//	{
//		APoint localPos = mov->LocalTransform.GetTranslate();
//
//		float uiCamRMax = mCamera->GetRMax();
//		float uiCamUMax = mCamera->GetUMax();
//		float uiPercentR = (uiCamRMax*2.0f) / mSize.Width;
//		float uiPercentU = (uiCamUMax*2.0f) / mSize.Height;
//
//		localPos.X() += horzPixel*uiPercentR;
//		localPos.Z() += vertPixel*uiPercentU;
//
//		mov->LocalTransform.SetTranslate(localPos);
//	}
//}
////----------------------------------------------------------------------------
//void EU_CanvasBP::UpdateLinkingPos(const APoint &screenPos)
//{
//	// picking
//	BPParam *bpParam = DynamicCast<BPParam>(PX2_BPEDIT.GetLinkingParam());
//	if (bpParam)
//	{
//		APoint worldRealPos = ScreenPosToCameraPos(screenPos);
//
//		PX2_BPEDIT.SetLinkingParam(bpParam);
//		PX2_BPEDIT.SetLinkingWorldPos(worldRealPos);
//	}
//}
////----------------------------------------------------------------------------
//void EU_CanvasBP::CreateEditMenuOnBPPackage(const APoint &pos)
//{
//	PX2EU_MAN.Menu_Edit_Begin("BP", "BPPackage");
//	PX2EU_MAN.Menu_Edit_AddSubItem("BP", "BPPackage", "BluePrint", "Create");
//
//	PX2EU_MAN.Menu_Edit_AddItem("BP", "BPPackageBluePrint", "BPPackage", "BPPackage", "n_CreateBPPackage()");
//	PX2EU_MAN.Menu_Edit_AddItem("BP", "BPPackageBluePrint", "BPFile", "BPFile", "n_CreateBPFile()");
//
//	PX2EU_MAN.Menu_Edit_EndPopUp("BP", pos);
//}
////----------------------------------------------------------------------------
//void RefreshMenuOnFunObject(FunObject *funObject, std::string &parentName)
//{
//	bool isClassCatalogue = funObject->IsClassCatalogue;
//	const std::string &className = funObject->ClassName;
//	const std::string &funName = funObject->FunName;
//
//	if (isClassCatalogue)
//	{
//		PX2EU_MAN.Menu_Edit_AddSubItem("BP", parentName, className, className);
//		parentName += className;
//	}
//	else
//	{
//		std::string script = "n_CreateBPModule('" + funObject->ClassName + "', '"
//			+ funObject->FunName + "')";
//		PX2EU_MAN.Menu_Edit_AddItem("BP", parentName, funName, funName, script);
//		return;
//	}
//
//	std::vector<Pointer0<FunObject> > funObjectVec = funObject->mChildFunObjectVec;
//	for (int i = 0; i < (int)funObjectVec.size(); i++)
//	{
//		FunObject *funObjectChild = funObjectVec[i];
//		RefreshMenuOnFunObject(funObjectChild, parentName);
//	}
//
//	funObjectVec = funObject->mChildFunObjectVec_Class;
//	if ((int)funObjectVec.size() > 0)
//	{
//		PX2EU_MAN.Menu_Edit_AddItemSeparater("BP", parentName);
//		for (int i = 0; i < (int)funObjectVec.size(); i++)
//		{
//			FunObject *funObjectChild = funObjectVec[i];
//			RefreshMenuOnFunObject(funObjectChild, parentName);
//		}
//	}
//}
////----------------------------------------------------------------------------
//void EU_CanvasBP::CreateEditMenuOnBPFile(const APoint &pos)
//{
//	PX2EU_MAN.Menu_Edit_Begin("BP", "BPFile");
//	PX2EU_MAN.Menu_Edit_AddSubItem("BP", "BPFile", "BluePrint", PX2_LM.V("Create"));
//
//	// Events
//	PX2EU_MAN.Menu_Edit_AddSubItem("BP", "BPFileBluePrint", "Events", "Events");
//	const std::vector<std::string> &events = PX2_LOGICM.GetEvents();
//	for (int i = 0; i < (int)events.size(); i++)
//	{
//		const std::string &eName = events[i];
//		if (!eName.empty())
//		{
//			std::string script = "n_CreateBPEvent('" + eName + "')";
//			PX2EU_MAN.Menu_Edit_AddItem("BP", "BPFileBluePrintEvents",
//				eName, eName, script);
//		}
//	}
//
//	PX2EU_MAN.Menu_Edit_AddSubItem("BP", "BPFileBluePrint", "Params", "Params");
//	const std::vector<std::string> &params = PX2_LOGICM.GetParams();
//	for (int i = 0; i < (int)params.size(); i++)
//	{
//		const std::string &name = params[i];
//		std::string script = "n_CreateBPParam('" + name + "')";
//		PX2EU_MAN.Menu_Edit_AddItem("BP", "BPFileBluePrintParams", name, name, script);
//	}
//
//
//	PX2EU_MAN.Menu_Edit_AddSubItem("BP", "BPFileBluePrint", "Functions", "Functions");
//
//	std::string script = "n_CreateBPModule(nil,'FunctionStart')";
//	PX2EU_MAN.Menu_Edit_AddItem("BP", "BPFileBluePrintFunctions", "FunctionStart", "FunctionStart", script);
//
//	PX2EU_MAN.Menu_Edit_AddItemSeparater("BP", "BPFileBluePrintFunctions");
//	const std::vector<std::string> &gfs = PX2_LOGICM.GetGFs();
//	for (int i = 0; i < (int)gfs.size(); i++)
//	{
//		const std::string &gfName = gfs[i];
//		std::string script = "n_CreateBPModule(nil, '" + gfName + "')";
//		PX2EU_MAN.Menu_Edit_AddItem("BP", "BPFileBluePrintFunctions", gfName, gfName, script);
//	}
//
//	FunObject *funObject = FunObjectManager::GetSingleton().GetFunObject();
//	std::string bpFileBlueName = std::string("BPFileBluePrint");
//	RefreshMenuOnFunObject(funObject, bpFileBlueName);
//
//	PX2EU_MAN.Menu_Edit_AddSubItem("BP", "BPFileBluePrint", "Option", "Option");
//	const std::vector<std::string> &optionStrs = PX2_LOGICM.GetOptions();
//	for (int i = 0; i < (int)optionStrs.size(); i++)
//	{
//		const std::string &optionStr = optionStrs[i];
//		std::string script = "n_CreateBPOption('" + optionStr + "')";
//		PX2EU_MAN.Menu_Edit_AddItem("BP", "BPFileBluePrintOption", optionStr, optionStr, script);
//	}
//
//	PX2EU_MAN.Menu_Edit_AddSubItem("BP", "BPFileBluePrint", "Operator", "Operator");
//	const std::vector<std::string> &operatorStrs = PX2_LOGICM.GetOperators();
//	for (int i = 0; i < (int)operatorStrs.size(); i++)
//	{
//		const std::string &operatorStr = operatorStrs[i];
//		std::string script = "n_CreateBPOperator('" + operatorStr + "')";
//		PX2EU_MAN.Menu_Edit_AddItem("BP", "BPFileBluePrintOperator", operatorStr, operatorStr, script);
//	}
//
//	PX2EU_MAN.Menu_Edit_AddItem("BP", "BPFile", "Delete", PX2_LM.V("Delete"), "n_DeleteSelection()");
//	PX2EU_MAN.Menu_Edit_AddItemSeparater("BP", "BPFile");
//	PX2EU_MAN.Menu_Edit_AddItem("BP", "BPFile", "Compile", PX2_LM.V("Compile"), "n_BPFileCompile()");
//
//	PX2EU_MAN.Menu_Edit_EndPopUp("BP", pos);
//}
////----------------------------------------------------------------------------
//void EU_CanvasBP::CreateEditMenuOnBPModule(const APoint &pos)
//{
//	PX2EU_MAN.Menu_Edit_Begin("BP", "BPModule");
//
//	PX2EU_MAN.Menu_Edit_AddItem("BP", "BPModule", "Delete", PX2_LM.V("Delete"), "n_DeleteSelection()");
//
//	PX2EU_MAN.Menu_Edit_EndPopUp("BP", pos);
//}
////----------------------------------------------------------------------------
//void EU_CanvasBP::CreateEditMenuOnBPParam(const APoint &pos)
//{
//	PX2EU_MAN.Menu_Edit_Begin("BP", "BPParam");
//
//	PX2EU_MAN.Menu_Edit_AddItem("BP", "BPParam", "Disconnect", PX2_LM.V("Disconnect"), "n_DisconnectParam()");
//
//	PX2EU_MAN.Menu_Edit_EndPopUp("BP", pos);
//}
////----------------------------------------------------------------------------