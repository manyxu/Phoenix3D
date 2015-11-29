// PX2SimulationEventHandler.cpp

#include "PX2SimulationEventHandler.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2SimulationEventType.hpp"
#include "PX2EngineLoop.hpp"
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

	PX2_ENGINELOOP.GetSimuES_EventHandler()->AddScriptHandler(sh);

	return sh;
}
//----------------------------------------------------------------------------
void ScriptHandler::Call()
{
	for (int i = 0; i < (int)mCallStrs.size(); i++)
	{
		std::string callStr = mCallStrs[i] + "()";
		PX2_SM.CallString(callStr.c_str());
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
std::string SimuES_EventHandler::sGEStrings[SimuES::E_QUANTITY] =
{
	"SimuES_E_QUANTITY"
};
//----------------------------------------------------------------------------
SimuES_EventHandler::SimuES_EventHandler()
{
	mCompareGEStrings.resize(SimuES::E_QUANTITY);

	for (int i = 0; i < SimuES::E_QUANTITY; i++)
	{
		mCompareGEStrings[i] = sGEStrings[i].c_str();
	}
}
//----------------------------------------------------------------------------
SimuES_EventHandler::~SimuES_EventHandler()
{
}
//----------------------------------------------------------------------------
void SimuES_EventHandler::DoEnter()
{
}
//----------------------------------------------------------------------------
void SimuES_EventHandler::DoExecute(Event *event)
{
	if (SimuES::IsIn(event))
	{
		Event::EventType coverType = SimuES::Cover(event);

		const FString &keyStr = mCompareGEStrings[coverType];
		std::map<FString, std::list<ScriptHandlerPtr> >::iterator it = 
			mScriptHandlers.find(keyStr);
		
		if (it != mScriptHandlers.end())
		{
			std::list<ScriptHandlerPtr> &itList = it->second;
			std::list<ScriptHandlerPtr>::iterator it = itList.begin();
			for (; it != itList.end(); it++)
			{
				(*it)->Call();
			}
		}
	}
}
//----------------------------------------------------------------------------
void SimuES_EventHandler::DoLeave()
{
}
//----------------------------------------------------------------------------
void SimuES_EventHandler::AddScriptHandler(ScriptHandler *handler)
{
	mScriptHandlers[handler->GetCareStr().c_str()].push_back(handler);
}
//----------------------------------------------------------------------------
void SimuES_EventHandler::RemoveAllHandlers()
{
	mScriptHandlers.clear();
}
//----------------------------------------------------------------------------