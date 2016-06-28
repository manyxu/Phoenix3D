// PX2N_Frame.hpp

#ifndef PX2N_FRAME_HPP
#define PX2N_FRAME_HPP

#include "PX2N_Pre.hpp"
#include "PX2N_GeneralWindow.hpp"
#include "PX2N_RenderView.hpp"
#include "PX2RenderWindow.hpp"

namespace NA
{
	
	class N_Frame : public wxFrame, public PX2::EventHandler
	{
	public:
		N_Frame(PX2::RenderWindow *uiWindow, wxWindow *parent, long style);
		virtual ~N_Frame();

		RenderView *GerRenderView();

		void OnTimer(wxTimerEvent& e);
		void OnSize(wxSizeEvent& e);

	protected:
		void _Init();

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
		bool mIsNWindow;
		PX2::RenderWindow *mNUIWindow;

		// main emnu
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

		// edit menu
	public:
		wxMenu *CreateEditMenu();
		void PopUpRightMenu(int x, int y);

	protected:
		wxMenu *mEditMenu;
		std::map<std::string, wxMenu*> mMenuMap_Edit;
		std::map<std::string, std::vector<wxMenuItem*> > mTagMenuItems_Edit;

		PX2::APoint mPopUpRightMenuPos;
		bool mIsPopUpRightMenu;

		// event
	public:
		virtual void DoExecute(PX2::Event *event);

		// renderview
	public:
		RenderView *GetMainRenderView();

	protected:
		PX2::RenderWindow *mRenderWindow;
		RenderView *mRenderView;

	protected:
		DECLARE_EVENT_TABLE()

		wxTimer mTimer;
		bool mIsInitlized;
		bool mIsSized;
		int mTimerID;

		wxSize mSize;

		// PopUp Window
	public:
		N_Frame *CreatePopUpWindow(PX2::RenderWindow *uiWindow);

		std::map<std::string, N_Frame*> mChildFrameMap;

		// general
	public:
		int MessageBox(const std::string &caption, const std::string &content,
			int type);
	};

}

#endif