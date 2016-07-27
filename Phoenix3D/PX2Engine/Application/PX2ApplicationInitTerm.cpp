// PX2ApplicationInitTerm.cpp

#include "PX2Application.hpp"
#include "PX2Assert.hpp"
#include "PX2RendererInput.hpp"
#include "PX2LocalDateTime.hpp"
#include "PX2PluginManager.hpp"
#include "PX2ToLua.hpp"
#include "PX2ToAngelScript.hpp"
#include "PX2NetInitTerm.hpp"
#include "PX2LuaContext.hpp"
#include "PX2LuaPlusContext.hpp"
#include "PX2ASContext.hpp"

using namespace PX2;

//----------------------------------------------------------------------------
ProjectEventController::ProjectEventController()
{
	ComeInEventWorld();
}
//----------------------------------------------------------------------------
ProjectEventController::~ProjectEventController()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void ProjectEventController::OnEvent(Event *event)
{
	Canvas *canvas = DynamicCast<Canvas>(GetControlledable());
	if (canvas)
	{
		const std::string &name = canvas->GetName();

		if ("ProjectSceneCanvas" == name)
		{
			if (ProjectES::IsEqual(event, ProjectES::NewScene))
			{
				Scene *scene = event->GetData<Scene*>();

				canvas->RemoveAllCameras();
				std::vector<CameraPtr> &cams = scene->GetCameras();
				for (int i = 0; i < (int)cams.size(); i++)
				{
					canvas->AddCamera(cams[i]);
				}

				canvas->AttachChild(scene);
			}
			else if (ProjectES::IsEqual(event, ProjectES::CloseScene))
			{
				Scene *scene = event->GetData<Scene*>();

				canvas->DetachChild(scene);
				canvas->SetRenderNode(0);
				canvas->RemoveAllCameras();
			}
		}
		else if ("ProjectUICanvas" == name)
		{
			if (ProjectES::IsEqual(event, ProjectES::NewUI))
			{
				UIFrame *uiFrame = event->GetData<UIFrame*>();

				canvas->AttachChild(uiFrame);
			}
			else if (ProjectES::IsEqual(event, ProjectES::CloseUI))
			{
				UIFrame *uiFrame = event->GetData<UIFrame*>();

				canvas->DetachChild(uiFrame);
				canvas->SetRenderNode(0);
			}
		}
	}
}
//----------------------------------------------------------------------------
static void *GetApplication()
{
	return Application::GetSingletonPtr();
}
static void *GetLanguageManager()
{
	return LanguageManager::GetSingletonPtr();
}
static void *GetLogger()
{
	return Logger::GetSingletonPtr();
}
static void *GetResourceManager()
{
	return ResourceManager::GetSingletonPtr();
}
static void *GetScriptManager()
{
	return ScriptManager::GetSingletonPtr();
}
static void *GetLuaContext()
{
	return PX2_SC_LUA;
}
static void *GetASContext()
{
	return PX2_SC_AS;
}
static void *GetSelectionManager()
{
	return SelectionManager::GetSingletonPtr();
}
static void *GetSelectM_D()
{
	return PX2_SELECTM_D;
}
static void *GetSelectM_E()
{
	return PX2_SELECTM_E;
}
static void *GetURDoManager()
{
	return URDoManager::GetSingletonPtr();
}
static void *GetCreater()
{
	return Creater::GetSingletonPtr();
}
static void *sGetProject()
{
	return Project::GetSingletonPtr();
}
//----------------------------------------------------------------------------
bool Application::Initlize()
{
	time_t ti; time(&ti); srand((unsigned int)ti);

#ifdef PX2_USE_MEMORY
	Memory::Initialize();
#endif

	StringHelp::Initlize();
	FString::Initlize();

	Logger *logger = new0 Logger();

#if defined(_WIN32) || defined(WIN32)
	logger->AddFileHandler("PX2Application_Log.txt",
	 LT_INFO | LT_ERROR | LT_USER);
#endif
	logger->AddOutputWindowHandler(LT_INFO | LT_ERROR | LT_USER);
	logger->StartLogger();

	LocalDateTime time;
	int year1 = time.Year();
	int month1 = time.Month();
	int week1 = time.Week();
	int day1 = time.Day();
	int dayOfWeek1 = time.DayOfWeek();
	int dayOfYear1 = time.DayOfYear();
	int hour1 = time.Hour();
	int minute1 = time.Minute();
	int second1 = time.Second();
	int millisecond1 = time.Millisecond();
	int microsecond1 = time.Microsecond();
	PX2_LOG_INFO("Year:%d; Month:%d; Week:%d; Day:%d; DayOfWeek:%d; DayOfYear:%d; Hour:%d; Minute:%d; Second:%d; Millisecond:%d; Microsecond:%d",
		year1, month1, week1, day1, dayOfWeek1, dayOfYear1, hour1, minute1, 
		second1, millisecond1, microsecond1);

	InitializeNetwork();

	mDynLibMan = new0 DynLibManager();
	PX2_UNUSED(mDynLibMan);
	mPluginMan = new0 PluginManager();
	PX2_UNUSED(mPluginMan);

	mTimerMan = new0 TimerManager();

	mIMEDisp = new0 IMEDispatcher();

	mInputMan = new0 InputManager();

	mLanguageMan = new0 LanguageManager();
	mResMan = new0 ResourceManager();

	mEventWorld = new0 EventWorld();

	mScriptMan = new0 ScriptManager();

	mRoot = new0 GraphicsRoot();
	mRoot->Initlize();

	mFontMan = new0 FontManager();
	mFontMan->Initlize();

	mADMan = new0 AddDeleteManager();
	PX2_UNUSED(mADMan);

	mSelectionMan = new0 SelectionManager();
	PX2_UNUSED(mSelectionMan);

	mURDoMan = new0 URDoManager();
	PX2_UNUSED(mURDoMan);

	mFunObjectManager = new0 FunObjectManager();
	mFunObjectManager->Initlize();

	mAccoutManager = new0 AccoutManager();

	mUIAuiManager = new0 UIAuiManager();

	mUISkinManager = new0 UISkinManager();

	mLogicManager = new0 LogicManager();

	mCreater = new0 Creater();

	LuaPlusContext *luaContext = (LuaPlusContext*)PX2_SC_LUA;
	ASContext *asContext = (ASContext*)PX2_SC_AS;

	// Lua
	tolua_PX2_open((lua_State*)luaContext->GetLuaState());
	LuaPlus::LuaState *luaPlusState = luaContext->GetLuaPlusState();

	PX2_SC_LUA->SetUserTypePointer("PX2_APP", "Application", Application::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_LM", "LanguageManager", LanguageManager::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_LOGGER", "Logger", Logger::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_RM", "ResourceManager", PX2_SC_LUA);
	PX2_SC_LUA->SetUserTypePointer("PX2_SM", "ScriptManager", ScriptManager::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_SC_LUA", "LuaContext", PX2_SC_LUA);
	PX2_SC_LUA->SetUserTypePointer("PX2_SC_AS", "ASContext", PX2_SC_LUA);
	PX2_SC_LUA->SetUserTypePointer("PX2_CREATER", "Creater", Creater::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_PROJ", "Project", Project::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_SELECTM", "SelectionManager", SelectionManager::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_SELECTM_D", "Selection", SelectionManager::GetSingleton().GetSelecton("Default"));
	PX2_SC_LUA->SetUserTypePointer("PX2_SELECTM_E", "Selection", SelectionManager::GetSingleton().GetSelecton("Editor"));
	// end Lua

	// AS
	PX2ToAngelScript(asContext->GetASScriptEngine());
	PX2_SC_AS->RegistOperators();

	PX2_SC_AS->SetUserFunction("PX2_APP", "Application", GetApplication);
	PX2_SC_AS->SetUserFunction("PX2_LM", "LanguageManager", GetLanguageManager);
	PX2_SC_AS->SetUserFunction("PX2_LOGGER", "Logger", GetLogger);
	PX2_SC_AS->SetUserFunction("PX2_RM", "ResourceManager", GetResourceManager);
	PX2_SC_AS->SetUserFunction("PX2_SM", "ScriptManager", GetScriptManager);
	PX2_SC_AS->SetUserFunction("+PX2_SC_LUA", "LuaContext", GetLuaContext);
	PX2_SC_AS->SetUserFunction("+PX2_SC_AS", "ASContext", GetASContext);
	PX2_SC_AS->SetUserFunction("PX2_CREATER", "Creater", GetCreater);
	PX2_SC_AS->SetUserFunction("PX2_PROJ", "Project", sGetProject);

	// end AS

	LoadBoost("Data/boost.xml");

	UICanvas *mainCanvas = new0 UICanvas();
	mainCanvas->SetMain(true);
	mainCanvas->EnableAnchorLayout(true);
	mainCanvas->SetAnchorHor(0.0f, 1.0f);
	mainCanvas->SetAnchorVer(0.0f, 1.0f);
	mainCanvas->ComeInEventWorld();
	mainCanvas->SetName("MainCanvas");

	RenderWindow *rw = PX2_GR.GetMainWindow();
	rw->SetMainCanvas(mainCanvas);

	PX2_SC_LUA->CallFile("Data/engine/scripts/lua/engine_start.lua");
	PX2_SC_AS->CallFileFunction("Data/engine/scripts/as/engine_start.as",
		"void engine_start()");

	return true;
}
//----------------------------------------------------------------------------
void Application::InitlizeDefaultEngineCanvas()
{
	RenderWindow *rw = PX2_GR.GetMainWindow();
	UICanvas *canvasMain = DynamicCast<UICanvas>(rw->GetMainCanvas());

	Canvas *sceneCanvas = new0 Canvas();
	mEngineSceneCanvas = sceneCanvas;
	canvasMain->AttachChild(sceneCanvas);
	sceneCanvas->AttachController(new0 ProjectEventController());
	sceneCanvas->SetName("ProjectSceneCanvas");
	sceneCanvas->EnableAnchorLayout(true);
	sceneCanvas->SetAnchorHor(0.0f, 1.0f);
	sceneCanvas->SetAnchorVer(0.0f, 1.0f);

	UICanvas *uiCanvas = new0 UICanvas();
	mEngineUICanvas = uiCanvas;
	canvasMain->AttachChild(uiCanvas);
	uiCanvas->AttachController(new0 ProjectEventController());
	uiCanvas->SetName("ProjectUICanvas");
	uiCanvas->EnableAnchorLayout(true);
	uiCanvas->SetAnchorHor(0.0f, 1.0f);
	uiCanvas->SetAnchorVer(0.0f, 1.0f);

	uiCanvas->GetCameraNode()->GetCamera()->SetClearFlag(false, true, true);
}
//----------------------------------------------------------------------------
bool Application::InitlizeRenderer()
{
	Renderer *defRenderer = CreateRenderer("DefaultRenderer", mPt_Data,
		(int)mPt_Size.Width, (int)mPt_Size.Height, 0);
	Renderer::SetDefaultRenderer(defRenderer);
	PX2_GR.GetMainWindow()->SetRenderer(defRenderer);

	mScreenSize = mPt_Size;

	return true;
}
//----------------------------------------------------------------------------
Renderer *Application::CreateRenderer(const std::string &name, void *winHandle,
	int width, int height, int numMultisamples)
{
	RendererInput *rendererInput = 0;
	Renderer *renderer = Renderer::CreateRenderer(winHandle, width,
		height, numMultisamples, rendererInput);

	mRenderersMap[name] = renderer;
	mRendererInputMap[name] = rendererInput;

	return renderer;
}
//----------------------------------------------------------------------------
void Application::WillEnterForeground(bool isFirstTime)
{
	if (!mIsInBackground) return;

	if (isFirstTime)
	{
		PX2_FM.Initlize();
	}
	else
	{
		Renderer::GetDefaultRenderer()->OnRestoreDevice();

		PX2_FM.SetNeedUpdate();
	}

	// PX2_SS.EnableMusic(mBeforeInBackgroundMusicEnable);
	// PX2_SS.EnableSounds(mBeforeInBackgroundSoundEnable);
	// PX2_SOUNDM.PreLoad();

	mIsInBackground = false;
}
//----------------------------------------------------------------------------
void Application::DidEnterBackground()
{
	PX2_FM.Terminate();
	PX2_AM.DeleteAllUsers();
	PX2_ADM.Clear();
	PX2_RM.Clear();
	PX2_SELECTM.Clear();
	PX2_URDOM.Clear();
	PX2_TimerM.ClearTimers();

	if (SoundSystem::GetSingletonPtr())
	{
		mBeforeInBackgroundMusicEnable = PX2_SS.IsMusicEnable();
		mBeforeInBackgroundSoundEnable = PX2_SS.IsSoundEnable();
		PX2_SS.EnableMusic(false);
		PX2_SS.EnableSounds(false);
	}

	Renderer::GetDefaultRenderer()->OnLostDevice();

	mIsInBackground = true;
}
//----------------------------------------------------------------------------
bool Application::Terminate()
{
	Play(Application::PT_NONE);
	CloseProject();

	PX2_PLUGINMAN.UnloadPlugins();

	PX2_SC_LUA->CallFile("Data/engine/scripts/lua/engine_end.lua");
	PX2_SC_AS->CallFileFunction("Data/engine/scripts/as/engine_end.as",
		"void engine_end()");

	mEngineSceneCanvas = 0;
	mEngineUICanvas = 0;

	PX2_EW.Shutdown(true);

	mScriptMan->TernimateAll();
	if (mScriptMan)
	{
		delete0(mScriptMan);
		ScriptManager::Set(0);
	}

	if (mCreater)
	{
		delete0(mCreater);
		Creater::Set(0);
	}

	if (mLogicManager)
	{
		delete0(mLogicManager);
		LogicManager::Set(0);
	}

	if (mUISkinManager)
	{
		delete0(mUISkinManager);
		UISkinManager::Set(0);
	}

	if (mUIAuiManager)
	{
		delete0(mUIAuiManager);
		UIAuiManager::Set(0);
	}

	if (mSelectionMan)
	{
		mSelectionMan->Clear();
		delete0(mSelectionMan);
		SelectionManager::Set(0);
	}

	if (mURDoMan)
	{
		delete0(mURDoMan);
		URDoManager::Set(0);
	}

	if (mADMan)
	{
		mADMan->Clear();
		delete0(mADMan);
		AddDeleteManager::Set(0);
	}

	if (mFunObjectManager)
	{
		mFunObjectManager->Terminate();
		delete0(mFunObjectManager);
		FunObjectManager::Set(0);
	}

	if (mAccoutManager)
	{
		delete0(mAccoutManager);
		AccoutManager::Set(0);
	}

	if (mFontMan)
	{
		delete0(mFontMan);
		FontManager::Set(0);
	}

	bool isInEditor = mRoot->IsInEditor();
	PX2_UNUSED(isInEditor);
	if (mRoot)
	{
		mRoot->Terminate();
		delete0(mRoot);
		GraphicsRoot::Set(0);
	}

	if (mInputMan)
	{
		delete0(mInputMan);
		mInputMan = 0;
	}

	if (mResMan)
	{
		mResMan->Clear();
		delete0(mResMan);
		ResourceManager::Set(0);
	}

	if (mLanguageMan)
	{
		delete0(mLanguageMan);
		LanguageManager::Set(0);
	}

	if (mIMEDisp)
	{ // 需要在资源管理器之后释放
		delete0(mIMEDisp);
		IMEDispatcher::Set(0);
	}

	if (mEventWorld)
	{
		delete0(mEventWorld);
		EventWorld::Set(0);
	}

	if (mTimerMan)
	{
		delete0(mTimerMan);
		TimerManager::Set(0);
	}

	std::map<std::string, Renderer*>::iterator it = mRenderersMap.begin();
	for (; it != mRenderersMap.end(); it++)
	{
		Renderer *renderer = it->second;
		delete0(renderer);
	}
	Renderer::SetDefaultRenderer(0);
	mRenderersMap.clear();

	std::map<std::string, RendererInput*>::iterator itInput = mRendererInputMap.begin();
	for (; itInput != mRendererInputMap.end(); itInput++)
	{
		RendererInput *rendererInput = itInput->second;
		delete0(rendererInput);
	}
	mRendererInputMap.clear();

	PX2_PLUGINMAN.UnloadPlugins();
	if (mPluginMan)
	{
		delete0(mPluginMan);
		PluginManager::Set(0);
	}

	if (mDynLibMan)
	{
		delete0(mDynLibMan);
		DynLibManager::Set(0);
	}

	TerminateNetwork();

	PX2_LOG_INFO("Engine Terminate done.");
	Logger *logger = Logger::GetSingletonPtr();
	if (logger)
	{
		delete0(logger);
		Logger::Set(0);
	}

	FString::Ternimate();

#ifdef PX2_USE_MEMORY

	if (isInEditor)
	{
		Memory::Terminate("Editor_MemoryReport.txt", false);
	}
	else
	{
		Memory::Terminate("PX2Application_MemoryReport.txt", false);
	}

#endif

	return true;
}
//----------------------------------------------------------------------------
bool Application::LoadBoost(const std::string &filename)
{
	XMLData data;

	int bufferSize = 0;
	char *buffer = 0;
	ResourceManager::GetSingleton().LoadBuffer(filename, bufferSize, buffer);
	if (!buffer || bufferSize == 0) return false;

	if (data.LoadBuffer(buffer, bufferSize))
	{
		mBoostSize.Width = data.GetNodeByPath("config.var").AttributeToFloat("width");
		mBoostSize.Height = data.GetNodeByPath("config.var").AttributeToFloat("height");
		mBoostProjectName = data.GetNodeByPath("play.var").AttributeToString("projectname");
		mPlayLogicMode = _StrToPlayLogicMode(data.GetNodeByPath("play.var").AttributeToString("playlogicmode"));

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
std::string Application::GetPlayLogicModeStr() const
{
	if (PLM_SIMPLE == mPlayLogicMode)
		return "simple";

	return "logic";
}
//----------------------------------------------------------------------------
Application::PlayLogicMode Application::_StrToPlayLogicMode(
	const std::string &str)
{
	if ("simple" == str)
		Application::PLM_SIMPLE;

	return PLM_LOGIC;
}
//----------------------------------------------------------------------------
void Application::SetBoostSize(const Sizef &size)
{
	mBoostSize = size;
}
//----------------------------------------------------------------------------
void Application::SetPlayLogicMode(PlayLogicMode mode)
{
	mPlayLogicMode = mode;
}
//----------------------------------------------------------------------------
bool Application::WriteBoost()
{
	XMLData data;

	data.Create();

	XMLNode boostNode = data.NewChild("boost");
	boostNode.SetAttributeString("name", "boost");

	XMLNode configNode = boostNode.NewChild("config");

	XMLNode varNode_config = configNode.NewChild("var");
	varNode_config.SetAttributeInt("width", (int)mBoostSize.Width);
	varNode_config.SetAttributeInt("height", (int)mBoostSize.Height);

	XMLNode playNode = boostNode.NewChild("play");
	XMLNode varNode_play = playNode.NewChild("var");
	varNode_play.SetAttributeString("projectname", mBoostProjectName);
	varNode_play.SetAttributeString("playlogicmode", GetPlayLogicModeStr());

	return data.SaveFile("Data/boost.xml");
}
//----------------------------------------------------------------------------
void Application::SetScreenSize(const Sizef &screenSize)
{
	mScreenSize = screenSize;
	PX2_GR.GetMainWindow()->SetScreenSize(mScreenSize);
	PX2_INPUTMAN.GetDefaultListener()->SetViewSize(mScreenSize);

	Renderer *defaultRenderer = Renderer::GetDefaultRenderer();
	if (defaultRenderer) defaultRenderer->ResizeWindow((int)mScreenSize.Width,
		(int)mScreenSize.Height);
}
//----------------------------------------------------------------------------