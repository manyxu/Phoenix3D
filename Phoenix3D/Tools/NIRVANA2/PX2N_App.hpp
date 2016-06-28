// PX2N_App.hpp

#ifndef PX2N_APP_HPP
#define PX2N_APP_HPP

#include "PX2N_Pre.hpp"

namespace NA
{

	class N_Frame;
	class N_PopUpMenuWindow;
	class N_GeneralFrame;
	class PX2::Event;

	class N_App : public wxApp, public PX2::EventHandler
	{
	public:
		N_App();
		virtual ~N_App();

		virtual bool OnInit();
		virtual int OnExit();

		virtual void DoExecute(PX2::Event *event);

	protected:
		N_Frame *CreateMainFrame();
		void SimuApp(int type);
	};

	DECLARE_APP(N_App)
}

#endif