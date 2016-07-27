// PX2Application.hpp

#ifndef PX2APPLICATION_HPP
#define PX2APPLICATION_HPP

#include "PX2Project.hpp"
#include "PX2LanguageManager.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2DynLibManager.hpp"
#include "PX2PluginManager.hpp"
#include "PX2FunObject.hpp"
#include "PX2IMEDispatcher.hpp"
#include "PX2InputManager.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2FontManager.hpp"
#include "PX2AddDeleteManager.hpp"
#include "PX2SelectionManager.hpp"
#include "PX2RedoUndo.hpp"
#include "PX2AccoutManager.hpp"
#include "PX2Project.hpp"
#include "PX2UIAuiManager.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2SoundSystem.hpp"
#include "PX2Creater.hpp"
#include "PX2Singleton.hpp"
#include "PX2RendererInput.hpp"
#include "PX2TimerManager.hpp"
#include "PX2LogicManager.hpp"
#include "PX2UICanvas.hpp"

namespace PX2
{

	//----------------------------------------------------------------------------
	class PX2_ENGINE_ITEM ProjectEventController : public Controller
	{
	public:
		ProjectEventController();
		virtual ~ProjectEventController();
		virtual void OnEvent(Event *event);
	};

	class PX2_ENGINE_ITEM Application
	{
		PX2_SINGLETION(Application);

		// pre init
	public:
		void SetPt_Data(void *data);
		void *GetPt_Data();
		void SetPt_Size(const Sizef &size);
		const Sizef &GetPt_Size() const;

	protected:
		void *mPt_Data;
		Sizef mPt_Size;

		// init term
	public:
		bool Initlize();
		void InitlizeDefaultEngineCanvas();
		bool InitlizeRenderer();
		Renderer *CreateRenderer(const std::string &name, void *winHandle,
			int width, int height, int numMultisamples);

		void WillEnterForeground(bool isFirstTime);
		void DidEnterBackground();

		bool Terminate();

	private:
		std::map<std::string, RendererInput *> mRendererInputMap;
		std::map<std::string, Renderer*> mRenderersMap;

		DynLibManager *mDynLibMan;
		PluginManager *mPluginMan;
		TimerManager *mTimerMan;
		EventWorld *mEventWorld;
		LanguageManager *mLanguageMan;
		ScriptManager *mScriptMan;
		GraphicsRoot *mRoot;
		InputManager *mInputMan;
		IMEDispatcher *mIMEDisp;
		ResourceManager *mResMan;
		FontManager *mFontMan;
		AddDeleteManager *mADMan;
		SelectionManager *mSelectionMan;
		URDoManager *mURDoMan;
		FunObjectManager *mFunObjectManager;
		AccoutManager *mAccoutManager;
		UIAuiManager *mUIAuiManager;
		UISkinManager *mUISkinManager;
		LogicManager *mLogicManager;
		Creater *mCreater;

		bool mIsInBackground;
		bool mBeforeInBackgroundMusicEnable;
		bool mBeforeInBackgroundSoundEnable;

		// Update
	public:
		void Update();
		float GetElapsedTime();

	private:
		double mAppTime;
		double mLastAppTime;
		double mElapsedTime;

		// screen adjust
	public:
		void SetScreenSize(const Sizef &screenSize);
		const Sizef &GetScreenSize() const;
	protected:
		Sizef mScreenSize;

		// boost
	public:
		enum PlayLogicMode
		{
			PLM_SIMPLE,
			PLM_LOGIC,
			PLM_MAX_MODE
		};

		bool LoadBoost(const std::string &filename);
		const Sizef &GetBoostSize() const;
		const std::string &GetProjectName() const;
		PlayLogicMode GetPlayLogicMode() const;
		std::string GetPlayLogicModeStr() const;

		void SetBoostSize(const Sizef &size);
		void SetPlayLogicMode(PlayLogicMode mode);
		bool WriteBoost();

	protected:
		PlayLogicMode _StrToPlayLogicMode(const std::string &str);

		Sizef mBoostSize;
		std::string mBoostProjectName;
		PlayLogicMode mPlayLogicMode;

		// project
	public:
		void NewProject(const std::string &pathname,
			const std::string &projName, int so, int width, int height);
		bool LoadProject(const std::string &pathname);
		bool SaveProject();
		bool SaveProjectAs(const std::string &pathname);
		void CloseProject();
		const std::string &GetProjectFilePath() const;

		void NewScene();
		bool LoadScene(const std::string &pathname);
		bool SaveScene(const std::string &pathname);
		bool SaveSceneAs(const std::string &pathname);
		void CloseScene();

		bool LoadUI(const std::string &pathname);
		void CloseUI();

		Canvas *GetEngineSceneCanvas();
		UICanvas *GetEngineUICanvas();

	protected:
		std::string _CalSavePath(const std::string &pathname);
		bool _SaveSceneInternal(const std::string &pathname);

		std::string mProjectFilePath;
		std::string mSceneFilePath;
		std::string mUIFilePath;
		std::string mBPFilePath;

		CanvasPtr mEngineSceneCanvas;
		UICanvasPtr mEngineUICanvas;

	public_internal:
		PointerRef<Project> TheProject;

		// play
	public:
		enum PlayType
		{
			PT_NONE,
			PT_SIMULATE,
			PT_PLAY,
			PT_MAX_TYPE
		};
		void Play(PlayType type);
		PlayType GetPlayType() const;

	protected:
		PlayType mPlayType;
	};
#include "PX2Application.inl"

#define  PX2_APP Application::GetSingleton()

}

#endif
