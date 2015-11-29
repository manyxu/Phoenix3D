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
inline const Sizef &BPFile::GetSize() const
{
	return mSize;
}
//----------------------------------------------------------------------------
inline int BPFile::GetNumLogicModules() const
{
	return (int)mBPModules.size();
}
//----------------------------------------------------------------------------
inline BPModule *BPFile::GetLogicModule(int i)
{
	return mBPModules[i];
}
//----------------------------------------------------------------------------
inline const std::string &BPFile::GetCompiledString() const
{
	return mCompiledString;
}
//----------------------------------------------------------------------------