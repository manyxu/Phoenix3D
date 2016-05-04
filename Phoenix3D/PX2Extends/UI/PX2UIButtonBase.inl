// PX2UIButtonBase.inl

//----------------------------------------------------------------------------
inline UIButtonBase::ButType UIButtonBase::GetButType() const
{
	return mButType;
}
//----------------------------------------------------------------------------
inline UIButtonBase::ButtonState UIButtonBase::GetButtonState() const
{
	return mButtonState;
}
//----------------------------------------------------------------------------
inline UIText *UIButtonBase::GetText()
{
	return mFText->GetText();
}
//----------------------------------------------------------------------------