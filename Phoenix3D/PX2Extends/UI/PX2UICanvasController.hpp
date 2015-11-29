// PX2UICanvasController.hpp

#ifndef PX2UICONVASCONTROLLER_HPP
#define PX2UICONVASCONTROLLER_HPP

#include "PX2UIPre.hpp"
#include "PX2Controller.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM UICanvasController : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(UICanvasController);

	public:
		UICanvasController();
		virtual ~UICanvasController();

		virtual void DoExecute(Event *event);

	protected:
		bool mIsRightPressed;
		APoint mRightPressedPos;
		APoint mCurPos;
		APoint mLastPos;
	};

#include "PX2UICanvasController.inl"
	PX2_REGISTER_STREAM(UICanvasController);
	typedef Pointer0<UICanvasController> UICanvasControllerPtr;

}

#endif