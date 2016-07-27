// PX2EU_PropertyGrid.cpp

#include "PX2EU_PropertyGrid.hpp"

#include "PX2SelectionManager.hpp"
#include "PX2EditEventType.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2EditorEventType.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIPropertyGrid, EU_PropertyGrid);
PX2_IMPLEMENT_STREAM(EU_PropertyGrid);
PX2_IMPLEMENT_FACTORY(EU_PropertyGrid);

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
			AddProperty(propObj.Tag, propObj.Name, propObj.Type, propObj.Data,
				propObj.Enable);
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
	std::string name = item->GetName();

	std::vector<Object::PropertyObject> &props = mObject->GetProperties();

	for (int i = 0; i < (int)props.size(); i++)
	{
		Object::PropertyObject &propObj = props[i];

		if (propObj.Tag == name)
		{
			propObj.Data = item->GetValue();
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