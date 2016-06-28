// PX2App.hpp

#ifndef PX2APP_HPP
#define PX2APP_HPP

#include "PX2AppBase.hpp"

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#include <ShellAPI.h>
#endif

namespace PX2
{

#define PX2_DECLARE_APPLICATION(classname) \
	\
	static bool RegisterInitTerm (); \
	static bool AppInitialize (); \
	static bool AppTerminate ();

#define PX2_REGISTER_APPLICATION(classname) \
	static bool gsAppInitializeRegistered_##classname = \
	classname::RegisterInitTerm ();

#define PX2_IMPLEMENT_APPLICATION(classname) \
	\
	bool classname::RegisterInitTerm () \
	{ \
		AppBase::msAppInitlizeFun = &classname::AppInitialize; \
		AppBase::msAppTernamateFun = &classname::AppTerminate; \
		AppBase::msEntry = &App::Entry; \
		return true; \
	} \
	\
	bool classname::AppInitialize () \
	{ \
		msApplication = new classname(); \
		return true; \
	} \
	\
	bool classname::AppTerminate () \
	{ \
		delete (msApplication); \
		msApplication = 0; \
		return true; \
	}
	//----------------------------------------------------------------------------

	class App : public AppBase
	{
	public:
		App ();
		virtual ~App ();

		static int Entry (int numArguments, char** arguments);
		virtual int Main (int numArguments, char** arguments);
		virtual bool OnIdle ();

		virtual bool Initlize();
		virtual bool Terminate();

		virtual void SetTitle(const std::string &title);

#if defined(_WIN32) || defined(WIN32)
		// ´°¿Ú
		HWND mhWnd;
#endif
	};

}

#endif