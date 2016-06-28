// PX2LP_Plugin.cpp

#include "PX2LP_Plugin.hpp"
#include "PX2LP_Manager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
LP_Plugin::LP_Plugin()
{
}
//----------------------------------------------------------------------------
LP_Plugin::~LP_Plugin()
{
}
//----------------------------------------------------------------------------
std::string LP_Plugin::GetName()
{
	return "Launcher";
}
//----------------------------------------------------------------------------
void LP_Plugin::OnInstall()
{
	LP_Manager::GetSingleton().Initlize();
}
//----------------------------------------------------------------------------
void LP_Plugin::OnUninstall()
{
	LP_Manager::GetSingleton().Terminate();
}
//----------------------------------------------------------------------------