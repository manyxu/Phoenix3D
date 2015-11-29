// PX2BPGroup.inl

//----------------------------------------------------------------------------
inline bool BPGroup::IsEnable() const
{
	return mIsEnable;
}
//----------------------------------------------------------------------------
inline const std::string &BPGroup::GetTip() const
{
	return mTip;
}
//----------------------------------------------------------------------------
inline const Sizef &BPGroup::GetSize() const
{
	return mSize;
}
//----------------------------------------------------------------------------
inline int BPGroup::GetNumLogicGroups() const
{
	return (int)mLogicGroups.size();
}
//----------------------------------------------------------------------------
inline BPGroup *BPGroup::GetLogicGroup(int i)
{
	return mLogicGroups[i];
}
//----------------------------------------------------------------------------
inline int BPGroup::GetNumLogicFiles() const
{
	return (int)mLogicFiles.size();
}
//----------------------------------------------------------------------------
inline BPFile *BPGroup::GetLogicFile(int i)
{
	return mLogicFiles[i];
}
//----------------------------------------------------------------------------