// PX2UIWindow.hpp

#ifndef PX2UIWINDOW_HPP
#define PX2UIWINDOW_HPP

#include "PX2UIPre.hpp"
#include "PX2UICanvas.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM UIWindow : public Object
	{
		PX2_DECLARE_RTTI;

	public:
		UIWindow();
		virtual ~UIWindow();

		void SetMain(bool main);
		bool IsMain() const;

		void SetFloat(bool fl);
		bool IsFloat() const;

		void SetUICanvas(UICanvas *view);
		UICanvas *GetUICanvas();

	protected:
		bool mIsMain;
		bool mIsFloat;
		UICanvasPtr mView;
	};

#include "PX2UIWindow.inl"
	typedef Pointer0<UIWindow> UIWindowPtr;

}

#endif