// PX2AppliactionPlay.cpp

#include "PX2Application.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2ProjectEvent.hpp"
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
		std::string callFilenameLua = "Data/" + mBoostProjectName + "/scripts/lua/start.lua";
		PX2_SC_LUA->CallFile(callFilenameLua.c_str());

		std::string callFilenameAS = "Data/" + mBoostProjectName + "/scripts/as/start.as";
		PX2_SC_AS->CallFileFunction(callFilenameAS.c_str(), "void start()");
	}

	Event *entUI = PX2_CREATEEVENTEX(ProjectES, Play);
	PX2_EW.BroadcastingLocalEvent(entUI);
}
//----------------------------------------------------------------------------