// PX2BPPackage.inl

//----------------------------------------------------------------------------
inline bool BPPackage::IsEnable() const
{
	return mIsEnable;
}
//----------------------------------------------------------------------------
inline const std::string &BPPackage::GetTip() const
{
	return mTip;
}
//----------------------------------------------------------------------------
inline int BPPackage::GetNumBPPackages() const
{
	return (int)mBPPackages.size();
}
//----------------------------------------------------------------------------
inline BPPackage *BPPackage::GetBPPackage(int i)
{
	return mBPPackages[i];
}
//----------------------------------------------------------------------------
inline int BPPackage::GetNumBPFiles() const
{
	return (int)mBPFiles.size();
}
//----------------------------------------------------------------------------
inline BPFile *BPPackage::GetBPFile(int i)
{
	return mBPFiles[i];
}
//----------------------------------------------------------------------------