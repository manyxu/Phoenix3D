// PX2EU_Manager.hpp

#ifndef PX2EU_MANAGER_HPP
#define PX2EU_MANAGER_HPP

#include "PX2EditPre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2UIMenu.hpp"
#include "PX2UIAuiFrame.hpp"

namespace PX2
{

	class UIView;
	class UIFrame;
	class UIMenu;
	
	class PX2_EXTENDS_ITEM EU_Manager : public Singleton<EU_Manager>
	{
	public:
		EU_Manager();
		virtual ~EU_Manager();

		bool Initlize();
		bool Ternamate();

		// view
		void CrateView_Main();
		UIView *GetView_Main();

		void CreateFrame_Main();
		UIFrame *GetFrame_Main();

		// top
		void CreateFrame_MainMenu();
		UIFrame *GetFrame_MainMenu();

		// status
		void CreateFrame_StatusBar();

		// content
		void CreateFrame_Content();

		void CreateFrame_Project();

		void CreateFrame_Inspector();

	protected:
		Pointer0<UIView> mUIView_Main;
		Pointer0<UIFrame> mFrame_Main;
		Pointer0<UIMenu> mFrame_MainMenu;
		float mMainMenuHeight;
		UIAuiFramePtr mFrame_Content;
		Pointer0<UIFrame> mFrame_StatusBar;
		float mStatusBarHeight;
	};

}

#endif