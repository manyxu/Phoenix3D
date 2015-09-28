// PX2N_App.hpp

#ifndef PX2N_APP_HPP
#define PX2N_APP_HPP

#include "PX2N_Pre.hpp"

namespace NA
{

	class N_MainFrame;
	class N_PopUpMenuWindow;
	class N_GeneralFrame;

	class N_App : public wxApp
	{
	public:
		N_App();
		virtual ~N_App();

		virtual bool OnInit();
		virtual int OnExit();

	protected:
		N_MainFrame *mMainFrame;
		N_PopUpMenuWindow *mMainMenuWindow;
	};

	DECLARE_APP(N_App)
}

#endif