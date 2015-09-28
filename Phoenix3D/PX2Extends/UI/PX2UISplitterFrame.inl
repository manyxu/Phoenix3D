// PX2UISplitterFrame.inl

//----------------------------------------------------------------------------
inline bool UISplitterFrame::IsHor() const
{
	return mIsHor;
}
//----------------------------------------------------------------------------
inline UISplitterFrame *UISplitterFrame::GetLinkFrame0()
{
	return mLinkFrame0;
}
//----------------------------------------------------------------------------
inline UISplitterFrame *UISplitterFrame::GetLinkFrame1()
{
	return mLinkFrame1;
}
//----------------------------------------------------------------------------
inline bool UISplitterFrame::IsDragable() const
{
	return mIsDragable;
}
//----------------------------------------------------------------------------
inline bool UISplitterFrame::IsDraging() const
{
	return mIsDraging;
}
//----------------------------------------------------------------------------