// PX2EditObject.cpp

#include "PX2EditObject.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, EditObject);
PX2_IMPLEMENT_STREAM(EditObject);
PX2_IMPLEMENT_FACTORY(EditObject);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, EditObject);

//----------------------------------------------------------------------------
EditObject::EditObject()
{
}
//----------------------------------------------------------------------------
EditObject::~EditObject()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void EditObject::RegistProperties()
{
	Object::RegistProperties();

	AddPropertyClass("EditObject");
}
//----------------------------------------------------------------------------
void EditObject::OnPropertyChanged(const PropertyObject &obj)
{
	Object::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EditObject::EditObject(LoadConstructor value) :
Object(value)
{
}
//----------------------------------------------------------------------------
void EditObject::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EditObject, source);
}
//----------------------------------------------------------------------------
void EditObject::Link(InStream& source)
{
	Object::Link(source);
}
//----------------------------------------------------------------------------
void EditObject::PostLink()
{
	Object::PostLink();
}
//----------------------------------------------------------------------------
bool EditObject::Register(OutStream& target) const
{
	if (Object::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void EditObject::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EditObject, target);
}
//----------------------------------------------------------------------------
int EditObject::GetStreamingSize(Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------