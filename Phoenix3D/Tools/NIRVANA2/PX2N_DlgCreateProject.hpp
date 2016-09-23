// PX2DlgCreateProject.hpp

#ifndef PX2N_DLGCREATEPROJECT_HPP
#define PX2N_DLGCREATEPROJECT_HPP

#include "PX2N_Pre.hpp"

namespace NA
{

	class DlgCreateProject : public wxDialog
	{
	public:
		DlgCreateProject(wxWindow *parent);

		void OnButton(wxCommandEvent& event);

		std::string mProjName;
		int mScreenOrientation;
		int mProjWidth;
		int mProjHeight;

	protected:
		DECLARE_EVENT_TABLE();

		wxTextCtrl *mName;
		wxChoice *mChoiceScreenOrientation;
		wxTextCtrl *mWidth;
		wxTextCtrl *mHeight;
		wxButton *mOK;
		wxButton *mCancel;
	};

}

#endif