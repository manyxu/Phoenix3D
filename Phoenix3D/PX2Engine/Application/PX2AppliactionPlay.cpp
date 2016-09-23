// PX2AppliactionPlay.cpp

#include "PX2Application.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2BPManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
void Application::Play(PlayType type)
{
	if (!Project::GetSingletonPtr()) return;
	if (mPlayType == type) return;

	PX2_GR.SetPlayType((GraphicsRoot::PlayType)type);
	mPlayType = type;

	if (PT_PLAY == mPlayType)
	{
		std::string projName = PX2_PROJ.GetName();

		std::string callFilenameLua = "Data/" + projName + "/scripts/lua/start.lua";
		PX2_SC_LUA->CallFileFunction(callFilenameLua.c_str(), "start");

		BPPackage *bpPackage = PX2_PROJ.GetBPPackage();
		PX2_BPM.Call(bpPackage, true);

		Event *entPlay = PX2_CREATEEVENTEX(ProjectES, Play);
		PX2_EW.BroadcastingLocalEvent(entPlay);
	}
	else if (PT_NONE == mPlayType)
	{
		Event *entPlay = PX2_CREATEEVENTEX(ProjectES, Stop);
		PX2_EW.BroadcastingLocalEvent(entPlay);
	}
}
//----------------------------------------------------------------------------