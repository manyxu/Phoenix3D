// PX2N_RenderView.hpp

#ifndef PX2N_RENDERVIEW_HPP
#define PX2N_RENDERVIEW_HPP

#include "PX2N_Pre.hpp"
#include "PX2EventHandler.hpp"
#include "PX2RenderWindow.hpp"

namespace NA
{

	class RenderView : public wxWindow, public PX2::EventHandler
	{
	public:
		RenderView(PX2::RenderWindow *renderWindow, wxWindow *parent);
		virtual ~RenderView();

		void OnTimer(wxTimerEvent& event);
		void OnSize(wxSizeEvent& e);
		void OnPaint(wxPaintEvent& e);
		void OnEraseBackground(wxEraseEvent& rEvent);
		void OnEnterWindow(wxMouseEvent& e);
		void OnLeaveWindow(wxMouseEvent& e);
		void OnLeftDown(wxMouseEvent& e);
		void OnLeftUp(wxMouseEvent& e);
		void OnLeftDoubleClick(wxMouseEvent& e);
		void OnMiddleDown(wxMouseEvent& e);
		void OnMiddleUp(wxMouseEvent& e);
		void OnMouseWheel(wxMouseEvent& e);
		void OnRightDown(wxMouseEvent& e);
		void OnRightUp(wxMouseEvent& e);
		void OnMotion(wxMouseEvent& e);
		void OnKeyDown(wxKeyEvent& e);
		void OnKeyUp(wxKeyEvent& e);
		void OnChar(wxKeyEvent& e);

		void OnMenuItem(wxCommandEvent &e);

		virtual void DoExecute(PX2::Event *event);

	protected:
		DECLARE_EVENT_TABLE()

		RenderView();

	protected:
		bool mIsInited;

		PX2::RenderWindow *mRenderWindow;

		int mTimerID;
		wxTimer mTimer;
		wxSize mSize;

		bool mIsRightDown;
		bool mIsRightDownOnMotion;

	protected:
		void OnCommondItem(wxCommandEvent &e);
	};

}

#endif