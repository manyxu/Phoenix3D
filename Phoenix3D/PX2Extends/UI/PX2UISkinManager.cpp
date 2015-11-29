// PX2UISkinManager.cpp

#include "PX2UISkinManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
UISkinManager::UISkinManager()
{
	Color_ViewBackground = Float3::MakeColor(30, 30, 30);

	Size_ToolBar = 20.0f;
	Color_ToolBar = Float3::MakeColor(100, 100, 100);

	Size_Splitter = 5.0f;
	Color_Splitter = Float3::MakeColor(80, 80, 80);
	Color_Splitter_Draging = Float3::MakeColor(25, 25, 25);

	Color_AuiButTab_Normal = Float3::MakeColor(120, 120, 120);
	Color_AuiButTab_Horvered = Float3::MakeColor(150, 150, 150);
	Color_AuiButTab_Pressed = Float3::MakeColor(180, 180, 180);
	Color_AuiButTab_Active = Float3::YELLOW;

	Color_ButTab_Normal = Float3::MakeColor(80, 80, 80);
	Color_ButTab_Horvered = Float3::MakeColor(110, 110, 110);
	Color_ButTab_Pressed = Float3::MakeColor(140, 140, 140);
	Color_ButTab_Active = Color_ViewBackground;

	Size_StatusBar = 25.0f;
	Color_StatusBar = Float3::MakeColor(104, 33, 122);
}
//----------------------------------------------------------------------------
UISkinManager::~UISkinManager()
{
}
//----------------------------------------------------------------------------