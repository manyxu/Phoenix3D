// PX2N_Frame_General.cpp

#include "PX2N_Frame.hpp"
using namespace NA;

//----------------------------------------------------------------------------
int N_Frame::MessageBox(const std::string &caption, const std::string &content,
	int type)
{
	int theWxType = wxOK;
	if (0 == type)
		theWxType = wxOK;
	else if (1 == type)
		theWxType = wxYES_NO;

	return wxMessageBox(content, caption, theWxType);
}
//----------------------------------------------------------------------------