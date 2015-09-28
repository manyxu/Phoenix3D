// PX2N_PopUpMenuWindow.cpp

#include "PX2N_PopUpMenuWindow.hpp"
using namespace NA;

BEGIN_EVENT_TABLE(N_PopUpMenuWindow, wxPopupTransientWindow)
END_EVENT_TABLE()
//----------------------------------------------------------------------------
N_PopUpMenuWindow::N_PopUpMenuWindow(wxWindow *parent)
{
	(void)Create(parent, wxBORDER_SIMPLE);
}
//----------------------------------------------------------------------------
N_PopUpMenuWindow::~N_PopUpMenuWindow()
{
}
//----------------------------------------------------------------------------
void N_PopUpMenuWindow::Popup(wxWindow *focus)
{
	wxPopupTransientWindow::Popup(focus);

	if ( !HasCapture() )
		CaptureMouse();
}
//----------------------------------------------------------------------------