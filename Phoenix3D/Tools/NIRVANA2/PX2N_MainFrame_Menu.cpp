// PX2N_MainFrame_Menu.cpp

#include "PX2N_MainFrame.hpp"
#include "PX2Edit.hpp"
#include "PX2ScriptManager.hpp"
using namespace NA;
using namespace PX2;

//----------------------------------------------------------------------------
void N_MainFrame::_CreateMenu()
{
	mMainMenuBar = new wxMenuBar();
	SetMenuBar(mMainMenuBar);
}
//----------------------------------------------------------------------------
void N_MainFrame::OnCommondItem(wxCommandEvent &e)
{
	int id = e.GetId();

	std::map<int, std::string>::iterator it = mIDScripts.find(id);

	if (it != mIDScripts.end())
	{
		std::string callStr = it->second;
		PX2_SM.CallString(callStr);
	}
}
//----------------------------------------------------------------------------
wxMenu *N_MainFrame::AddMainMenuItem(const std::string &title)
{
	wxMenu* menu = new wxMenu();
	mMainMenuBar->Append(menu, title);
	return menu;
}
//----------------------------------------------------------------------------
wxMenuItem *N_MainFrame::AddMenuItem(wxMenu *menu,
	const std::string &title,
	const std::string &script,
	const std::string &tag)
{
	int id = PX2_EDIT_GETID;
	wxMenuItem *item = new wxMenuItem(menu, id, title);
	menu->Append(item);

	Connect(id, wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(N_MainFrame::OnCommondItem));

	mIDScripts[id] = script;

	if (!tag.empty())
	{
		mTagMenuItems[tag].push_back(item);
	}

	return item;
}
//----------------------------------------------------------------------------
void N_MainFrame::AddSeparater(wxMenu *menu)
{
	wxMenuItem *item = new wxMenuItem(menu, wxID_SEPARATOR);
	menu->Append(item);
}
//----------------------------------------------------------------------------
void N_MainFrame::EnableMenusTag(const std::string &tag, bool enable)
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