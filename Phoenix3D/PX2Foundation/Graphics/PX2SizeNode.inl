// PX2SizeNode.inl

//----------------------------------------------------------------------------
inline const Sizef &SizeNode::GetSize() const
{
	return mSize;
}
//----------------------------------------------------------------------------
inline float SizeNode::GetWidth() const
{
	return mSize.Width;
}
//----------------------------------------------------------------------------
inline float SizeNode::GetHeight() const
{
	return mSize.Height;
}
//----------------------------------------------------------------------------
inline const Sizef &SizeNode::GetBorderSize() const
{
	return mBorderSize;
}
//----------------------------------------------------------------------------
inline float SizeNode::GetBorderWidth() const
{
	return mBorderSize.Width;
}
//----------------------------------------------------------------------------
inline float SizeNode::GetBorderHeight() const
{
	return mBorderSize.Height;
}
//----------------------------------------------------------------------------
inline const Float2 &SizeNode::GetPvoit() const
{
	return mPvoit;
}
//----------------------------------------------------------------------------
inline const bool SizeNode::IsAnchorLayoutEnable() const
{
	return mIsAnchorLayoutEnable;
}
//----------------------------------------------------------------------------
inline const Float2 &SizeNode::GetAnchorHor() const
{
	return mAnchorHor;
}
//----------------------------------------------------------------------------
inline const Float2 &SizeNode::GetAnchorVer() const
{
	return mAnchorVer;
}
//----------------------------------------------------------------------------
inline const Float2 &SizeNode::GetAnchorParamHor() const
{
	return mAnchorParamHor;
}
//----------------------------------------------------------------------------
inline const Float2 &SizeNode::GetAnchorParamVer() const
{
	return mAnchorParamVer;
}