// PX2EU_ProjectTree.cpp

#include "PX2EU_ProjectTree.hpp"
#include "PX2SimulationEventType.hpp"
#include "PX2Project.hpp"
#include "PX2Scene.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UITree, EU_ProjectTree);
PX2_IMPLEMENT_STREAM(EU_ProjectTree);
PX2_IMPLEMENT_FACTORY(EU_ProjectTree);

//----------------------------------------------------------------------------
EU_ProjectTree::EU_ProjectTree(ProjectTreeType ptt) :
mProjectTreeType(ptt)
{
	ShowRootItem(false);
	ComeInEventWorld();

	if (PTT_PROJECT == mProjectTreeType)
	{
		UIItem *itemProject = AddItem(mRootItem, "Project");
		itemProject->GetButBack()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
			->SetColor(Float3::MakeColor(255, 127, 35));
		itemProject->GetButBack()->GetPicBoxAtState(UIButtonBase::BS_HOVERED)
			->SetColor(Float3::MakeColor(255, 127, 35));
		itemProject->GetButBack()->GetPicBoxAtState(UIButtonBase::BS_HOVERED)
			->SetBrightness(1.2f);

		UIItem *itemBluePrints = AddItem(mRootItem, "BluePrints");
		itemBluePrints->GetButBack()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
			->SetColor(Float3::MakeColor(255, 201, 14));
		itemBluePrints->GetButBack()->GetPicBoxAtState(UIButtonBase::BS_HOVERED)
			->SetColor(Float3::MakeColor(255, 127, 35));
		itemBluePrints->GetButBack()->GetPicBoxAtState(UIButtonBase::BS_HOVERED)
			->SetBrightness(1.2f);
	}
	else if (PTT_SCENE == mProjectTreeType)
	{
		UIItem *itemProject = AddItem(mRootItem, "Scene");
		itemProject->GetButBack()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
			->SetColor(Float3::MakeColor(0, 162, 232));
		itemProject->GetButBack()->GetPicBoxAtState(UIButtonBase::BS_HOVERED)
			->SetColor(Float3::MakeColor(0, 162, 232));
		itemProject->GetButBack()->GetPicBoxAtState(UIButtonBase::BS_HOVERED)
			->SetBrightness(1.2f);
	}
	else if (PTT_UI == mProjectTreeType)
	{
		UIItem *itemProject = AddItem(mRootItem, "UI");
		itemProject->GetButBack()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
			->SetColor(Float3::MakeColor(34, 177, 76));
		itemProject->GetButBack()->GetPicBoxAtState(UIButtonBase::BS_HOVERED)
			->SetColor(Float3::MakeColor(34, 177, 76));
		itemProject->GetButBack()->GetPicBoxAtState(UIButtonBase::BS_HOVERED)
			->SetBrightness(1.2f);
	}		
}
//----------------------------------------------------------------------------
EU_ProjectTree::~EU_ProjectTree()
{
	GoOutEventWorld();
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
	
	if (PTT_SCENE == mProjectTreeType)
	{
		if (SimuES::IsEqual(ent, SimuES::NewScene))
		{
			_RefreshScene();
		}
		else if (SimuES::IsEqual(ent, SimuES::LoadedScene))
		{
			_RefreshScene();
		}
		else if (SimuES::IsEqual(ent, SimuES::CloseScene))
		{
			_ClearScene();
		}
	}
	else if (PTT_UI == mProjectTreeType)
	{
		if (SimuES::IsEqual(ent, SimuES::NewUI))
		{
			_RefreshUI();
		}
		else if (SimuES::IsEqual(ent, SimuES::LoadedUI))
		{
			_RefreshUI();
		}
		else if (SimuES::IsEqual(ent, SimuES::CloseUI))
		{
			_ClearUI();
		}
	}
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_RefreshProject()
{
	_ClearProject();

	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	AddItem(mRootItem, proj->GetName());
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_ClearProject()
{
	RemoveAllItemsExceptRoot();
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_RefreshScene()
{
	RemoveAllItemsExceptRoot();

	Scene *scene = PX2_PROJ.GetScene();
	_RefreshOnMoveable(mRootItem, scene);
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_RefreshOnMoveable(UIItem *parentItem, Movable *mov)
{
	Node *node = DynamicCast<Node>(mov);

	if (node)
	{
		for (int i = 0; i < node->GetNumChildren(); i++)
		{
			Movable *mov = node->GetChild(i);
			if (mov)
			{
				std::string name = mov->GetName();
				if (name.empty())
					name = "Empty";

				UIItem *item = AddItem(parentItem, name);
				_RefreshOnMoveable(item, mov);
			}
		}
	}
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_ClearScene()
{
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_RefreshUI()
{
	RemoveAllItemsExceptRoot();

	AddItem(mRootItem, "UICanvas");
}
//----------------------------------------------------------------------------
void EU_ProjectTree::_ClearUI()
{

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