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
inline const Sizef &UIAuiBlockFrame::GetMinSize() const
{
	return mMinSize;
}
//----------------------------------------------------------------------------
inline UISplitterFrame *UIAuiBlockFrame::GetSideFrameHor0()
{
	return mSideFrameHor0;
}
//----------------------------------------------------------------------------
inline UISplitterFrame *UIAuiBlockFrame::GetSideFrameHor1()
{
	return mSideFrameHor1;
}
//----------------------------------------------------------------------------
inline UISplitterFrame *UIAuiBlockFrame::GetSideFrameVer0()
{
	return mSideFrameVer0;
}
//----------------------------------------------------------------------------
inline UISplitterFrame *UIAuiBlockFrame::GetSideFrameVer1()
{
	return mSideFrameVer1;
}
//----------------------------------------------------------------------------
inline UIAuiBlockFrame *UIAuiBlockFrame::GetParentSpBlockFrame()
{
	return mParentAuiBlockFrame;
}
//----------------------------------------------------------------------------