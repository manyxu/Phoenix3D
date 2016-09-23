// PX2EU_ProjectEventController.hpp

#ifndef PX2EU_PROJECTEVENTCONTROLLER_HPP
#define PX2EU_PROJECTEVENTCONTROLLER_HPP

#include "PX2Event.hpp"
#include "PX2Controller.hpp"
#include "PX2EditorPre.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_ProjectEventController : public Controller
	{
	public:
		EU_ProjectEventController();
		virtual ~EU_ProjectEventController();
		virtual void OnEvent(Event *event);
	};

}

#endif