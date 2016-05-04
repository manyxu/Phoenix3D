// PX2Character.inl

//----------------------------------------------------------------------------
inline void Character::SetMovableUseAutoWorldBound(bool isUseAutoWorldBound)
{
	mIsMovableAutoWorldBound = isUseAutoWorldBound;
}
//----------------------------------------------------------------------------
inline bool Character::IsMovableUseAutoWorldBound() const
{
	return mIsMovableAutoWorldBound;
}
//----------------------------------------------------------------------------
inline float Character::GetMovableAutoWorldBoundRadius() const
{
	return mMovableAutoWorldBoundRadius;
}
//----------------------------------------------------------------------------
inline void Character::SetAnimType(AnimType type)
{
	mAnimType = type;
}
//----------------------------------------------------------------------------
inline Character::AnimType Character::GetAnimType() const
{
	return mAnimType;
}
//----------------------------------------------------------------------------
inline std::map<int, AnimationPtr> &Character::GetAnimsMap()
{
	return mAnimsMap;
}
//----------------------------------------------------------------------------
inline int Character::GetDefaultAnimID() const
{
	return mDefaultAnimID;
}
//----------------------------------------------------------------------------
inline Animation *Character::GetDefaultAnim()
{
	return mDefaultAnim;
}
//----------------------------------------------------------------------------
inline Animation *Character::GetCurPlayingAnim()
{
	return mCurPlayingAnim;
}
//----------------------------------------------------------------------------