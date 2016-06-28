// PX2N_Frame_Menu.cpp

#include "PX2N_Frame.hpp"
#include "PX2Edit.hpp"
#include "PX2ScriptManager.hpp"
using namespace NA;
using namespace PX2;

//----------------------------------------------------------------------------
void N_Frame::_CreateMenu()
{
	mMainMenuBar = new wxMenuBar();
	SetMenuBar(mMainMenuBar);
}
//----------------------------------------------------------------------------
void N_Frame::OnCommondItem(wxCommandEvent &e)
{
	int id = e.GetId();

	std::map<int, std::string>::iterator it = mIDScripts.find(id);

	if (it != mIDScripts.end())
	{
		std::string callStr = it->second;
		PX2_SC_LUA->CallString(callStr);
	}
}
//----------------------------------------------------------------------------
wxMenu *N_Frame::AddMainMenuItem(const std::string &title)
{
	wxMenu* menu = new wxMenu();
	mMainMenuBar->Append(menu, title);

	return menu;
}
//----------------------------------------------------------------------------
wxMenu *N_Frame::AddSubMenuItem(wxMenu *menu, const std::string &title)
{
	wxMenu *subMenu = new wxMenu();
	menu->AppendSubMenu(subMenu, title);

	return subMenu;
}
//----------------------------------------------------------------------------
wxMenuItem *N_Frame::AddMenuItem(wxMenu *menu,
	const std::string &title,
	const std::string &script,
	const std::string &tag)
{
	int id = PX2_EDIT_GETID;
	wxMenuItem *item = new wxMenuItem(menu, id, title);
	menu->Append(item);

	Connect(id, wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(N_Frame::OnCommondItem));

	mIDScripts[id] = script;

	if (!tag.empty())
	{
		mTagMenuItems[tag].push_back(item);
	}

	return item;
}
//----------------------------------------------------------------------------
void N_Frame::AddSeparater(wxMenu *menu)
{
	wxMenuItem *item = new wxMenuItem(menu, wxID_SEPARATOR);
	menu->Append(item);
}
//----------------------------------------------------------------------------
void N_Frame::EnableMenusTag(const std::string &tag, bool enable)
{
	std::map<std::string, std::vector<wxMenuItem*> >::iterator it =
		mTagMenuItems.find(tag);

	if (it != mTagMenuItems.end())
	{
		for (int i = 0; i < (int)it->second.size(); i++)
		{
			wxMenuItem *menuItem = it->second[i];
			menuItem->Enable(enable);
		}
	}
}
//----------------------------------------------------------------------------
wxMenu *N_Frame::CreateEditMenu()
{
	if (mEditMenu)
	{
		delete mEditMenu;
		mEditMenu = 0;
	}
	mMenuMap_Edit.clear();

	mEditMenu = new wxMenu();

	return mEditMenu;
}
//----------------------------------------------------------------------------
void N_Frame::PopUpRightMenu(int x, int y)
{
	mIsPopUpRightMenu = false;

	if (mEditMenu)
	{
		PopupMenu(mEditMenu, x, y);
	}
}
//----------------------------------------------------------------------------