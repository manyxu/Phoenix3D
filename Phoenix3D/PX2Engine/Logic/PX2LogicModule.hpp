// PX2LogicModule.hpp

#ifndef PX2LOGICMODULE_HPP
#define PX2LOGICMODULE_HPP

#include "PX2SimulationPre.hpp"
#include "PX2Object.hpp"
#include "PX2FunObject.hpp"

namespace PX2
{
		
	class PX2_GAME_ITEM LogicModule : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;

	public:
		LogicModule();
		~LogicModule();

		enum Type
		{
			T_NONE,
			T_FUNCTION,
			T_FRAMELOOP,
			T_MAX_TYPE
		};
		Type TheType;

		enum State
		{
			S_NONE,
			S_OVER,
			S_MAX_TYPE
		};
		State TheState;

		State Update();

		PointerRef<LogicModule> NextModule;
		int FrameCount;
		int UpdateFrameCount;

		// FunObject
	public:
		FunObject TheFunObject;
	};

	typedef PointerRef<LogicModule> LogicModulePtr;

}

#endif