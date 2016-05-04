// PX2EngineLoop.inl

//----------------------------------------------------------------------------
inline float EngineLoop::GetElapsedTime()
{
	return (float)(mAppTime - mLastAppTime);
}
//----------------------------------------------------------------------------
inline const Sizef &EngineLoop::GetBoostSize() const
{
	return mBoostSize;
}
//----------------------------------------------------------------------------
inline const std::string &EngineLoop::GetProjectName() const
{
	return mProjectName;
}
//----------------------------------------------------------------------------
inline const std::string &EngineLoop::GetProjectFilePath() const
{
	return mProjectFilePath;
}
//----------------------------------------------------------------------------
inline const Sizef &EngineLoop::GetScreenSize() const
{
	return mScreenSize;
}
//----------------------------------------------------------------------------
inline EngineLoop::PlayType EngineLoop::GetPlayType() const
{
	return mPlayType;
}
//----------------------------------------------------------------------------
inline General_EventHandler *EngineLoop::GetGeneral_EventHandler()
{
	return mSimuES_EventHandler;
}
//----------------------------------------------------------------------------