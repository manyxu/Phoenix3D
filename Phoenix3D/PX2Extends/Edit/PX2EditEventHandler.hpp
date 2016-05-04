// PX2EditEventHandler.hpp

#ifndef PX2EDITEVENTHANDLER_HPP
#define PX2EDITEVENTHANDLER_HPP

#include "PX2SimulationPre.hpp"
#include "PX2EventHandler.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2Event.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EditEventHandler : public EventHandler
	{
	public:
		EditEventHandler();
		virtual ~EditEventHandler();

		virtual void DoExecute(Event *event);
	};
	typedef Pointer0<EditEventHandler> EditEventHandlerPtr;

}

#endif