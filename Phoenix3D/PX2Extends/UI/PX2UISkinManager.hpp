// PX2UISkinManager.hpp

#ifndef PX2UISKINMANAGER_HPP
#define PX2UISKINMANAGER_HPP

#include "PX2UIPre.hpp"
#include "PX2Singleton_NeedNew.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM UISkinManager : public Singleton<UISkinManager>
	{
	public:
		UISkinManager();
		virtual ~UISkinManager();

		// aui
	public:
		Float3 Color_ViewBackground;

		float Size_ToolBar;
		float Size_ToolBarBut;

		Float3 Color_ToolBar;
		Float3 Color_ToolBarSpliter;

		float Size_SearchBar;

		float Size_Splitter;
		Float3 Color_Splitter;
		Float3 Color_Splitter_Over;
		Float3 Color_Splitter_Draging;

		Float3 Color_AuiButTab_Normal;
		Float3 Color_AuiButTab_Horvered;
		Float3 Color_AuiButTab_Pressed;
		Float3 Color_AuiButTab_Active;

		Float3 Color_ButTab_Normal;
		Float3 Color_ButTab_Horvered;
		Float3 Color_ButTab_Pressed;
		Float3 Color_ButTab_Active;

		float Size_MainToolBar;
		Float3 Color_MainToolBar;

		float Size_StatusBar;
		Float3 Color_StatusBar;

		Float3 Color_SelectItem;

		Float3 Color_PropertyClass;
		float Size_PropertyFontScale;

		// font
		Float3 Color_ContentFont;
		Float3 Color_ContentFontDisable;
		Float3 Color_ContentBackground;

		Float3 Color_TableFont;
	};

#define PX2_UISM UISkinManager::GetSingleton()

}

#endif