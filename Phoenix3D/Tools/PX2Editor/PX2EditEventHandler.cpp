// PX2EditEventHandler.cpp

#include "PX2EditEventHandler.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
EditEventHandler::EditEventHandler()
{

}
//----------------------------------------------------------------------------
EditEventHandler::~EditEventHandler()
{
}
//----------------------------------------------------------------------------
void EditEventHandler::OnEvent(Event *event)
{
	ScriptManager *sm = ScriptManager::GetSingletonPtr();
	if (sm)
	{
		//int entType = event->GetEventType();
		//std::string callStr = "n_EditOnEvent(" + 
		//	StringHelp::IntToString(entType) + ")";
		//sm->CallString(callStr);
	}
}
//----------------------------------------------------------------------------