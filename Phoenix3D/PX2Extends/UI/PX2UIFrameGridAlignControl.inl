// PX2UIFrameGridAlignControl.inl

//----------------------------------------------------------------------------
inline const Sizef &UIFrameGridAlignControl::GetCellSize() const
{
	return mCellSize;
}
//----------------------------------------------------------------------------
inline const Float2 &UIFrameGridAlignControl::GetSpacing() const
{
	return mSpacing;
}
//----------------------------------------------------------------------------
inline UIFrameGridAlignControl::StartCornerType UIFrameGridAlignControl::
GetStartCorner() const
{
	return mStartCornerType;
}
//----------------------------------------------------------------------------
inline UIFrameGridAlignControl::StartAxisType UIFrameGridAlignControl::
GetStartAxis() const
{
	return mStartAxisType;
}
//----------------------------------------------------------------------------
inline UIFrameGridAlignControl::ChildAlignmentType UIFrameGridAlignControl::
GetChildAlignment() const
{
	return mChildAlignmentType;
}
//----------------------------------------------------------------------------
inline UIFrameGridAlignControl::ConstraintType UIFrameGridAlignControl::
GetConstraintType() const
{
	return mConstraintType;
}
//----------------------------------------------------------------------------