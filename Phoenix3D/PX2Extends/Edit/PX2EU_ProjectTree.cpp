// PX2EU_ProjectTree.cpp

#include "PX2EU_ProjectTree.hpp"
#include "PX2SimulationEventType.hpp"
#include "PX2Project.hpp"
#include "PX2Scene.hpp"
#include "PX2SelectionManager.hpp"
#include "PX2EditEventType.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2GraphicsEventData.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2EU_ProjectItem.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UITree, EU_ProjectTree);
PX2_IMPLEMENT_STREAM(EU_ProjectTree);
PX2_IMPLEMENT_FACTORY(EU_ProjectTree);

//----------------------------------------------------------------------------
EU_ProjectTree::EU_ProjectTree() :
mShowType(ST_GENERAL)
{
	ShowRootItem(false);
	ComeInEventWorld();

	float levelAdjust = -0.5f;

	mItemProject = AddItem(mRootItem, "Project");
	mItemProject->GetFText()->GetText()->SetFontScale(0.8f);

	mItemSetting = AddItem(mItemProject, "Setting");
	mItemSetting->SetLevelAdjust(levelAdjust);
	AddItem(mItemSetting, "Set");
	AddItem(mItemSetting, "Build");

	mItemSceneCanvas = AddItem(mItemProject, "SceneCanvas");
	mItemSceneCanvas->SetLevelAdjust(levelAdjust);
	mItemSceneCanvas->GetFText()->GetText()->SetFontColor(Float3::MakeColor(0, 162, 232));

	mItemUICanvas = AddItem(mItemProject, "UICanvas");
	mItemUICanvas->SetLevelAdjust(levelAdjust);
	mItemUICanvas->GetFText()->GetText()->SetFontColor(Float3::MakeColor(34, 177, 76));
}
//----------------------------------------------------------------------------
EU_ProjectTree::~EU_ProjectTree()
{
	GoOutEventWorld();
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
	if (!cb)
		cb = parentItem->CreateButArrow();

	cb->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture(
		"DataNIRVANA2/images/icons/tree/tree_expanded.png");
	cb->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetTexture(
		"DataNIRVANA2/images/icons/tree/tree_collapsed.png");

	cb->SetUICallback(EU_ProjectTreeCallback);
	cb->SetUserData("Item", parentItem);

	item->SetLevelAdjust(-0.25f);

	return item;
}
//----------------------------------------------------------------------------
void EU_ProjectTree::DoExecute(Event *ent)
{
		if (SimuES::IsEqual(ent, SimuES::NewProject) ||
			SimuES::IsEqual(ent, SimuES::LoadedProject))
		{
			_RefreshProject();
		}
		else if (SimuES::IsEqual(ent, SimuES::CloseProject))
		{
			_ClearProject();
		}
		else if (SimuES::IsEqual(ent, SimuES::NewScene))
		{
			_RefreshScene();
		}
		else if (SimuES::IsEqual(ent, SimuES::CloseScene))
		{
			_ClearScene();
		}
		if (SimuES::IsEqual(ent, SimuES::NewUI))
		{
			_RefreshUI();
		}
		else if (SimuES::IsEqual(ent, SimuES::CloseUI))
		{
			_ClearUI();
		}
	
	if (GraphicsES::IsEqual(ent, GraphicsES::AddObject))
	{
		AddObjectData objData = ent->GetData<AddObjectData>();
		Object *parentObj = objData.ParentObj;
		UIItem *itemParent = GetItemByObject(parentObj);
		if (itemParent)
		{
			const std::string &name = objData.Obj->GetName();
			UIItem *addedItem = AddItem(itemParent, name, name, objData.Obj);
			_RefreshOnMoveableScene(addedItem, DynamicCast<Movable>(objData.Obj));
		}
	}
	else if (GraphicsES::IsEqual(ent, GraphicsES::RemoveObject))
	{
		Object *obj = ent->GetData<Object*>();
		if (obj)
		{
			UIItem *item = GetItemByObject(obj);
			if (item)
			{
				RemoveItem(item);
			}
		}
	}
	else if (EditEventSpace::IsEqual(ent, EditEventSpace::N_ObjectNameChanged))
	{
		Object *obj = ent->GetData<Object*>();
		UIItem *item = GetItemByObject(obj);
		if (item)
		{
			item->SetName(obj->GetName());
			item->GetFText()->GetText()->SetText(obj->GetName());
		}
	}
}
//----------------------------------------------------------------------------
void EU_ProjectTree::OnSelected(UIItem *item)
{
	Object *obj = item->GetItemObject();

	if (obj)
	{
		PX2_SELECTM_E->Clear();
		PX2_SELECTM_E->AddObject(obj);
	}
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_RefreshProject()
{
	_ClearProject();

	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	mItemUICanvas->RemoveAllChildItems();
	mItemUICanvas->SetItemObject(PX2_PROJ.GetSceneCanvas());
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_ClearProject()
{
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_RefreshScene()
{
	mItemSceneCanvas->RemoveAllChildItems();

	Scene *scene = PX2_PROJ.GetScene();
	UIItem *sceneItem = AddItem(mItemSceneCanvas, scene->GetName(),
		scene->GetName(), scene);
	_RefreshOnMoveableScene(sceneItem, scene);
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_RefreshOnMoveableScene(UIItem *parentItem, Movable *mov)
{
	Node *node = DynamicCast<Node>(mov);

	if (node)
	{
		for (int i = 0; i < node->GetNumChildren(); i++)
		{
			Movable *mov = node->GetChild(i);
			Actor *actor = DynamicCast<Actor>(mov);

			if (actor)
			{
				std::string name = mov->GetName();
				if (name.empty())
					name = "Empty";

				UIItem *item = AddItem(parentItem, name, "", mov);
				_RefreshOnMoveableScene(item, mov);
				item->Expand(false);
			}
		}
	}
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_ClearScene()
{
	if (mItemSceneCanvas)
		mItemSceneCanvas->RemoveAllChildItems();
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_RefreshUI()
{
	_ClearUI();

	UICanvas *uiCanvas = PX2_PROJ.GetUICanvas();
	mItemUICanvas->SetItemObject(uiCanvas);

	UIFrame *uiFrame = PX2_PROJ.GetUIFrame();
	UIItem *uiItem = AddItem(mItemUICanvas, uiFrame->GetName(),
		uiFrame->GetName(), uiFrame);

	//_RefreshOnMoveable(uiItem, uiFrame);
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_ClearUI()
{
	if (mItemUICanvas)
		mItemUICanvas->RemoveAllChildItems();
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