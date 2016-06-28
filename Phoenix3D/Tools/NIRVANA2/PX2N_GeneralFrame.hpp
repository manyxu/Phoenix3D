// PX2N_GeneralFrame.hpp

#ifndef PX2N_GENERALFRAME_HPP
#define PX2N_GENERALFRAME_HPP

#include "PX2N_Pre.hpp"

namespace NA
{

	class N_GeneralFrame : public wxFrame
	{
	public:
		N_GeneralFrame(wxFrame *parent);
		virtual ~N_GeneralFrame();

	protected:
		DECLARE_EVENT_TABLE()
	};

}

#endif