// PX2UISlider.hpp

#ifndef PX2UISLIDER_HPP
#define PX2UISLIDER_HPP

#include "PX2UIFrame.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2UIButton.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UISlider : public UIFrame
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

		void SetSliderLength(float length);
		float GetSliderLength() const;

		void SetPercent(float percent);
		float GetPercent() const;

		void SetContentFrame(UIFrame *contentFrame);
		UIFrame *GetContentFrame();

		bool IsDraging() const;

		virtual void OnBeAttached();
		virtual void OnBeDetach();
	
	protected:
		virtual void OnWidgetPicked(const UIInputData &inputData);
		virtual void OnWidgetNotPicked(const UIInputData &inputData);

	protected:
		virtual void OnSizeChanged();
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		void _GenSlider();
		void _UpdateSliderLength();
		void _ReGenSliderLayout();
		void _UpdateSlider();
		void _SliderDrag(UIFrame *frame, UICallType type);
		void _SetDraging(bool draging);
		void _RelativeSizeChangeCallback(SizeNode *tellObject);

		bool mIsNeedReGenSliderLayout;

		DirectionType mDirectionType;
		UIFPicBoxPtr mFPicBoxBack;
		UIButtonPtr mButSlider;
		float mButSliderLength;
		float mPercent;
		bool mIsNeededUpdate;
		bool mIsDraging;

		UIFramePtr mContentFrame;
	};

	PX2_REGISTER_STREAM(UISlider);
	typedef PointerRef<UISlider> UISliderPtr;
#include "PX2UISlider.inl"

}

#endif