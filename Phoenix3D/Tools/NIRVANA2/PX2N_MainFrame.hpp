// PX2N_MainFrame.hpp

#ifndef PX2N_MAINFRAME_HPP
#define PX2N_MAINFRAME_HPP

#include "PX2N_Pre.hpp"
#include "PX2N_GeneralWindow.hpp"

namespace NA
{
	
	class N_MainFrame : public wxFrame, public PX2::EventHandler, public PX2::Singleton<N_MainFrame>
	{
	public:
		N_MainFrame(const std::string &title, int xPos, int yPos,
			int width, int height);
		virtual ~N_MainFrame();

		bool Initlize();

		void OnTimer(wxTimerEvent& e);
		void OnSize(wxSizeEvent& e);
		void OnEnterWindow(wxMouseEvent& e);
		void OnLeaveWindow(wxMouseEvent& e);
		void OnLeftDown(wxMouseEvent& e);
		void OnLeftUp(wxMouseEvent& e);
		void OnMotion(wxMouseEvent& e);

	protected:
		DECLARE_EVENT_TABLE()

		wxTimer mTimer;
		bool mIsInitlized;
		bool mIsSized;

		wxSize mSize;
	};

}

#endif