// PX2N_PopUpMenuWindow.cpp

#include "PX2N_GeneralWindow.hpp"
using namespace NA;

IMPLEMENT_DYNAMIC_CLASS(N_GeneralWindow, wxWindow)
BEGIN_EVENT_TABLE(N_GeneralWindow, wxWindow)
END_EVENT_TABLE()
//----------------------------------------------------------------------------
N_GeneralWindow::N_GeneralWindow()
{
}
//----------------------------------------------------------------------------
N_GeneralWindow::N_GeneralWindow(wxWindow *parent) :
wxWindow(parent, -1, wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER)
{
}
//----------------------------------------------------------------------------
N_GeneralWindow::~N_GeneralWindow()
{
}
//----------------------------------------------------------------------------