// PX2GeneralEventHandler.hpp

#ifndef PX2GENERALEVENTHANDLER_HPP
#define PX2GENERALEVENTHANDLER_HPP

#include "PX2SimulationEventType.hpp"
#include "PX2SmartPointer.hpp"

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

	class PX2_EXTENDS_ITEM General_EventHandler : public EventHandler
	{
	public:
		General_EventHandler();
		virtual ~General_EventHandler();

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
	typedef Pointer0<General_EventHandler> General_EventHandlerPtr;

}

#endif