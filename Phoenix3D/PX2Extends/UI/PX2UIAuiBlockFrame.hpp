// PX2UIAuiBlockFrame.hpp

#ifndef PX2UIAUIBLOCKFRAME_HPP
#define PX2UIAUIBLOCKFRAME_HPP

#include "PX2UIPre.hpp"
#include "PX2UISplitterFrame.hpp"
#include "PX2UIDefine.hpp"
#include "PX2UITabFrame.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM UIAuiBlockFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UIAuiBlockFrame);

	public:
		UIAuiBlockFrame(UILayoutPosType posType);
		virtual ~UIAuiBlockFrame();

		UILayoutPosType GetLayOutPosType() const;

		virtual int AttachChild(Movable* child);
		UITabFrame *GetUITabFrame();

		virtual void OnSizeChanged();

		void SetMinSize(const Sizef &minSize);
		const Sizef &GetMinSize() const;

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		void _CalSideFrames(std::vector<UISplitterFramePtr> &frames);

		UILayoutPosType mPosType;
		UITabFramePtr mUITabFrame;

		bool mIsFramesSortNeedUpdate;
		std::vector<UISplitterFramePtr > mSplitterFrames;
		std::vector<UISplitterFramePtr> mSplitterFrames_Hor;
		std::vector<UISplitterFramePtr> mSplitterFrames_Ver;
		Sizef mMinSize;

		// ËÄÌõ±ß
	public:
		void SetSideFrameHor0(UISplitterFrame *frame);
		UISplitterFrame *GetSideFrameHor0();
		bool IsSideFrameInnerHor0() const;

		void SetSideFrameHor1(UISplitterFrame *frame);
		UISplitterFrame *GetSideFrameHor1();
		bool IsSideFrameInnerHor1() const;

		void SetSideFrameVer0(UISplitterFrame *frame);
		UISplitterFrame *GetSideFrameVer0();
		bool IsSideFrameInnerVer0() const;

		void SetSideFrameVer1(UISplitterFrame *frame);
		UISplitterFrame *GetSideFrameVer1();
		bool IsSideFrameInnerVer1() const;

	public_internal:
		static void _UpdateLayout(UIAuiBlockFrame *auiBlockFrame,
			bool setToAuiBlockFrame = true,
			Sizef *outSize = 0, APoint *outPos = 0);

	protected:
		UISplitterFrame *mSideFrameHor0;
		UISplitterFrame *mSideFrameHor1;
		UISplitterFrame *mSideFrameVer0;
		UISplitterFrame *mSideFrameVer1;

	protected:
		virtual void UpdateLayout();

		UISplitterFrame *_CalGetNewSideFrameHor0();
		UISplitterFrame *_CalGetNewSideFrameHor1();
		UISplitterFrame *_CalGetNewSideFrameVer0();
		UISplitterFrame *_CalGetNewSideFrameVer1();

		UISplitterFrame *_CalGetLinkFrame0(UILayoutPosType pos);
		UISplitterFrame *_CalGetLinkFrame1(UILayoutPosType pos);
		UIAuiBlockFrame *_GetSideFrame(UILayoutPosType pos);

	public:
		UIAuiBlockFrame *CreateAddPosFrame(UILayoutPosType pos, const Sizef &size);
		UIAuiBlockFrame *GetPosFrame(UILayoutPosType pos);

	protected:
		void SetParentAuiBlockFrame(UIAuiBlockFrame *frame);
		UIAuiBlockFrame *GetParentSpBlockFrame();

		std::map<UILayoutPosType, Pointer0<UIAuiBlockFrame> > mSideFrames;
		UIAuiBlockFrame *mParentAuiBlockFrame;
	};

#include "PX2UIAuiBlockFrame.inl"
	PX2_REGISTER_STREAM(UIAuiBlockFrame);
	typedef Pointer0<UIAuiBlockFrame> UIAuiBlockFramePtr;

}

#endif