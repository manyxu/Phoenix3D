// PX2EU_PropertyGridEdit.cpp

#include "PX2EU_PropertyGridEdit.hpp"
#include "PX2SelectionManager.hpp"
#include "PX2EditEventType.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2EditorEventType.hpp"
#include "PX2Edit.hpp"
#include "PX2ResourceManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIPropertyGrid, EU_PropertyGridEdit);
PX2_IMPLEMENT_STREAM(EU_PropertyGridEdit);
PX2_IMPLEMENT_FACTORY(EU_PropertyGridEdit);

//----------------------------------------------------------------------------
EU_PropertyGridEdit::EU_PropertyGridEdit()
{
	ComeInEventWorld();
	ShowRootItem(false);
}
//----------------------------------------------------------------------------
EU_PropertyGridEdit::~EU_PropertyGridEdit()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void EU_PropertyGridEdit::OnEvent(Event *event)
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
void EU_PropertyGridEdit::_RefreshSelect()
{
	int numSelObjs = PX2_SELECTM_E->GetNumObjects();
	if (1 == numSelObjs)
	{
		EditObject *editObj = 0;

		Object *firstObject = PX2_SELECTM_E->GetFirstObject();
		Terrain *terrain = DynamicCast<Terrain>(firstObject);
		if (terrain)
		{
			editObj = PX2_EDIT.GetTerrainEdit()->GetEditObjectTerrain();
		}

		if (!editObj)
		{
			_ClearSelect();
		}

		if (editObj == mObject)
			return;

		_ClearSelect();

		mObject = editObj;

		editObj->RemoveAllProperties();
		editObj->RegistProperties();
		const std::vector<Object::PropertyObject> &props = editObj->GetProperties();

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
					//but->AddUICallback(EditBoxStringButCallback);
					but->SetUserData("Object", editObj);
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
void EU_PropertyGridEdit::OnItemChanged(UIItem *item)
{
	std::string name = item->GetName();

	std::vector<Object::PropertyObject> &props = mObject->GetProperties();
	for (int i = 0; i < (int)props.size(); i++)
	{
		Object::PropertyObject &propObj = props[i];
		if (name == propObj.Tag)
		{			
			propObj.Data = item->GetValue();

			mObject->OnPropertyChanged(propObj);
		}
	}
}
//----------------------------------------------------------------------------
void EU_PropertyGridEdit::_ClearSelect()
{
	mObject = 0;
	RemoveAllProperties();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_PropertyGridEdit::EU_PropertyGridEdit(LoadConstructor value) :
UIPropertyGrid(value)
{
}
//----------------------------------------------------------------------------
void EU_PropertyGridEdit::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIPropertyGrid::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_PropertyGridEdit, source);
}
//----------------------------------------------------------------------------
void EU_PropertyGridEdit::Link(InStream& source)
{
	UIPropertyGrid::Link(source);
}
//----------------------------------------------------------------------------
void EU_PropertyGridEdit::PostLink()
{
	UIPropertyGrid::PostLink();
}
//----------------------------------------------------------------------------
bool EU_PropertyGridEdit::Register(OutStream& target) const
{
	if (UIPropertyGrid::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_PropertyGridEdit::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIPropertyGrid::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_PropertyGridEdit, target);
}
//----------------------------------------------------------------------------
int EU_PropertyGridEdit::GetStreamingSize(Stream &stream) const
{
	int size = UIPropertyGrid::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------