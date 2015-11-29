// PX2SumulationEventHandler.hpp

#ifndef PX2SIMULATIONEVENTHANDLER_HPP
#define PX2SIMULATIONEVENTHANDLER_HPP

#include "PX2SimulationEventType.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM ScriptHandler
	{
	public:
		ScriptHandler();
		~ScriptHandler();

		static ScriptHandler *Create(const std::string &careStr);

		void AddCall(const std::string &callStr);
		int GetNumCalls() const;
		const std::string &GetCall(int i) const;
		void RemoveCall(const std::string &callStr);
		void RemoveAllCalls();

		void Call();

		const std::string &GetCareStr() const;

	protected:
		std::string mCareStr;
		std::vector<std::string> mCallStrs;
	};
	typedef Pointer0<ScriptHandler> ScriptHandlerPtr;

	class PX2_EXTENDS_ITEM SimuES_EventHandler : public EventHandler
	{
	public:
		SimuES_EventHandler();
		virtual ~SimuES_EventHandler();

		virtual void DoEnter();
		virtual void DoExecute(Event *event);
		virtual void DoLeave();

		void AddScriptHandler(ScriptHandler *handler);
		void RemoveAllHandlers();

		static std::string sGEStrings[SimuES::E_QUANTITY];

	protected:
		std::vector<FString> mCompareGEStrings;
		std::map<FString, std::list<ScriptHandlerPtr> > mScriptHandlers;
	};
	typedef Pointer0<SimuES_EventHandler> SimuES_EventHandlerPtr;

}

#endif