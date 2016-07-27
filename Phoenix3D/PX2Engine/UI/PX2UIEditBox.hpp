// PX2UIEditBox.hpp

#ifndef PX2UIEDITBOX_HPP
#define PX2UIEDITBOX_HPP

#include "PX2UIFrame.hpp"
#include "PX2UIInputText.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2InterpCurveAlphaCtrl.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIEditBox : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(UIEditBox);

	public:
		UIEditBox();
		virtual ~UIEditBox();

		virtual void Enable(bool enable);

		void SetPassword(bool isPassword);
		bool IsPassword() const;

		UIFPicBox *GetBackPicBox() const;

		void SetText(const std::string &text);
		const std::string &GetText() const;
		const std::string &GetRealText() const;
		UIInputText *GetInputText();

		void SetFixedWidth(float fixedWidth);
		float GetFixedWidth() const;

		virtual void OnSizeChanged();
		virtual void UpdateLeftBottomCornerOffset(Movable *parent);

		virtual void OnAttachWithIME();
		virtual void OnDetachWithIME();
		virtual void OnTextChanged();
		virtual void OnEnter(); // »Ø³µ

	protected:
		virtual void OnUIPicked(const UIInputData &data);
		virtual void OnUINotPicked(const UIInputData &data);
		void AttachIME();
		void DetachIME();
		void _AdjustFadePicBoxPos();

	protected:
		UIInputTextPtr mInputText;
		UIFPicBoxPtr mBackPicBox;
		UIFPicBoxPtr mFadePicBox;
		InterpCurveAlphaControllerPtr mFadeCtrl;

		float mFadePicBoxSideWidth;
		float mFixedWidth;

		bool mIsAttachedIME;
	};

	PX2_REGISTER_STREAM(UIEditBox);
	typedef PointerRef<UIEditBox> UIEditBoxPtr;
#include "PX2UIEditBox.inl"

}

#endif