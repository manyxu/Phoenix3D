// PX2ScriptManager.hpp

#ifndef PX2SCRIPTMANAGER_HPP
#define PX2SCRIPTMANAGER_HPP

#include "PX2UnityPre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2ScriptContext.hpp"
#include "PX2ScriptController.hpp"

namespace PX2
{

	class PX2_FOUNDATION_ITEM ScriptManager : public Singleton <ScriptManager>
	{
	public:
		ScriptManager();
		virtual ~ScriptManager();

		ScriptController *CreateScriptController(const std::string &filename);

		ScriptContext *GetContext(ScriptContext::ContextType type);
		
		void Ternimate(ScriptContext::ContextType type);
		void TernimateAll();

	protected:
		ScriptContextPtr mContextLua;
		ScriptContextPtr mContextAS;
	};

#define PX2_SM ScriptManager::GetSingleton()
#define PX2_SC_LUA ScriptManager::GetSingleton().GetContext(ScriptContext::CT_LUA)
#define PX2_SC_AS ScriptManager::GetSingleton().GetContext(ScriptContext::CT_ANGELSCRIPT)

}

#endif