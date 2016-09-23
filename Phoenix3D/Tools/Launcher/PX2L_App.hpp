// PX2L_App.hpp

#ifndef PX2L_APP_HPP
#define PX2L_APP_HPP

#include "PX2L_Pre.hpp"
#include "PX2EventHandler.hpp"

namespace Launcher
{

	class L_Frame;
	class PX2::Event;

	class L_App : public wxApp, public PX2::EventHandler
	{
	public:
		L_App();
		virtual ~L_App();

		virtual bool OnInit();
		virtual int OnExit();

	protected:
		L_Frame *CreateMainFrame(const std::string &name);
	};

	DECLARE_APP(L_App)
}

#endif