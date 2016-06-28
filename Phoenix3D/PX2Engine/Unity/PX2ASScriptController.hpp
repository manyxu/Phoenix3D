// PX2ASScriptController.hpp

#ifndef PX2ASSCRIPTCONTROLLER_HPP
#define PX2ASSCRIPTCONTROLLER_HPP

#include "PX2UnityPre.hpp"
#include "PX2ScriptController.hpp"

class asIScriptObject;

namespace PX2
{

	class PX2_ENGINE_ITEM ASScriptController : public ScriptController
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(ASScriptController);

	public:
		ASScriptController();
		virtual ~ASScriptController();

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

		asIScriptObject *ASIScriptObject;
	};

	PX2_REGISTER_STREAM(ASScriptController);
	typedef PointerRef<ASScriptController> ASScriptControllerPtr;

}

#endif