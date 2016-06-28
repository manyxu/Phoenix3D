// PX2LogicModuleCatalogue.hpp

#ifndef PX2LOGICMODULECATALOGUE_HPP
#define PX2LOGICMODULECATALOGUE_HPP

#include "PX2SimulationPre.hpp"
#include "PX2Object.hpp"
#include "PX2LogicModule.hpp"

namespace PX2
{

	class PX2_GAME_ITEM LogicModuleCatalogue : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;

	public:
		LogicModuleCatalogue();
		~LogicModuleCatalogue();

		std::vector<LogicModulePtr> Modules;
	};

	typedef PointerRef<LogicModuleCatalogue> LogicModuleCataloguePtr;

}

#endif