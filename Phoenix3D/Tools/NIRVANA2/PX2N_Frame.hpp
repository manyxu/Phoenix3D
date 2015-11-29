// PX2N_Frame.hpp

#ifndef PX2N_FRAME_HPP
#define PX2N_FRAME_HPP

#include "PX2N_Pre.hpp"
#include "PX2N_GeneralWindow.hpp"
#include "PX2N_RenderView.hpp"

namespace NA
{
	
	class N_Frame : public wxFrame, public PX2::EventHandler
	{
	public:
		N_Frame(wxWindow *parent, int id, const std::string &name, const std::string &title,
			int xPos, int yPos, int width, int height, long style);
		virtual ~N_Frame();

		RenderView *GerRenderView();

		void OnTimer(wxTimerEvent& e);
		void OnSize(wxSizeEvent& e);
		void OnEnterWindow(wxMouseEvent& e);
		void OnLeaveWindow(wxMouseEvent& e);
		void OnLeftDown(wxMouseEvent& e);
		void OnLeftUp(wxMouseEvent& e);
		void OnMotion(wxMouseEvent& e);

		// operators
	public:
		void DoNewProject();
		void DoOpenProject();
		void DoSaveProject();
		void DoCloseProject();
		void DoNewScene();
		void DoOpenScene();
		void DoSaveScene();
		void DoSaveSceneAs();
		void DoCloseScene();

		void DoExit();

	protected:
		bool mIsNewProject;
		bool mIsOpenProject;
		bool mIsSaveProject;		
		bool mIsCloseProject;
		bool mIsNewScene;
		bool mIsOpenScene;
		bool mIsSaveScene;
		bool mIsSaveSceneAs;
		bool mIsCloseScene;
		bool mIsExit;

		// emnu
	public:
		wxMenu *AddMainMenuItem(const std::string &title);
		wxMenu *AddSubMenuItem(wxMenu *menu, const std::string &title);
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
		std::map<std::string, wxMenu*> mMenuMap;
		std::map<int, std::string> mIDScripts;
		std::map<std::string, std::vector<wxMenuItem*> > mTagMenuItems;

		// event
	public:
		virtual void DoExecute(PX2::Event *event);

		// renderview
	public:
		RenderView *GetMainRenderView();

	protected:
		RenderView *mRenderView;

	protected:
		DECLARE_EVENT_TABLE()

		wxTimer mTimer;
		bool mIsInitlized;
		bool mIsSized;

		wxSize mSize;

		// PopUp Window
	public:
		void CreatePopUpWindow(const std::string &name);

		std::map<std::string, N_Frame*> mChildFrameMap;

		// general
	public:
		int MessageBox(const std::string &caption, const std::string &content,
			int type);
	};

}

#endif