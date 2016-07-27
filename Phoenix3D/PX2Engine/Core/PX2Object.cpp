// PX2Object.cpp

#include "PX2Object.hpp"
#include "PX2EventHandlerObject.hpp"
#include "PX2Memory.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Object::Object () :
mIsEnable(true),
mIsActivated(true),
mReadedVersion(0),
mCurStream(NULL),
mIsNameChangeable(true),
mID(0)
{
	mEventHandler = new0 ObjectEventHandler(this);
}
//----------------------------------------------------------------------------
Object::~Object ()
{
	if (mEventHandler->IsInWorld())
		GoOutEventWorld();

	if (0 != mEventHandler)
		delete0(mEventHandler);
}
//----------------------------------------------------------------------------
void Object::Enable(bool enable)
{
	mIsEnable = enable;
}
//----------------------------------------------------------------------------
bool Object::IsEnable() const
{
	return mIsEnable;
}
//----------------------------------------------------------------------------
void Object::SetActivate(bool act)
{
	mIsActivated = act;
}
//----------------------------------------------------------------------------
bool Object::IsActivated() const
{
	return mIsActivated;
}
//----------------------------------------------------------------------------
Object *Object::New()
{
	assertion(false, "No New");
	return 0;
}
//----------------------------------------------------------------------------
Object *Object::New(const std::string &name)
{
	assertion(false, "No New");

	PX2_UNUSED(name);
	return 0;
}
//----------------------------------------------------------------------------