// PX2UITabFrame.inl

//----------------------------------------------------------------------------
inline UITabFrame::LayoutPosType UITabFrame::GetLayoutPos() const
{
	return mLayoutPosType;
}
//----------------------------------------------------------------------------
inline bool UITabFrame::IsSkinAui() const
{
	return mIsSkinAui;
}
//----------------------------------------------------------------------------
inline float UITabFrame::GetTabWidth() const
{
	return mTabWidth;
}
//----------------------------------------------------------------------------
inline float UITabFrame::GetTabHeight() const
{
	return mTabHeight;
}
//----------------------------------------------------------------------------
inline UIAuiBlockFrame *UITabFrame::GetAuiBlockFrame()
{
	return mAuiBlockFrame;
}
//----------------------------------------------------------------------------