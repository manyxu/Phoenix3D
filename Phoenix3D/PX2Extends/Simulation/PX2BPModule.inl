// PX2BPModule.inl

//----------------------------------------------------------------------------
inline BPModule::ModuleType BPModule::GetModuleType() const
{
	return mModuleType;
}
//----------------------------------------------------------------------------
inline bool BPModule::IsEnable() const
{
	return mIsEnable;
}
//----------------------------------------------------------------------------
inline const std::map<std::string, BPParam*> &BPModule::
GetInputParams() const
{
	return mInputParams;
}
//----------------------------------------------------------------------------
inline const std::map<std::string, BPParam*> &BPModule::
GetOutputParams() const
{
	return mOutputParams;
}
//----------------------------------------------------------------------------
inline int BPModule::GetNumInputs() const
{
	return (int)mInputParams.size();
}
//----------------------------------------------------------------------------
inline int BPModule::GetNumOutputs() const
{
	return (int)mOutputParams.size();
}
//----------------------------------------------------------------------------
inline void BPModule::SetCompilingAsParam(bool isAsParam)
{
	mIsCompilingAsParam_DoNotNeedCall = isAsParam;
}
//----------------------------------------------------------------------------
inline bool BPModule::IsCompilingAsParam() const
{
	return mIsCompilingAsParam_DoNotNeedCall;
}
//----------------------------------------------------------------------------
inline const std::string &BPModule::GetModuleAs_ParamName() const
{
	return mModuleAs_ParamName;
}
//----------------------------------------------------------------------------
inline void BPModule::SetCompiled(bool compiled)
{
	mIsCompiled = compiled;
}
//----------------------------------------------------------------------------
inline bool BPModule::IsCompiled() const
{
	return mIsCompiled;
}
//----------------------------------------------------------------------------
inline const Sizef &BPModule::GetSize() const
{
	return mSize;
}
//----------------------------------------------------------------------------
inline float BPModule::GetItemHeight() const
{
	return mItemHeight;
}
//----------------------------------------------------------------------------
inline float BPModule::GetInOutButSize() const
{
	return mInOutButSize;
}
//----------------------------------------------------------------------------