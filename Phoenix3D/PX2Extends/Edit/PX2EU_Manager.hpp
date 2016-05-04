// PX2EU_Manager.hpp

#ifndef PX2EU_MANAGER_HPP
#define PX2EU_MANAGER_HPP

#include "PX2EditPre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2UIMenu.hpp"
#include "PX2UIAuiFrame.hpp"
#include "PX2RenderWindow.hpp"
#include "PX2UICanvas.hpp"

namespace PX2
{

	class UICanvas;
	class UIFrame;
	class UIMenu;
	
	class PX2_EXTENDS_ITEM EU_Manager : public Singleton<EU_Manager>, public Visitor
	{
	public:
		EU_Manager();
		virtual ~EU_Manager();

		bool Initlize();
		bool Terminate();

		// windows
		void CreateUIWindowMain();
		RenderWindow *GetUIWindowMain();
		RenderWindow *CreateUIWindow(const std::string &name,
			const std::string &title, const APoint &pos, const Sizef &size, 
			bool isFloat);
		UIFrame *CreateMainFrame(Canvas *canvas, UIAuiFrame *&outAuiFrame);

		UIFrame *GetFrame_Main();

		// top
		UIFrame *GetFrame_MainMenu();

		// frames
		UIAuiBlockFrame *CreateFrame_Center();
		UIAuiBlockFrame *CreateFrame_Project(UIAuiBlockFrame *beforeCenterFrame, UIAuiBlockFrame *&outCenter);
		UIAuiBlockFrame *CreateFrame_Property(UIAuiBlockFrame *beforeCenterFrame, UIAuiBlockFrame *&outCenter);
		UIAuiBlockFrame *CreateFrame_Resource(UIAuiBlockFrame *beforeCenterFrame, UIAuiBlockFrame *&outCenter);
		UIAuiBlockFrame *CreateFrame_PreView(UIAuiBlockFrame *beforeCenterFrame, UIAuiBlockFrame *&outCenter);

		// toolbar
		void CreateFrame_ToolBar();

		// status
		void CreateFrame_StatusBar();

		UIAuiBlockFrame *CreateUIAuiBlockFrame(UIAuiBlockFrame *parent,
			UILayoutPosType pos, const Sizef &size = Sizef(300.0f, 300.0f));

		UIAuiBlockFrame *CreateUIAuiBlockFrame2(UIAuiBlockFrame *parent,
			UILayoutPosType pos, UIAuiBlockFrame *&outCenter, 
			const Sizef &size = Sizef(300.0f, 300.0f));

		virtual void Visit(Object *obj, int info);

	protected:
		PointerRef<UIFrame> mFrame_Main;
		UIAuiFramePtr mFrame_Content;
		UIFramePtr mFrame_ToolBar;
		PointerRef<UIFrame> mFrame_StatusBar;

		// menus
	public:
		void Menu_Main_AddMainItem(const std::string &name, const std::string &title);
		void Menu_Main_AddSubItem(const std::string &parentName, const std::string &name,
			const std::string &title);
		void Menu_Main_AddItem(const std::string &parentName, const std::string &name,
			const std::string &title, const std::string &script, 
			const std::string &tag = "");
		void Menu_Main_AddItemSeparater(const std::string &parentName);

		void Menu_Edit_Begin(const std::string &whe, const std::string &name);
		void Menu_Edit_AddSubItem(const std::string &whe,
			const std::string &parentName, const std::string &name,
			const std::string &title);
		void Menu_Edit_AddItem(const std::string &whe,
			const std::string &parentName, const std::string &name,
			const std::string &title,
			const std::string &script,
			const std::string &tag="");
		void Menu_Edit_AddItemSeparater(const std::string &whe,
			const std::string &parentName);
		void Menu_Edit_EndPopUp(const std::string &whe,
			const APoint &pos); // x z

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

		// tip
	public:
		void PlayTip(const std::string &title, const std::string &content);
	};

#define PX2EU_MAN EU_Manager::GetSingleton()

}

#endif