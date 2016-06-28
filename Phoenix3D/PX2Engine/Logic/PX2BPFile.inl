// BPFile.inl

//----------------------------------------------------------------------------
inline bool BPFile::IsEnable() const
{
	return mIsEnable;
}
//----------------------------------------------------------------------------
inline const std::string &BPFile::GetTip() const
{
	return mTip;
}
//----------------------------------------------------------------------------
inline int BPFile::GetNumBPModules() const
{
	return (int)mBPModules.size();
}
//----------------------------------------------------------------------------
inline BPModule *BPFile::GetBPModule(int i)
{
	return mBPModules[i];
}
//----------------------------------------------------------------------------
inline const std::string &BPFile::GetCompiledString() const
{
	return mCompiledString;
}
//----------------------------------------------------------------------------