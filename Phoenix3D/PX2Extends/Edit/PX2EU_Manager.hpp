// PX2EU_Manager.hpp

#ifndef PX2EU_MANAGER_HPP
#define PX2EU_MANAGER_HPP

#include "PX2EditPre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2UIMenu.hpp"
#include "PX2UIAuiFrame.hpp"
#include "PX2UIWindow.hpp"

namespace PX2
{

	class UICanvas;
	class UIFrame;
	class UIMenu;
	
	class PX2_EXTENDS_ITEM EU_Manager : public Singleton<EU_Manager>
	{
	public:
		EU_Manager();
		virtual ~EU_Manager();

		bool Initlize();
		bool Terminate();

		// windows
		void CreateUIWindowMain();
		UIWindow *GetUIWindowMain();

		UIWindow *CreateUIWindow(const std::string &name,
			const std::string title, bool isFloat, bool isMain);

		void CreateFrame_Main();
		UIFrame *GetFrame_Main();

		// top
		UIFrame *GetFrame_MainMenu();

		// center
		void CreateFrame_Center();

		// status
		void CreateFrame_StatusBar();

		// content
		void CreateFrame_Content();

		void CreateFrame_ProjectRes();

		void CreateFrame_Inspector();

		void CreateFrame_Console();

		UIAuiBlockFrame *CreateUIAuiBlockFrame(UIAuiBlockFrame *parent,
			UILayoutPosType pos, const Sizef &size = Sizef(200.0f, 200.0f));

	public_internal:
		std::map<std::string, Pointer0<UIWindow> > mWindowMap;

	protected:
		Pointer0<UIWindow> mUIWindowMain;
		Pointer0<UICanvas> mUICanvas_Main;
		Pointer0<UIFrame> mFrame_Main;
		UIAuiFramePtr mFrame_Content;
		Pointer0<UIFrame> mFrame_StatusBar;

		// ids
	public:
		int GetViewID(const std::string &viewName);

	private:
		int mNextViewID;
		std::map<std::string, int> mViewIDMap;

		// menus
	public:
		void Menu_Main_AddMainItem(const std::string &name, const std::string &title);
		void Menu_Main_AddSubItem(const std::string &parentName, const std::string &name,
			const std::string &title);
		void Menu_Main_AddItem(const std::string &parentName, const std::string &name,
			const std::string &title, const std::string &script, 
			const std::string &tag = "");
		void Menu_Main_AddItemSeparater(const std::string &parentName);

		// commonds
	public:
		void NewProject();
		void OpenProject();
		void SaveProject();
		void CloseProject();
		void NewScene();
		void OpenScene();
		void SaveScene();
		void SaveSceneAs();
		void CloseScene();
		void Exit();
	};

#define PX2EU_MAN EU_Manager::GetSingleton()

}

#endif