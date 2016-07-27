// PX2LuaScriptController.hpp

#ifndef PX2LUASCRIPTCONTROLLER_HPP
#define PX2LUASCRIPTCONTROLLER_HPP

#include "PX2UnityPre.hpp"
#include "PX2ScriptController.hpp"

namespace LuaPlus
{
	class LuaObject;
	class LuaState;
}

namespace PX2
{

	class PX2_ENGINE_ITEM LuaScriptController : public ScriptController
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(LuaScriptController);

	public:
		LuaScriptController();
		LuaScriptController(const std::string &filename, 
			const std::string &tableName);
		virtual ~LuaScriptController();

		virtual void ScriptOnAttached();
		virtual void ScriptOnDetach();
		virtual void ScriptOnInitUpdate();
		virtual void ScriptOnUpdate();
		virtual void ScriptOnFixUpdate();

		virtual void ScriptOnResetPlay();
		virtual void ScriptOnReset();
		virtual void ScriptOnPlay();
		virtual void ScriptOnStop();
		virtual void ScriptOnPlayUpdate();

		virtual void _Update(double applicationTime, double elapsedTime);

	private:
		static void RegisterScriptClass();
		static bool sIsClassRegisted;

	public:
		bool BuildCppDataFromScript(LuaPlus::LuaObject &scriptClass, 
			LuaPlus::LuaObject &constructionData);

		LuaPlus::LuaObject *mLO_OnAttached;
		LuaPlus::LuaObject *mLO_OnDetach;
		LuaPlus::LuaObject *mLO_OnInitUpdate;
		LuaPlus::LuaObject *mLO_OnUpdate;
		LuaPlus::LuaObject *mLO_OnFixUpdate;

		LuaPlus::LuaObject *mLO_OnResetPlay;
		LuaPlus::LuaObject *mLO_OnReset;
		LuaPlus::LuaObject *mLO_OnPlay;
		LuaPlus::LuaObject *mLO_OnStop;
		LuaPlus::LuaObject *mLO_OnPlayUpdate;

	public:
		LuaPlus::LuaObject *mLO_Self;

		LuaPlus::LuaState *mLuaState;
	};

	PX2_REGISTER_STREAM(LuaScriptController);
	typedef PointerRef<LuaScriptController> LuaScriptControllerPtr;

}

#endif