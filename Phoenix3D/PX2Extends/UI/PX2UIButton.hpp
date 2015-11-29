// PX2UIButton.hpp

#ifndef PX2UIBUTTON_HPP
#define PX2UIBUTTON_HPP

#include "PX2UIButtonBase.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM UIButton : public UIButtonBase
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(UIButton);

	public:
		UIButton();
		virtual ~UIButton();

		virtual void Enable(bool enable);

		enum AfterReleasedType
		{
			ART_NORMAL,
			ART_DISABLE,
			ART_DISABLE_RECOVER,
			ART_MAX_TYPE
		};
		void SetAfterReleasedType(AfterReleasedType type);
		AfterReleasedType GetAfterReleasedType() const;
		void SetAfterReleasedRecoverTime(float time);
		float GetAfterReleasedRecoverTime() const;

		virtual void OnEvent(Event *event);

		void OnPressed();
		void OnReleased();

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		virtual void OnUIPicked(int info, Movable *child);
		virtual void OnUINotPicked(int info);

		AfterReleasedType mAfterReleasedType;
		float mAfterReleasedRecoverTime;
		bool mIsRecoverBegin;
		float mRecoverBeginTime;
	};

#include "PX2UIButton.inl"
	PX2_REGISTER_STREAM(UIButton);
	typedef Pointer0<UIButton> UIButtonPtr;

}

#endif