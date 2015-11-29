// PX2UIFText.hpp

#ifndef PX2UIFTEXT_HPP
#define PX2UIFTEXT_HPP

#include "PX2UIPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIText.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM UIFText : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UIFText);

	public:
		UIFText();
		virtual ~UIFText();

		UIText *GetText();

		virtual void OnSizeChanged();

	protected:
		UITextPtr mText;
	};

#include "PX2UIFText.inl"
	PX2_REGISTER_STREAM(UIFText);
	typedef Pointer0<UIFText> UIFTextPtr;

}

#endif