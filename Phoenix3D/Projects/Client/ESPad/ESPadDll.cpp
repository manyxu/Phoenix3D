// ESPadDll.cpp

#include "PX2PluginManager.hpp"
#include "ESPadPlugin.hpp"

namespace PX2
{

	ESPadPlugin *plugin = 0;

	extern "C" void ESPAD_DLL_ITEM DLLLoadPlugin(void)
	{
		plugin = new ESPadPlugin();
		PluginManager::GetSingleton().InstallPlugin(plugin);
	}

	extern "C" void ESPAD_DLL_ITEM DLLUnloadPlugin(void)
	{
		PluginManager::GetSingleton().UninstallPlugin(plugin);
	}

}