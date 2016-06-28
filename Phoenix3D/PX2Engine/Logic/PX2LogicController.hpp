// PX2LogicController.hpp

#ifndef PX2LOGICCONTROLLER_HPP
#define PX2LOGICCONTROLLER_HPP

#include "PX2SimulationPre.hpp"
#include "PX2ScriptController.hpp"
#include "PX2LogicModule.hpp"

namespace PX2
{

	class PX2_GAME_ITEM LogicController : public ScriptController
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(LogicController);

	public:
		LogicController();
		virtual ~LogicController();

		virtual void FixUpdate();

	protected:
		LogicModulePtr mModule;
	};

	PX2_REGISTER_STREAM(LogicController);
	typedef PointerRef<LogicController> LogicControllerPtr;

}

#endif