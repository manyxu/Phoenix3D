// PX2EU_PropertyGrid.cpp

#include "PX2EU_PropertyGrid.hpp"
#include "PX2SelectionManager.hpp"
#include "PX2EditEventType.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2EditorEventType.hpp"
#include "PX2Edit.hpp"
#include "PX2ResourceManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIPropertyGrid, EU_PropertyGrid);
PX2_IMPLEMENT_STREAM(EU_PropertyGrid);
PX2_IMPLEMENT_FACTORY(EU_PropertyGrid);

//----------------------------------------------------------------------------
void EditBoxStringButCallback(UIFrame *frame, UICallType type)
{
	UIButton *but = DynamicCast<UIButton>(frame);
	if (!but) return;

	if (UICallType::UICT_RELEASED == type)
	{
		Object *itemObj = but->GetUserData<Object*>("UIPropertyItem");
		UIPropertyItem *item = DynamicCast<UIPropertyItem>(itemObj);
		Object *obj = but->GetUserData<Object*>("Object");
		Object::PropertyObject po = but->GetUserData<Object::PropertyObject>("PropertyObject");

		if (item)
		{
			const SelectResData &data = PX2_EDIT.GetSelectedResource();
			SelectResData::SelectResType selectResType = data.GetSelectResType();
			if (data.ResPathname.empty()) return;

			if (SelectResData::RT_NORMAL == selectResType)
			{
				item->SetValue(data.ResPathname);
			}
			else if (SelectResData::RT_TEXPACKELEMENT == selectResType)
			{
				const TexPack &texPack = PX2_RM.GetTexPack(data.ResPathname);

				if (texPack.IsValid())
				{
					item->SetValue(data.EleName);
				}
				else if (PX2_RM.AddTexPack(data.ResPathname))
				{
					item->SetValue(data.EleName);
				}
			}

			UIPropertyGrid *grid =
				item->GetFirstParentDerivedFromType<UIPropertyGrid>();
			if (grid)
			{
				grid->OnItemChanged(item);
			}				
		}
	}
}
//----------------------------------------------------------------------------
EU_PropertyGrid::EU_PropertyGrid()
{
	ComeInEventWorld();
	ShowRootItem(false);
}
//----------------------------------------------------------------------------
EU_PropertyGrid::~EU_PropertyGrid()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void EU_PropertyGrid::OnEvent(Event *event)
{
	if (EditES::IsEqual(event, EditES::AddSelect))
	{
		_RefreshSelect();
	}
	else if (EditES::IsEqual(event, EditES::RemoveSelect))
	{
		_RefreshSelect();
	}
	else if (EditES::IsEqual(event, EditES::RemoveAllSelects))
	{
		_ClearSelect();
	}
}
//----------------------------------------------------------------------------
void EU_PropertyGrid::_RefreshSelect()
{
	int numSelObjs = PX2_SELECTM_E->GetNumObjects();
	if (1 == numSelObjs)
	{
		Object *obj = PX2_SELECTM_E->GetFirstObject();
		if (obj == mObject)
			return;

		_ClearSelect();

		mObject = obj;

		obj->RemoveAllProperties();
		obj->RegistProperties();
		const std::vector<Object::PropertyObject> &props = obj->GetProperties();

		for (int i = 0; i < (int)props.size(); i++)
		{
			const Object::PropertyObject &propObj = props[i];
			UIItem *item = AddProperty(propObj.Tag, propObj.Name, propObj.Type,
				propObj.Data, propObj.Data1, propObj.SliderMin, propObj.SliderMax, 
				propObj.Enable);

			if (Object::PropertyType::PT_STRINGBUTTON == propObj.Type)
			{
				UIButton *but = DynamicCast<UIButton>(
					item->GetObjectByName("Button"));
				if (but)
				{
					but->AddUICallback(EditBoxStringButCallback);
					but->SetUserData("Object", obj);
					but->SetUserData("PropertyObject", propObj);
				}
			}
		}
	}
	else
	{
		_ClearSelect();
	}
}
//----------------------------------------------------------------------------
void EU_PropertyGrid::OnItemChanged(UIItem *item)
{
	std::string name;
	std::string itemName = item->GetName();
	bool isFindSub = (itemName.find("_Sub")!=std::string::npos);
	if (isFindSub)
	{
		name = itemName.substr(0, itemName.length() - 5);
	}
	else
	{
		name = itemName;
	}

	std::vector<Object::PropertyObject> &props = mObject->GetProperties();

	for (int i = 0; i < (int)props.size(); i++)
	{
		Object::PropertyObject &propObj = props[i];
		if (name == propObj.Tag)
		{
			if ("LocalTransform" == propObj.Name || "WorldTransform" == propObj.Name)
			{
				UIItem *parent = DynamicCast<UIItem>(item->GetParent());
				if (parent)
				{
					Transform trans = PX2_ANY_AS(parent->GetValue(), Transform);
					APoint val = PX2_ANY_AS(item->GetValue(), APoint);

					std::string subTag = item->GetUserData<std::string>("sub_tag");

					if ("pos" == subTag)
					{
						trans.SetTranslate(val);
					}
					else if ("rotate" == subTag)
					{
						trans.SetRotate(val);
					}
					else if ("scale" == subTag)
					{
						trans.SetScale(val);
					}
					parent->SetValue(trans);
					propObj.Data = trans;
				}
			}
			else
			{
				propObj.Data = item->GetValue();
			}

			mObject->OnPropertyChanged(propObj);

			if (propObj.Name == "Name")
			{
				Event *ent = PX2_CREATEEVENT(EditorEventSpace, N_ObjectNameChanged);
				ent->SetData<Object*>((Object*)mObject);
				PX2_EW.BroadcastingLocalEvent(ent);
			}
		}
	}
}
//----------------------------------------------------------------------------
void EU_PropertyGrid::_ClearSelect()
{
	mObject = 0;
	RemoveAllProperties();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_PropertyGrid::EU_PropertyGrid(LoadConstructor value) :
UIPropertyGrid(value)
{
}
//----------------------------------------------------------------------------
void EU_PropertyGrid::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIPropertyGrid::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_PropertyGrid, source);
}
//----------------------------------------------------------------------------
void EU_PropertyGrid::Link(InStream& source)
{
	UIPropertyGrid::Link(source);
}
//----------------------------------------------------------------------------
void EU_PropertyGrid::PostLink()
{
	UIPropertyGrid::PostLink();
}
//----------------------------------------------------------------------------
bool EU_PropertyGrid::Register(OutStream& target) const
{
	if (UIPropertyGrid::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_PropertyGrid::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIPropertyGrid::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_PropertyGrid, target);
}
//----------------------------------------------------------------------------
int EU_PropertyGrid::GetStreamingSize(Stream &stream) const
{
	int size = UIPropertyGrid::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------