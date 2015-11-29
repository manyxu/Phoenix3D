// PX2UIFramePicBox.hpp

#ifndef PX2UIFRAMEPICBOX_HPP
#define PX2UIFRAMEPICBOX_HPP

#include "PX2UIPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIPicBox.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM UIFPicBox : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UIFPicBox);

	public:
		UIFPicBox();
		virtual ~UIFPicBox();

		UIPicBox *GetUIPicBox();

		virtual void OnSizeChanged();

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);

		UIPicBoxPtr mPicBox;
	};
	
#include "PX2UIFPicBox.inl"
	PX2_REGISTER_STREAM(UIFPicBox);
	typedef Pointer0<UIFPicBox> UIFPicBoxPtr;

}

#endif