// PX2EU_ProjectTree.cpp

#include "PX2EU_ProjectTree.hpp"
#include "PX2EU_ProjectItem.hpp"
#include "PX2EU_Manager.hpp"
#include "PX2SelectionManager.hpp"
#include "PX2Project.hpp"
#include "PX2Scene.hpp"
#include "PX2SelectionManager.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2EditEventType.hpp"
#include "PX2EditorEventType.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2GraphicsEventData.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2Selection.hpp"
#include "PX2InputEvent.hpp"
#include "PX2InputEventData.hpp"
#include "PX2Edit.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UITree, EU_ProjectTree);
PX2_IMPLEMENT_STREAM(EU_ProjectTree);
PX2_IMPLEMENT_FACTORY(EU_ProjectTree);

//----------------------------------------------------------------------------
EU_ProjectTree::EU_ProjectTree() :
mShowType(ST_GENERAL)
{
	ComeInEventWorld();

	float levelAdjust = 0.0f;
	ShowRootItem(false);

	mItemProject = AddItem(mRootItem, "Project");
	mItemProject->GetFText()->GetText()->SetFontScale(0.8f);
	mItemProject->SetLevelAdjust(levelAdjust);

	mItemScene = AddItem(mItemProject, "Scene");
	mItemUI = AddItem(mItemProject, "UI");
	mItemBP = AddItem(mItemProject, "BluePrint");
}
//----------------------------------------------------------------------------
EU_ProjectTree::~EU_ProjectTree()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void EU_ProjectTree::OnEvent(Event *ent)
{
	if (ProjectES::IsEqual(ent, ProjectES::NewProject) ||
		ProjectES::IsEqual(ent, ProjectES::LoadedProject))
	{
		_RefreshProject();
	}
	else if (ProjectES::IsEqual(ent, ProjectES::CloseProject))
	{
		_ClearProject();
	}
	else if (ProjectES::IsEqual(ent, ProjectES::NewScene))
	{
		_RefreshScene();
	}
	else if (ProjectES::IsEqual(ent, ProjectES::CloseScene))
	{
		_ClearScene();
	}
	if (ProjectES::IsEqual(ent, ProjectES::NewUI))
	{
		_RefreshUI();
	}
	else if (ProjectES::IsEqual(ent, ProjectES::CloseUI))
	{
		_ClearUI();
	}
	else if (ProjectES::IsEqual(ent, ProjectES::NewBP))
	{
		_RefreshBP();
	}
	else if (ProjectES::IsEqual(ent, ProjectES::CloseBP))
	{
		_ClearBP();
	}

	if (GraphicsES::IsEqual(ent, GraphicsES::AddObject))
	{
		AddObjectData objData = ent->GetData<AddObjectData>();
		Object *parentObj = objData.ParentObj;
		
		bool isBP = false;
		BPPackage *bpPackage = DynamicCast<BPPackage>(parentObj);
		BPFile *bpFile = DynamicCast<BPFile>(parentObj);
		BPModule *bpModule = DynamicCast<BPModule>(parentObj);
		if (bpPackage || bpFile || bpModule)
			isBP = true;

		UIItem *itemParent = GetItemByObject(parentObj);
		if (itemParent)
		{
			const std::string &name = objData.Obj->GetName();
			UIItem *addedItem = AddItem(itemParent, name, name, objData.Obj);
			int expanedLevel = 0;
			_RefreshOnMoveable(addedItem, 
				DynamicCast<Movable>(objData.Obj), expanedLevel, isBP);
		}
	}
	else if (GraphicsES::IsEqual(ent, GraphicsES::RemoveObject))
	{
		Object *obj = ent->GetData<Object*>();
		if (obj)
		{
			// 当场景从Canvas中Detach掉时，也会触发该消息,屏蔽之
			UIItem *item = GetItemByObject(obj);
			if (item && item != mItemScene)
			{
				RemoveItem(item);
			}
		}
	}
	else if (EditorEventSpace::IsEqual(ent, EditorEventSpace::N_ObjectNameChanged))
	{
		Object *obj = ent->GetData<Object*>();
		UIItem *item = GetItemByObject(obj);
		if (item)
		{
			item->SetName(obj->GetName());
			item->GetFText()->GetText()->SetText(obj->GetName());
		}
	}
	else if (InputEventSpace::IsEqual(ent, InputEventSpace::MouseReleased))
	{
		ent->GetData<InputEventData>();
		const Rectf &rect = GetWorldRect();
	}
	else if (EditES::IsEqual(ent, EditES::RemoveAllSelects))
	{
		ClearAllSelectItems(false);
	}
	else if (EditES::IsEqual(ent, EditES::AddSelect))
	{
		Object *obj = ent->GetData<Object*>();
		UIItem *item = GetItemByObject(obj);
		if (item)
		{
			AddSelectItem(item, false, false);
		}
	}
}
//----------------------------------------------------------------------------
void EU_ProjectTree::OnUIPicked(const UIInputData &inputData)
{
	UITree::OnUIPicked(inputData);

	if (UIPT_RELEASED == inputData.PickType &&
		UIInputData::MouseTag::MT_RIGHT == inputData.TheMouseTag)
	{
		Object *firstObject = PX2_SELECTM_E->GetFirstObject();
		if (firstObject)
		{


			EU_Manager::EditMenuType emt = EU_Manager::EMT_SCENE;
			PX2EU_MAN.CreateEditMenu(inputData.WorldPos, emt);
		}
	}
}
//----------------------------------------------------------------------------
void EU_ProjectTree::SetShowType(ShowType st)
{
	mShowType = st;

	for (int i = 0; i < (int)mSelectedItems.size(); i++)
	{
		EU_ProjectItem *item = DynamicCast<EU_ProjectItem>(mSelectedItems[i]);
		item->SetShowType(st);
	}
}
//----------------------------------------------------------------------------
ShowType EU_ProjectTree::GetShowType() const
{
	return mShowType;
}
//----------------------------------------------------------------------------
void EU_ProjectTreeCallback(UIFrame *frame, UICallType type)
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
//----------------------------------------------------------------------------
UIItem *EU_ProjectTree::AddItem(UIItem *parentItem, const std::string &label,
	const std::string &name, Object *obj)
{
	EU_ProjectItem *item = new0 EU_ProjectItem();
	item->SetEU_ProjectTree(this);
	parentItem->AttachChild(item);
	item->SetName(name);
	item->SetSize(parentItem->GetSize());
	item->GetFText()->GetText()->SetText(label);
	item->SetItemObject(obj);

	UIButton *butBack = item->GetButBack();
	if (butBack)
	{
		butBack->SetMemUICallback(this,
			(UIFrame::MemUICallback)(&UITree::SelectCallback));
	}

	if (!name.empty())
	{
		mTagItems[name] = item;
	}

	item->GetFText()->GetText()->SetColor(Float3::WHITE);
	item->GetFText()->GetText()->SetColorSelfCtrled(true);
	item->GetFText()->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	item->GetFText()->GetText()->SetDrawStyle(FD_SHADOW);
	item->GetFText()->GetText()->SetBorderShadowAlpha(0.5f);

	UICheckButton *cb = parentItem->GetButArrow();
	if (!cb) cb = parentItem->CreateButArrow();

	cb->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture(
		"DataNIRVANA2/images/icons/tree/tree_expanded.png");
	cb->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetTexture(
		"DataNIRVANA2/images/icons/tree/tree_collapsed.png");

	cb->AddUICallback(EU_ProjectTreeCallback);
	cb->SetUserData("Item", parentItem);

	item->SetLevelAdjust(-0.25f);

	return item;
}
//----------------------------------------------------------------------------
void EU_ProjectTree::OnSelected(UIItem *item, bool isDouble)
{
	UITree::OnSelected(item, isDouble);

	if (item)
	{
		Object *obj = item->GetItemObject();

		if (obj)
		{
			PX2_SELECTM_E->Clear();
			PX2_SELECTM_E->AddObject(obj);
		}

		if (isDouble)
		{
			PX2_EDIT.FocusOnSelection();
		}
	}
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_RefreshProject()
{
	_ClearProject();

	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	mItemUI->RemoveAllChildItems();
	mItemUI->SetItemObject(PX2_PROJ.GetUI());
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_ClearProject()
{
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_RefreshScene()
{
	mItemScene->RemoveAllChildItems();

	Scene *scene = PX2_PROJ.GetScene();
	mItemScene->SetItemObject(scene);

	int expandLevel = 1;
	_RefreshOnMoveable(mItemScene, scene, expandLevel, false);
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_ClearScene()
{
	mItemScene->RemoveAllChildItems();
	mItemScene->SetItemObject(0);
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_RefreshUI()
{
	_ClearUI();

	UI *ui = PX2_PROJ.GetUI();
	mItemUI->SetItemObject(ui);

	int expandLevel = 1;
	_RefreshOnMoveable(mItemUI, ui, expandLevel, false);
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_ClearUI()
{
	mItemUI->RemoveAllChildItems();
	mItemUI->SetItemObject(0);
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_RefreshBP()
{
	_ClearBP();

	BPPackage *package = PX2_PROJ.GetBPPackage();
	mItemBP->SetItemObject(package);

	int expandLevel = 1;
	_RefreshOnMoveable(mItemBP, package, expandLevel, true);
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_ClearBP()
{
	mItemBP->RemoveAllChildItems();
	mItemBP->SetItemObject(0);
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_RefreshOnMoveable(UIItem *parentItem, 
	Movable *parentMov, int &expandLevel, bool isBP)
{
	Node *node = DynamicCast<Node>(parentMov);

	if (node)
	{
		parentItem->Expand(expandLevel > 0);
		expandLevel--;

		for (int i = 0; i < node->GetNumControllers(); i++)
		{
			Controller *ctrl = node->GetController(i);
			if (ctrl)
			{
				const std::string &name = ctrl->GetName();
				UIItem *item = AddItem(parentItem, name, "", ctrl);
			}
		}

		for (int i = 0; i < node->GetNumChildren(); i++)
		{
			Movable *mov = node->GetChild(i);
			BPPackage *package = DynamicCast<BPPackage>(mov);
			BPFile *file = DynamicCast<BPFile>(mov);
			BPModule *module = DynamicCast<BPModule>(mov);
			
			if (!isBP)
			{
				if (mov)
				{
					const std::string &name = mov->GetName();
					UIItem *item = AddItem(parentItem, name, "", mov);
					_RefreshOnMoveable(item, mov, expandLevel, isBP);
				}
			}
			else if (package || file || module)
			{
				const std::string &name = mov->GetName();
				UIItem *item = AddItem(parentItem, name, "", mov);
				_RefreshOnMoveable(item, mov, expandLevel, isBP);
			}
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_ProjectTree::EU_ProjectTree(LoadConstructor value) :
UITree(value)
{
}
//----------------------------------------------------------------------------
void EU_ProjectTree::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UITree::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_ProjectTree, source);
}
//----------------------------------------------------------------------------
void EU_ProjectTree::Link(InStream& source)
{
	UITree::Link(source);
}
//----------------------------------------------------------------------------
void EU_ProjectTree::PostLink()
{
	UITree::PostLink();
}
//----------------------------------------------------------------------------
bool EU_ProjectTree::Register(OutStream& target) const
{
	if (UITree::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_ProjectTree::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UITree::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_ProjectTree, target);
}
//----------------------------------------------------------------------------
int EU_ProjectTree::GetStreamingSize(Stream &stream) const
{
	int size = UITree::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------