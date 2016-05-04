// PX2EngineLoop_Play.cpp

#include "PX2EngineLoop.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2Project.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2SimulationEventType.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
void EngineLoop::Play(EngineLoop::PlayType type)
{
	if (!Project::GetSingletonPtr()) return;
	if (mPlayType == type) return;

	PX2_GR.SetPlayType((GraphicsRoot::PlayType)type);
	mPlayType = type;

	if (PT_PLAY == mPlayType)
	{
		std::string callFilenameLua = "Data/" + mProjectName + "/scripts/lua/start.lua";
		PX2_SC_LUA->CallFile(callFilenameLua.c_str());

		std::string callFilenameAS = "Data/" + mProjectName + "/scripts/as/start.as";
		PX2_SC_AS->CallFileFunction(callFilenameAS.c_str(), "void start()");

		BPPackage *bpPackage = PX2_PROJ.GetBPPackage();
		if (bpPackage)
		{
			PX2_BPM.Call(bpPackage, false);
		}
	}

	Event *ent = SimuES::CreateEventX(SimuES::Play);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------