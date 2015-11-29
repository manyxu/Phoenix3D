// PX2BPManager.inl

//----------------------------------------------------------------------------
inline const Object::FunObject *BPManager::GetStartGF() const
{
	return &mFunStartObject;
}
//----------------------------------------------------------------------------
inline const std::vector<std::string> &BPManager::GetGFs() const
{
	return mGeneralFunctions;
}
//----------------------------------------------------------------------------
inline const std::vector<std::string> &BPManager::GetEvents() const
{
	return mEvents;
}
//----------------------------------------------------------------------------
inline std::map<std::string, std::vector<Object::FunObject> > &BPManager
::GetObjectFunMap()
{
	return mObjectFunMap;
}
//----------------------------------------------------------------------------
inline const std::vector<std::string> &BPManager::GetOptions() const
{
	return mOption;
}
//----------------------------------------------------------------------------
inline std::map<std::string, Object::FunObject> &BPManager::GetOptionMap()
{
	return mOptionObjects;
}
//----------------------------------------------------------------------------
inline const std::vector<std::string> &BPManager::GetParams() const
{
	return mParam;
}
//----------------------------------------------------------------------------
inline std::map<std::string, Object::FunObject> &BPManager::GetParamMap()
{
	return mParamObjects;
}
//----------------------------------------------------------------------------
inline const std::vector<std::string> &BPManager::GetOperators() const
{
	return mOperators;
}
//----------------------------------------------------------------------------
inline std::map<std::string, Object::FunObject> &BPManager::GetOperatorMap()
{
	return mOperatorsObjects;
}
//----------------------------------------------------------------------------