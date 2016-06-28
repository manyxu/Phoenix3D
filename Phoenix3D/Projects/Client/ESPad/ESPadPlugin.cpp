// ESPadPlugin.cpp

#include "ESPadPlugin.hpp"
#include "ESPadManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ESPadPlugin::ESPadPlugin()
{
}
//----------------------------------------------------------------------------
ESPadPlugin::~ESPadPlugin()
{
}
//----------------------------------------------------------------------------
std::string ESPadPlugin::GetName()
{
	return "ESPad";
}
//----------------------------------------------------------------------------
void ESPadPlugin::OnInstall()
{
	ESPadManager::GetSingleton().Initlize();
}
//----------------------------------------------------------------------------
void ESPadPlugin::OnUninstall()
{
	ESPadManager::GetSingleton().Terminate();
}
//----------------------------------------------------------------------------
void ESPadPlugin::OnUpdate()
{
	ESPadManager::GetSingleton().Update();
}
//----------------------------------------------------------------------------