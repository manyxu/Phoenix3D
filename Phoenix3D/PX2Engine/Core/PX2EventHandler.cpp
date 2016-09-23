// PX2EventHandler.cpp

#include "PX2Event.hpp"
#include "PX2EventHandler.hpp"
#include "PX2EventWorld.hpp"
using namespace PX2;

EventHandler::EventHandler ()
:
mEntered(false),
mEnableUpdate(true),
mWorld(0)
{
	mChannel.Fill();
}
//----------------------------------------------------------------------------
EventHandler::~EventHandler()
{
}
//----------------------------------------------------------------------------
void EventHandler::SetChannel (const EventChannel &channel)
{
	mChannel = channel;
}
//----------------------------------------------------------------------------
const EventChannel &EventHandler::GetChannel () const
{
	return mChannel;
}
//----------------------------------------------------------------------------
void EventHandler::MergeChannel (const EventChannel &channel)
{
	mChannel.Merge(channel);
}
//----------------------------------------------------------------------------
void EventHandler::DisMergeChannel (const EventChannel &channel)
{
	mChannel.DisMerge(channel);
}
//----------------------------------------------------------------------------
void EventHandler::ClearChannel ()
{
	mChannel.Clear();
}
//----------------------------------------------------------------------------
bool EventHandler::IsListening (const EventChannel &channel)
{
	return mChannel.IsListening(channel);
}
//----------------------------------------------------------------------------
EventWorld* EventHandler::GetWorld ()
{
	return mWorld;
}
//----------------------------------------------------------------------------
bool EventHandler::IsInWorld () const
{
	return (mWorld != 0);
}
//----------------------------------------------------------------------------
bool EventHandler::IsInWorld (EventWorld* world) const
{
	if (world)
		return (mWorld == world);

	return false;
}
//----------------------------------------------------------------------------
bool EventHandler::IsInOrIningWorld(EventWorld *world) const
{
	if (IsInWorld(world))
		return true;

	if (world->IsInComingIn(this))
		return true;

	return false;
}
//----------------------------------------------------------------------------
void EventHandler::OnEnter ()
{
}
//----------------------------------------------------------------------------
void EventHandler::OnEvent (Event *event)
{
	PX2_UNUSED(event);
}
//----------------------------------------------------------------------------
void EventHandler::OnLeave ()
{
}
//----------------------------------------------------------------------------
void EventHandler::Execute (Event* event)
{
	assertion(event!=0, "event must exist.");
	if (mEntered)
	{
		OnEvent(event);
	}
}
//----------------------------------------------------------------------------
void EventHandler::Enter (EventWorld* world)
{
	mWorld = world;
	mEntered = true;
	OnEnter();
}
//----------------------------------------------------------------------------
void EventHandler::Leave ()
{
	mEntered = false;
	OnLeave();
	mWorld = 0;
}
//----------------------------------------------------------------------------