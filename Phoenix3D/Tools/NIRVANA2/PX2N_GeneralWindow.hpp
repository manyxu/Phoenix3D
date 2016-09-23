// PX2N_GeneralWindow.hpp

#ifndef PX2N_GENERALWINDOW_HPP
#define PX2N_GENERALWINDOW_HPP

#include "PX2N_Pre.hpp"

namespace NA
{

	class N_GeneralWindow : public wxWindow
	{
		DECLARE_DYNAMIC_CLASS(N_PopUpMenuWindow)

	public:
		N_GeneralWindow(wxWindow *parent);
		virtual ~N_GeneralWindow();

		void Popup(wxWindow *focus);

	protected:
		DECLARE_EVENT_TABLE()

		N_GeneralWindow();
	};

}

#endif