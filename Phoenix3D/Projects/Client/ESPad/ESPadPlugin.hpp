// ESPadPlugin.hpp

#ifndef ESPADPLUGIN_HPP
#define ESPADPLUGIN_HPP

#include "PX2Plugin.hpp"
#include "ESPadPre.hpp"

namespace PX2
{

	class ESPAD_DLL_ITEM ESPadPlugin : public Plugin
	{
	public:
		ESPadPlugin();
		virtual ~ESPadPlugin();

		virtual std::string GetName();

		virtual void OnInstall() ;
		virtual void OnUninstall();
		virtual void OnUpdate();
	};

}

#endif