// PX2N_GeneralFrame.cpp

#include "PX2N_GeneralFrame.hpp"
using namespace NA;

BEGIN_EVENT_TABLE(N_GeneralFrame, wxFrame)
END_EVENT_TABLE();

//----------------------------------------------------------------------------
N_GeneralFrame::N_GeneralFrame(wxFrame *parent):
wxFrame(parent, -1, "GeneralFrame", wxDefaultPosition, wxDefaultSize, 
		wxRESIZE_BORDER|wxFRAME_NO_TASKBAR|wxFRAME_FLOAT_ON_PARENT)
{
}
//----------------------------------------------------------------------------
N_GeneralFrame::~N_GeneralFrame()
{
}
//----------------------------------------------------------------------------