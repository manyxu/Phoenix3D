// PX2UIAuiBlockFrame.inl

//----------------------------------------------------------------------------
inline UILayoutPosType UIAuiBlockFrame::GetLayOutPosType() const
{
	return mPosType;
}
//----------------------------------------------------------------------------
inline UITabFrame *UIAuiBlockFrame::GetUITabFrame()
{
	return mUITabFrame;
}
//----------------------------------------------------------------------------
inline UISplitterFrame *UIAuiBlockFrame::GetSideFrameHor0()
{
	return mSideFrameHor0;
}
//----------------------------------------------------------------------------
inline bool UIAuiBlockFrame::IsSideFrameInnerHor0 () const
{
	return mIsInnerHor0;
}
//----------------------------------------------------------------------------
inline UISplitterFrame *UIAuiBlockFrame::GetSideFrameHor1()
{
	return mSideFrameHor1;
}
//----------------------------------------------------------------------------
inline bool UIAuiBlockFrame::IsSideFrameInnerHor1() const
{
	return mIsInnerHor1;
}
//----------------------------------------------------------------------------
inline UISplitterFrame *UIAuiBlockFrame::GetSideFrameVer0()
{
	return mSideFrameVer0;
}
//----------------------------------------------------------------------------
inline bool UIAuiBlockFrame::IsSideFrameInnerVer0 () const
{
	return mIsInnerVer0;
}
//----------------------------------------------------------------------------
inline UISplitterFrame *UIAuiBlockFrame::GetSideFrameVer1()
{
	return mSideFrameVer1;
}
//----------------------------------------------------------------------------
inline bool UIAuiBlockFrame::IsSideFrameInnerVer1 () const
{
	return mIsInnerVer1;
}
//----------------------------------------------------------------------------
inline UIAuiBlockFrame *UIAuiBlockFrame::GetParentSpBlockFrame()
{
	return mParentAuiBlockFrame;
}
//----------------------------------------------------------------------------