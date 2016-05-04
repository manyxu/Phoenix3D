// PX2GeneralEventHandler.cpp

#include "PX2GeneralEventHandler.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2SimulationEventType.hpp"
#include "PX2EngineLoop.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2UIEvent.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
// ScriptHandler
//----------------------------------------------------------------------------
ScriptHandler::ScriptHandler()
{
}
//----------------------------------------------------------------------------
ScriptHandler::~ScriptHandler()
{
}
//----------------------------------------------------------------------------
ScriptHandler *ScriptHandler::Create(const std::string &careStr)
{
	ScriptHandler *sh = new0 ScriptHandler();
	sh->mCareStr = careStr;

	PX2_ENGINELOOP.GetGeneral_EventHandler()->AddScriptHandler(sh);

	return sh;
}
//----------------------------------------------------------------------------
void ScriptHandler::Call()
{
	for (int i = 0; i < (int)mCallStrs.size(); i++)
	{
		std::string callStr = mCallStrs[i] + "()";
		//PX2_SM.CallString(callStr.c_str());
	}
}
//----------------------------------------------------------------------------
void ScriptHandler::AddCall(const std::string &callStr)
{
	if (callStr.empty())
		return;

	mCallStrs.push_back(callStr);
}
//----------------------------------------------------------------------------
int ScriptHandler::GetNumCalls() const
{
	return (int)mCallStrs.size();
}
//----------------------------------------------------------------------------
const std::string &ScriptHandler::GetCall(int i) const
{
	return mCallStrs[i];
}
//----------------------------------------------------------------------------
void ScriptHandler::RemoveCall(const std::string &callStr)
{
	std::vector<std::string>::iterator it = mCallStrs.begin();
	for (; it != mCallStrs.end();)
	{
		if (callStr == *it)
		{
			it = mCallStrs.erase(it);
		}
		else
		{
			it++;
		}
	}
}
//----------------------------------------------------------------------------
void ScriptHandler::RemoveAllCalls()
{
	mCallStrs.clear();
}
//----------------------------------------------------------------------------
const std::string &ScriptHandler::GetCareStr() const
{
	return mCareStr;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SimuES_Handler
//----------------------------------------------------------------------------
std::string General_EventHandler::sGEStrings[SimuES::E_QUANTITY] =
{
	"NewProject",
	"LoadedProject",
	"SavedProject",
	"CloseProject",
	"NewScene",
	"SavedScene",
	"CloseScene",
	"NewUI",
	"CloseUI",
	"NewBP",
	"LoadedBP",
	"CloseBP",

	"Play",

	"ButReleased",

	"GeneralString",
	"Character_Die",
	"Actor_Delete",
	"Scene_ViewPortProjectChanged",
	"Scene_BloomChanged",
	"Scene_ShadowMapChange"
};
//----------------------------------------------------------------------------
General_EventHandler::General_EventHandler()
{
	mCompareGEStrings.resize(SimuES::E_QUANTITY);

	for (int i = 0; i < SimuES::E_QUANTITY; i++)
	{
		mCompareGEStrings[i] = sGEStrings[i].c_str();
	}
}
//----------------------------------------------------------------------------
General_EventHandler::~General_EventHandler()
{
}
//----------------------------------------------------------------------------
void General_EventHandler::DoEnter()
{
}
//----------------------------------------------------------------------------
void General_EventHandler::DoExecute(Event *event)
{
	Event::EventType eventType = event->GetEventType();
	PX2_UNUSED(eventType);

	ScriptManager *sm = ScriptManager::GetSingletonPtr();
	if (sm)
	{
		//sm->CallString("generalOnEvent");
	}

	if (SimuES::IsIn(event))
	{
		Event::EventType coverType = SimuES::Cover(event);

		const FString &keyStr = mCompareGEStrings[coverType];
		std::map<FString, std::list<ScriptHandlerPtr> >::iterator it = 
			mScriptHandlers.find(keyStr);
		
		if (it != mScriptHandlers.end())
		{
			std::list<ScriptHandlerPtr> &itList = it->second;
			std::list<ScriptHandlerPtr>::iterator it1 = itList.begin();
			for (; it1 != itList.end(); it1++)
			{
				(*it1)->Call();
			}
		}
	}

	//if (UIES::IsEqual(event, UIES::ButReleased))
	//{
	//	UIButton *but = event->GetData<UIButton*>();
	//	PX2_BPM.SetReleasedBut(but);

	//	Event *ent1 = SimuES::CreateEventX(SimuES::ButReleased);
	//	PX2_EW.BroadcastingLocalEvent(ent1);
	//}
}
//----------------------------------------------------------------------------
void General_EventHandler::DoLeave()
{
}
//----------------------------------------------------------------------------
void General_EventHandler::AddScriptHandler(ScriptHandler *handler)
{
	mScriptHandlers[handler->GetCareStr().c_str()].push_back(handler);
}
//----------------------------------------------------------------------------
void General_EventHandler::RemoveAllHandlers()
{
	mScriptHandlers.clear();
}
//----------------------------------------------------------------------------