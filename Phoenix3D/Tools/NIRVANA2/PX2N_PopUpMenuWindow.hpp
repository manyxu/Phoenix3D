// PX2N_PopUpWindow.hpp

#ifndef PX2N_POPUPMENUWINDOW_HPP
#define PX2N_POPUPMENUWINDOW_HPP

#include "PX2N_Pre.hpp"

namespace NA
{

	class N_PopUpMenuWindow : public wxPopupTransientWindow
	{
	public:
		 N_PopUpMenuWindow(wxWindow *parent);
		 virtual ~N_PopUpMenuWindow();

		 void Popup(wxWindow *focus);

	protected:
		DECLARE_EVENT_TABLE()
	};

}

#endif