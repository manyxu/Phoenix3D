// PX2ScriptController.hpp

#ifndef PX2SCRIPTCONTROLLER_HPP
#define PX2SCRIPTCONTROLLER_HPP

#include "PX2UnityPre.hpp"
#include "PX2Controller.hpp"

namespace PX2
{

	class PX2_FOUNDATION_ITEM ScriptController : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(ScriptController);

	public:
		ScriptController();
		virtual ~ScriptController();
	};

	PX2_REGISTER_STREAM(ScriptController);
	typedef PointerRef<ScriptController> ScriptControllerPtr;

}

#endif