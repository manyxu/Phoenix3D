// PX2UISlider.inl

//----------------------------------------------------------------------------
inline UISlider::DirectionType UISlider::GetDirectionType()
{
	return mDirectionType;
}
//----------------------------------------------------------------------------
inline float UISlider::GetPercent() const
{
	return mPercent;
}
//----------------------------------------------------------------------------
inline bool UISlider::IsDraging() const
{
	return mIsDraging;
}
//----------------------------------------------------------------------------