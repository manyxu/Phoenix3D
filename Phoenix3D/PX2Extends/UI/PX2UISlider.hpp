// PX2UISlider.hpp

#ifndef PX2UISLIDER_HPP
#define PX2UISLIDER_HPP

#include "PX2UIFrame.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2UIButton.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM UISlider : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(UISlider);

	public:
		UISlider();
		virtual ~UISlider();

		enum DirectionType
		{
			DT_HORIZONTAL,
			DT_VERTICALITY,
			DT_MAX_TYPE
		};
		void SetDirectionType(DirectionType dt);
		DirectionType GetDirectionType();

		void SetPercent(float percent);
		float GetPercent() const;

		bool IsDraging() const;	
	
	public_internal:
		virtual void OnUIPicked(int info, Movable *child);
		virtual void OnUINotPicked(int info);

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		void _GenSlider();
		void _UpdateSlider();
		void _SliderDrag(UIFrame *frame, UICallType type);
		void _SetDraging(bool draging);

		DirectionType mDirectionType;
		UIFPicBoxPtr mFPicBoxBack;
		UIButtonPtr mButSlider;
		float mButSliderWidth;
		float mPercent;
		bool mIsNeededUpdate;
		bool mIsDraging;

		APoint mDragStartPos;
	};

	PX2_REGISTER_STREAM(UISlider);
	typedef Pointer0<UISlider> UISliderPtr;
#include "PX2UISlider.inl"

}

#endif