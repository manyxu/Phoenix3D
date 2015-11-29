// PX2UISplitterFrame.hpp

#ifndef PX2UISPLITTERFRAME_HPP
#define PX2UISPLITTERFRAME_HPP

#include "PX2UIPre.hpp"
#include "PX2UIFrame.hpp"

namespace PX2
{

	class UIAuiBlockFrame;

	class PX2_EXTENDS_ITEM UISplitterFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UISplitterFrame);

	public:
		UISplitterFrame(bool hor);
		virtual ~UISplitterFrame();

		bool IsHor() const;

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		bool mIsHor;

	public:
		void SetLinkFrame0(UISplitterFrame *frame);
		UISplitterFrame *GetLinkFrame0();

		void SetLinkFrame1(UISplitterFrame *frame);
		UISplitterFrame *GetLinkFrame1();

	protected:
		bool IsHasLinkMeSpFrame(UISplitterFrame *frame);
		void AddLinkMeSpFrame(UISplitterFrame *frame);
		void RemoveLinkMeSpFrame(UISplitterFrame *frame);
		void _MarkLinkMeFramesChange();

		bool mIsLinkFrameChanged;

		UISplitterFrame *mLinkFrame0;
		UISplitterFrame *mLinkFrame1;
		std::vector<UISplitterFrame*> mLinkMeSpFrames;

	public:
		void SetDragable(bool isDragable);
		bool IsDragable() const;

		void SetDraging(bool isDraged);
		bool IsDraging() const;

	protected:
		bool mIsDragable;
		bool mIsDraging;

public_internal:
		void _AddAuiBlockFrame(UIAuiBlockFrame *blockframe);
		void _RemoveAuiBlockFrame(UIAuiBlockFrame *blockframe);
		bool _IsCanMakeBlcokFramesChange();
		void _MarkBlockFramesChange();

	protected:
		std::vector<UIAuiBlockFrame*> mAuiBlockFrames;

public_internal:
		virtual void OnNotPicked(int info);
		virtual void OnUIPicked(int info, Movable *child);

		virtual void DoExecute(Event *event);
	};

#include "PX2UISplitterFrame.inl"
	PX2_REGISTER_STREAM(UISplitterFrame);
	typedef Pointer0<UISplitterFrame> UISplitterFramePtr;

}

#endif