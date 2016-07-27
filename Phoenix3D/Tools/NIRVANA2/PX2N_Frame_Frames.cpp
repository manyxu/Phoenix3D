// PX2N_Frame_Frames.cpp

#include "PX2N_Frame.hpp"
#include "PX2APoint.hpp"
#include "PX2EventWorld.hpp"
using namespace NA;
using namespace PX2;

//----------------------------------------------------------------------------
N_Frame *N_Frame::CreatePopUpWindow(RenderWindow *uiWindow)
{
	const APoint &pos = uiWindow->GetPosition();
	int x = (int)pos.X();
	int y = GetClientSize().GetHeight() - (int)pos.Z();

	x -= 50;
	y -= 5;

	//N_Frame *frame = new N_Frame(uiWindow, this,
	//	wxRESIZE_BORDER|wxFRAME_NO_TASKBAR|wxFRAME_FLOAT_ON_PARENT|wxCLIP_CHILDREN);

	N_Frame *frame = new N_Frame(uiWindow, this, wxDEFAULT_FRAME_STYLE);
	frame->Show();
	frame->SetSize(x, y, uiWindow->GetScreenSize().Width, 
		uiWindow->GetScreenSize().Height);

	mChildFrameMap[uiWindow->GetName()] = frame;

	PX2_EW.ComeIn(frame);

	return frame;
}
//----------------------------------------------------------------------------