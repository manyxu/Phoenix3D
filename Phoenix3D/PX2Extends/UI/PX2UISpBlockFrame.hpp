// PX2UISpBlockFrame.hpp

#ifndef PX2UISPBLOCKFRAME_HPP
#define PX2UISPBLOCKFRAME_HPP

#include "PX2UIPre.hpp"
#include "PX2UISplitterFrame.hpp"
#include "PX2UIDefine.hpp"
#include "PX2UITabFrame.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM UISpBlockFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UISpBlockFrame);

	public:
		UISpBlockFrame(UILayOutPos posType);
		virtual ~UISpBlockFrame();

		virtual int AttachChild(Movable* child);

		UITabFrame *GetUITabFrame();

	protected:
		UILayOutPos mPosType;
		UITabFramePtr mUITabFrame;

		// ËÄÌõ±ß
	public:
		void SetSideFrameHor0(UISplitterFrame *frame);
		UISplitterFrame *GetSideFrameHor0();

		void SetSideFrameHor1(UISplitterFrame *frame);
		UISplitterFrame *GetSideFrameHor1();

		void SetSideFrameVer0(UISplitterFrame *frame);
		UISplitterFrame *GetSideFrameVer0() const;

		void SetSideFrameVer1(UISplitterFrame *frame);
		UISplitterFrame *GetSideFrameVer1() const;

	protected:
		UISplitterFrame *mSideFrameHor0;
		UISplitterFrame *mSideFrameHor1;
		UISplitterFrame *mSideFrameVer0;
		UISplitterFrame *mSideFrameVer1;

	protected:
		virtual void UpdateLayout();

	public:
		UISpBlockFrame *CreateAddSideFrame(UILayOutPos pos, const Sizef &size);

		UISpBlockFrame *GetSideFrame(UILayOutPos pos);

	protected:
		void SetParentSpBlockFrame(UISpBlockFrame *frame);
		UISpBlockFrame *GetParentSpBlockFrame();
		UISplitterFrame *_CalGetLinkFrame0(UILayOutPos pos);
		UISplitterFrame *_CalGetLinkFrame1(UILayOutPos pos);
		UISpBlockFrame *_GetTopestPosFrame(UILayOutPos pos);

		std::map<UILayOutPos, Pointer0<UISpBlockFrame> > mSideFrames;
		UISpBlockFrame *mParentSpBlockFrame;
	};

#include "PX2UISpBlockFrame.inl"
	PX2_REGISTER_STREAM(UISpBlockFrame);
	typedef Pointer0<UISpBlockFrame> UISpBlockFramePtr;

}

#endif