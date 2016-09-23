// PX2UISplitterFrame.hpp

#ifndef PX2UISPLITTERFRAME_HPP
#define PX2UISPLITTERFRAME_HPP

#include "PX2UIPre.hpp"
#include "PX2UIFrame.hpp"

namespace PX2
{

	class UIAuiBlockFrame;

	class PX2_ENGINE_ITEM UISplitterFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UISplitterFrame);

	public:
		UISplitterFrame(bool hor);
		virtual ~UISplitterFrame();

		bool IsHor() const;

		enum DragType
		{
			DT_VALUE,
			DT_PERCENT,
			DT_MAX_TYPE
		};
		void SetDragType(DragType dt);
		DragType GetDragType() const;

	protected:
		bool mIsHor;
		DragType mDragType;

	public:
		enum PosType
		{
			PT_NONE,
			PT_LEFT,
			PT_RIGHT,
			PT_BOTTOM,
			PT_TOP,
			PT_MAX_TYPE
		};
		void SetPosType(PosType pt);
		PosType GetPosType() const;

	protected:
		PosType mPosType;

	public:
		void SetDragable(bool isDragable);
		bool IsDragable() const;

		void SetDraging(bool isDraged);
		bool IsDraging() const;

		void OnDraging();

	protected:
		bool mIsDragable;
		bool mIsDraging;

	public_internal:
		bool IsCanDraging(float toParam);

		void _AddAuiBlockFrame0(UIAuiBlockFrame *blockframe);
		void _RemoveAuiBlockFrame0(UIAuiBlockFrame *blockframe);
		void _AddAuiBlockFrame1(UIAuiBlockFrame *blockframe);
		void _RemoveAuiBlockFrame1(UIAuiBlockFrame *blockframe);
		void _MarkBlockFramesChange();

	protected:
		std::vector<UIAuiBlockFrame*> mAuiBlockFrames0;
		std::vector<UIAuiBlockFrame*> mAuiBlockFrames1;

	protected:
		virtual void OnWidgetPicked(const UIInputData &inputData);
		virtual void OnWidgetNotPicked(const UIInputData &inputData);
		void _UpdateDraging(const UIInputData &inputData);
		void _SetOverMe(bool over);

		bool mIsOverMe;
	};

#include "PX2UISplitterFrame.inl"
	PX2_REGISTER_STREAM(UISplitterFrame);
	typedef PointerRef<UISplitterFrame> UISplitterFramePtr;

}

#endif