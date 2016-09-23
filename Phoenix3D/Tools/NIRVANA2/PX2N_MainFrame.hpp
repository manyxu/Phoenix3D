// PX2N_MainFrame.hpp

#ifndef PX2N_MAINFRAME_HPP
#define PX2N_MAINFRAME_HPP

#include "PX2N_Pre.hpp"
#include "PX2N_GeneralWindow.hpp"
#include "PX2N_RenderView.hpp"

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

		// emnu
	public:
		wxMenu *AddMainMenuItem(const std::string &title);
		wxMenuItem *AddMenuItem(wxMenu *menu, 
			const std::string &title, 
			const std::string &script, 
			const std::string &tag = "");
		void AddSeparater(wxMenu *menu);
		void EnableMenusTag(const std::string &tag, bool enable);

	protected:
		void _CreateMenu();
		void OnCommondItem(wxCommandEvent &e);

		wxMenuBar *mMainMenuBar;
		std::map<int, std::string> mIDScripts;
		std::map<std::string, std::vector<wxMenuItem*> > mTagMenuItems;

		// event
	public:
		virtual void DoExecute(PX2::Event *event);

		// renderview
	public:
		RenderView *GetMainRenderView();

	protected:
		RenderView *mMainRenderView;

	protected:
		DECLARE_EVENT_TABLE()

		wxTimer mTimer;
		bool mIsInitlized;
		bool mIsSized;

		wxSize mSize;
	};

}

#endif