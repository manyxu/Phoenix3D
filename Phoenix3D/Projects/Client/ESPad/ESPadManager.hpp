// ESPadManager.hpp

#ifndef ESPADMANAGER_HPP
#define ESPADMANAGER_HPP

#include "ESPadPre.hpp"
#include "PX2EventHandler.hpp"
#include "PX2Singleton.hpp"
#include "PX2UIFrame.hpp"

namespace PX2
{

	class ESPAD_DLL_ITEM ESPadManager : public EventHandler
	{
	public:
		PX2_SINGLETION(ESPadManager);

	public:
		void Initlize();
		void Terminate();
		
		void Update();

		virtual void DoExecute(Event *event);
	};

}

#endif