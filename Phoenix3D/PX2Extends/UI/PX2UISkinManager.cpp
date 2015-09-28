// PX2UISkinManager.cpp

#include "PX2UISkinManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
UISkinManager::UISkinManager()
{
	Size_Splitter = 5.0f;
	Color_Splitter = Float3::MakeColor(80, 80, 80);
	Color_Splitter_Draging = Float3::MakeColor(25, 25, 25);
}
//----------------------------------------------------------------------------
UISkinManager::~UISkinManager()
{
}
//----------------------------------------------------------------------------