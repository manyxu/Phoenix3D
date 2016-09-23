// PX2LogicManager.inl

//----------------------------------------------------------------------------
inline const FunObject *LogicManager::GetStartGF() const
{
	return &mFunStartObject;
}
//----------------------------------------------------------------------------
inline const std::vector<std::string> &LogicManager::GetGFs() const
{
	return mGeneralFunctions;
}
//----------------------------------------------------------------------------
inline const std::vector<std::string> &LogicManager::GetEventClass() const
{
	return mEventClasses;
}
//----------------------------------------------------------------------------
inline const std::map<std::string, std::vector<std::string> > &LogicManager
::GetEvents() const
{
	return mEvents;
}
//----------------------------------------------------------------------------
inline const std::vector<std::string> &LogicManager::GetOptions() const
{
	return mOption;
}
//----------------------------------------------------------------------------
inline std::map<std::string, FunObject> &LogicManager::GetOptionMap()
{
	return mOptionObjects;
}
//----------------------------------------------------------------------------
inline const std::vector<std::string> &LogicManager::GetParams() const
{
	return mParam;
}
//----------------------------------------------------------------------------
inline std::map<std::string, FunObject> &LogicManager::GetParamMap()
{
	return mParamObjects;
}
//----------------------------------------------------------------------------
inline const std::vector<std::string> &LogicManager::GetOperators() const
{
	return mOperators;
}
//----------------------------------------------------------------------------
inline std::map<std::string, FunObject> &LogicManager::GetOperatorMap()
{
	return mOperatorsObjects;
}
//----------------------------------------------------------------------------