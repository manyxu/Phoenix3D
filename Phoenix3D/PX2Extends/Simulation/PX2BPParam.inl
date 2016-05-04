// PX2BPParam.inl

//----------------------------------------------------------------------------
inline bool BPParam::IsIn() const
{
	return mIsIn;
}
//----------------------------------------------------------------------------
inline bool BPParam::IsExe() const
{
	return mIsExe;
}
//----------------------------------------------------------------------------
inline FunParamType BPParam::GetDataType() const
{
	return mDataType;
}
//----------------------------------------------------------------------------
inline const Any &BPParam::GetValue() const
{
	return mData;
}
//----------------------------------------------------------------------------
inline const std::string &BPParam::GetOutputScriptVarString() const
{
	return mOutputScriptVarString;
}
//----------------------------------------------------------------------------
inline BPModule *BPParam::GetModule()
{
	return mModule;
}
//----------------------------------------------------------------------------
inline const BPModule *BPParam::GetModule() const
{
	return mModule;
}
//----------------------------------------------------------------------------
inline bool BPParam::IsOwnObjectParam() const
{
	return FPT_POINTER_THIS == mDataType ||
		FPT_POINTER_THIS_STATIC == mDataType;
}
//----------------------------------------------------------------------------
inline int BPParam::GetNumLinkToParams() const
{
	return (int)mLinkToParamVec.size();
}
//----------------------------------------------------------------------------
inline BPParam *BPParam::GetLinkToParam(int index)
{
	return mLinkToParamVec[index].TheParam;
}
//----------------------------------------------------------------------------
inline int BPParam::GetNumLinkMeParams() const
{
	return (int)mLinkMeParams.size();
}
//----------------------------------------------------------------------------
inline std::vector<BPParam *> &BPParam::GetLinkMeParams()
{
	return mLinkMeParams;
}
//----------------------------------------------------------------------------
inline BPParam *BPParam::GetLinkMeParam(int i)
{
	return mLinkMeParams[i];
}
//----------------------------------------------------------------------------
inline UIButton *BPParam::GetButton()
{
	return mBut;
}
//----------------------------------------------------------------------------
inline UIFText *BPParam::GetNameText()
{
	return mNameText;
}
//----------------------------------------------------------------------------