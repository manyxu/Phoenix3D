// PX2LuaScriptController.cpp

#include "PX2LuaScriptController.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2ASContext.hpp"
#include "PX2Controlledable.hpp"
#include "PX2LuaPlusContext.hpp"
#include "PX2Log.hpp"
#include "LuaPlus.h"

using namespace PX2;
using namespace LuaPlus;

PX2_IMPLEMENT_RTTI(PX2, ScriptController, LuaScriptController);
PX2_IMPLEMENT_STREAM(LuaScriptController);
PX2_IMPLEMENT_DEFAULT_NAMES(ScriptController, LuaScriptController);
PX2_IMPLEMENT_FACTORY(LuaScriptController);

//---------------------------------------------------------------------------
const char* LuaSCRIPCONTROLLER_NAME = "LuaScriptController";
//---------------------------------------------------------------------------
bool LuaScriptController::sIsClassRegisted = false;
//---------------------------------------------------------------------------
LuaScriptController::LuaScriptController() :
mLO_OnAttached(0),
mLO_OnDetach(0),
mLO_OnInitUpdate(0),
mLO_OnUpdate(0),
mLO_OnFixUpdate(0),
mLO_OnResetPlay(0),
mLO_OnReset(0),
mLO_OnPlay(0),
mLO_OnStop(0),
mLO_OnPlayUpdate(0),
mLO_Self(0),
mLuaState(0)
{
	if (!sIsClassRegisted)
	{
		RegisterScriptClass();
	}

	mLuaState = ((LuaPlusContext*)PX2_SC_LUA)->GetLuaPlusState();

	mLO_OnAttached = new0 LuaObject();
	mLO_OnDetach = new0 LuaObject();
	mLO_OnInitUpdate = new0 LuaObject();
	mLO_OnUpdate = new0 LuaObject();
	mLO_OnFixUpdate = new0 LuaObject();

	mLO_OnResetPlay = new0 LuaObject();
	mLO_OnReset = new0 LuaObject();
	mLO_OnPlay = new0 LuaObject();
	mLO_OnStop = new0 LuaObject();
	mLO_OnPlayUpdate = new0 LuaObject();

	mLO_OnAttached->AssignNil(mLuaState);
	mLO_OnDetach->AssignNil(mLuaState);
	mLO_OnUpdate->AssignNil(mLuaState);
	mLO_OnFixUpdate->AssignNil(mLuaState);

	mLO_OnResetPlay->AssignNil(mLuaState);
	mLO_OnReset->AssignNil(mLuaState);
	mLO_OnPlay->AssignNil(mLuaState);
	mLO_OnStop->AssignNil(mLuaState);
	mLO_OnPlayUpdate->AssignNil(mLuaState);
}
//---------------------------------------------------------------------------
LuaScriptController::LuaScriptController(const std::string &filename,
	const std::string &tableName) :
	mLO_OnAttached(0),
	mLO_OnDetach(0),
	mLO_OnInitUpdate(0),
	mLO_OnUpdate(0),
	mLO_OnFixUpdate(0),
	mLO_OnResetPlay(0),
	mLO_OnReset(0),
	mLO_OnPlay(0),
	mLO_OnStop(0),
	mLO_OnPlayUpdate(0),
	mLO_Self(0),
	mLuaState(0)
{
	if (!sIsClassRegisted)
	{
		RegisterScriptClass();
	}

	LuaPlusContext *luaContext = (LuaPlusContext*)PX2_SC_LUA;
	mLuaState = luaContext->GetLuaPlusState();

	mLO_OnAttached = new0 LuaObject();
	mLO_OnDetach = new0 LuaObject();
	mLO_OnInitUpdate = new0 LuaObject();
	mLO_OnUpdate = new0 LuaObject();
	mLO_OnFixUpdate = new0 LuaObject();

	mLO_OnResetPlay = new0 LuaObject();
	mLO_OnReset = new0 LuaObject();
	mLO_OnPlay = new0 LuaObject();
	mLO_OnStop = new0 LuaObject();
	mLO_OnPlayUpdate = new0 LuaObject();

	mLO_Self = new0 LuaObject();

	mLO_OnAttached->AssignNil(mLuaState);
	mLO_OnDetach->AssignNil(mLuaState);
	mLO_OnUpdate->AssignNil(mLuaState);
	mLO_OnFixUpdate->AssignNil(mLuaState);

	mLO_OnResetPlay->AssignNil(mLuaState);
	mLO_OnReset->AssignNil(mLuaState);
	mLO_OnPlay->AssignNil(mLuaState);
	mLO_OnStop->AssignNil(mLuaState);
	mLO_OnPlayUpdate->AssignNil(mLuaState);

	mLO_Self->AssignNil(mLuaState);

	luaContext->CallFile(filename);

	LuaPlus::LuaObject subClass = mLuaState->GetGlobal(tableName.c_str());
	LuaPlus::LuaObject constructionData;
	constructionData.AssignNil(mLuaState);

	mLO_Self->AssignNewTable(mLuaState);
	if (BuildCppDataFromScript(subClass, constructionData))
	{
		LuaPlus::LuaObject metaTableObj = luaContext->GetLuaPlusState()
			->GetGlobals().Lookup(LuaSCRIPCONTROLLER_NAME);
		assertion(!metaTableObj.IsNil(), "");

		mLO_Self->SetLightUserdata("__object", this);
		mLO_Self->SetMetatable(metaTableObj);
	}
	else
	{
		mLO_Self->AssignNil(mLuaState);
	}
}
//---------------------------------------------------------------------------
LuaScriptController::~LuaScriptController()
{
	delete0(mLO_OnAttached);
	delete0(mLO_OnDetach);
	delete0(mLO_OnInitUpdate);
	delete0(mLO_OnUpdate);
	delete0(mLO_OnFixUpdate);

	delete0(mLO_OnResetPlay);
	delete0(mLO_OnReset);
	delete0(mLO_OnPlay);
	delete0(mLO_OnStop);
	delete0(mLO_OnPlayUpdate);

	delete0(mLO_Self);
}
//---------------------------------------------------------------------------
bool LuaScriptController::BuildCppDataFromScript(
	LuaPlus::LuaObject &scriptClass,
	LuaPlus::LuaObject &constructionData)
{
	if (scriptClass.IsTable())
	{
		// OnInit()
		LuaPlus::LuaObject temp;
		temp = scriptClass.GetByName("OnAttached");
		if (temp.IsFunction())
			*mLO_OnAttached = temp;

		temp = scriptClass.GetByName("OnDetach");
		if (temp.IsFunction())
			*mLO_OnDetach = temp;

		temp = scriptClass.GetByName("OnInitUpdate");
		if (temp.IsFunction())
			*mLO_OnInitUpdate = temp;

		temp = scriptClass.GetByName("OnUpdate");
		if (temp.IsFunction())
			*mLO_OnUpdate = temp;

		temp = scriptClass.GetByName("OnFixUpdate");
		if (temp.IsFunction())
			*mLO_OnFixUpdate = temp;

		temp = scriptClass.GetByName("OnPResetPlay");
		if (temp.IsFunction())
			*mLO_OnResetPlay = temp;

		temp = scriptClass.GetByName("OnPReset");
		if (temp.IsFunction())
			*mLO_OnReset = temp;

		temp = scriptClass.GetByName("OnPPlay");
		if (temp.IsFunction())
			*mLO_OnPlay = temp;

		temp = scriptClass.GetByName("OnPStop");
		if (temp.IsFunction())
			*mLO_OnStop = temp;

		temp = scriptClass.GetByName("OnPUpdate");
		if (temp.IsFunction())
			*mLO_OnPlayUpdate = temp;
	}
	else
	{
		PX2_LOG_INFO("scriptClass is not a table in ScriptProcess::BuildCppDataFromScript()");
		return false;
	}

	if (constructionData.IsTable())
	{
		for (LuaPlus::LuaTableIterator constructionDataIt(constructionData); 
			constructionDataIt; constructionDataIt.Next())
		{
			const char* key = constructionDataIt.GetKey().GetString();
			LuaPlus::LuaObject val = constructionDataIt.GetValue();

			mLO_Self->SetObject(key, val);
		}
	}

	return true;
}
//---------------------------------------------------------------------------
void LuaScriptController::RegisterScriptClass()
{
	LuaPlusContext *luaContext = (LuaPlusContext*)PX2_SC_LUA;

	LuaPlus::LuaObject metaTableObj = luaContext->GetLuaPlusState()
		->GetGlobals().CreateTable(LuaSCRIPCONTROLLER_NAME);

	metaTableObj.SetObject("__index", metaTableObj);
	metaTableObj.SetObject("base", metaTableObj);  // base refers to the parent class; ie the metatable
	metaTableObj.SetBoolean("cpp", true);

	//metaTableObj.RegisterObjectDirect("OnAttached", (LuaScriptController*)0,
	//	&LuaScriptController::ScriptOnAttached);

	sIsClassRegisted = true;
}
//---------------------------------------------------------------------------
void LuaScriptController::ScriptOnAttached()
{
	//if (!mLO_OnAttached->IsNil())
	//{
		//LuaPlus::LuaFunction<void> func(*mLO_OnAttached);
		//func(*mLO_Self);
	//}

	if (mLO_OnAttached && !mLO_OnAttached->IsNil())
	{
		LuaCall call = *mLO_OnAttached;
		call << LuaRun();
	}
}
//---------------------------------------------------------------------------
void LuaScriptController::ScriptOnDetach()
{
	if (mLO_OnDetach && !mLO_OnDetach->IsNil())
	{
		LuaCall call = *mLO_OnDetach;
		call << LuaRun();
	}
}
//---------------------------------------------------------------------------
void LuaScriptController::ScriptOnInitUpdate()
{
	if (mLO_OnInitUpdate && !mLO_OnInitUpdate->IsNil())
	{
		LuaCall call = *mLO_OnInitUpdate;
		call << LuaRun();
	}
}
//---------------------------------------------------------------------------
void LuaScriptController::ScriptOnUpdate()
{
	if (mLO_OnUpdate && !mLO_OnUpdate->IsNil())
	{
		LuaCall call = *mLO_OnUpdate;
		call << LuaRun();
	}
}
//---------------------------------------------------------------------------
void LuaScriptController::ScriptOnFixUpdate()
{
	if (mLO_OnFixUpdate && !mLO_OnFixUpdate->IsNil())
	{
		LuaCall call = *mLO_OnFixUpdate;
		call << LuaRun();
	}
}
//---------------------------------------------------------------------------
void LuaScriptController::ScriptOnResetPlay()
{
	if (mLO_OnResetPlay && !mLO_OnResetPlay->IsNil())
	{
		LuaCall call = *mLO_OnResetPlay;
		call << LuaRun();
	}
}
//---------------------------------------------------------------------------
void LuaScriptController::ScriptOnReset()
{
	if (mLO_OnReset && !mLO_OnReset->IsNil())
	{
		LuaCall call = *mLO_OnReset;
		call << LuaRun();
	}
}
//---------------------------------------------------------------------------
void LuaScriptController::ScriptOnPlay()
{
	if (mLO_OnPlay && !mLO_OnPlay->IsNil())
	{
		LuaCall call = *mLO_OnPlay;
		call << LuaRun();
	}
}
//---------------------------------------------------------------------------
void LuaScriptController::ScriptOnStop()
{
	if (mLO_OnStop && !mLO_OnStop->IsNil())
	{
		LuaCall call = *mLO_OnStop;
		call << LuaRun();
	}
}
//---------------------------------------------------------------------------
void LuaScriptController::ScriptOnPlayUpdate()
{
	if (mLO_OnPlayUpdate && mLO_OnPlayUpdate->IsNil())
	{
		LuaCall call = *mLO_OnPlayUpdate;
		call << LuaRun();
	}
}
//---------------------------------------------------------------------------
void LuaScriptController::_Update(double applicationTime, double elapsedTime)
{
	PX2_UNUSED(applicationTime);
	PX2_UNUSED(elapsedTime);

	ScriptOnPlayUpdate();
}
//---------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
LuaScriptController::LuaScriptController(LoadConstructor value) :
ScriptController(value),
mLO_OnAttached(0),
mLO_OnDetach(0),
mLO_OnUpdate(0),
mLO_OnFixUpdate(0),
mLO_OnResetPlay(0),
mLO_OnReset(0),
mLO_OnPlay(0),
mLO_OnStop(0),
mLO_OnPlayUpdate(0),
mLO_Self(0),
mLuaState(0)
{
}
//----------------------------------------------------------------------------
void LuaScriptController::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	ScriptController::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(LuaScriptController, source);
}
//----------------------------------------------------------------------------
void LuaScriptController::Link(InStream& source)
{
	ScriptController::Link(source);
}
//----------------------------------------------------------------------------
void LuaScriptController::PostLink()
{
	ScriptController::PostLink();
}
//----------------------------------------------------------------------------
bool LuaScriptController::Register(OutStream& target) const
{
	if (ScriptController::Register(target))
	{
		target.Register(mObject);
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void LuaScriptController::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	ScriptController::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(LuaScriptController, target);
}
//----------------------------------------------------------------------------
int LuaScriptController::GetStreamingSize(Stream &stream) const
{
	int size = ScriptController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------