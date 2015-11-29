// PX2N_Frame_Frames.cpp

#include "PX2N_Frame.hpp"
using namespace NA;
using namespace PX2;

//----------------------------------------------------------------------------
void N_Frame::CreatePopUpWindow(const std::string &name)
{
	N_Frame *frame = new N_Frame(this, 2, "Frame", "Frame", 0, 0, 600, 400,
		wxRESIZE_BORDER|wxFRAME_NO_TASKBAR|wxFRAME_FLOAT_ON_PARENT|
		wxCLIP_CHILDREN);
	frame->SetSize(wxSize(600, 200));
	frame->Show();

	mChildFrameMap[name] = frame;

	PX2_EW.ComeIn(frame);
}
//----------------------------------------------------------------------------