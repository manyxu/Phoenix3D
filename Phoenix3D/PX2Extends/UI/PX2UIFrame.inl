// PX2UIFrame.inl

//----------------------------------------------------------------------------
inline void UIFrame::SetUICallback(UICallback callback)
{
	mUICallback = callback;
}
//----------------------------------------------------------------------------
inline UICallback UIFrame::GetUICallback() const
{
	return mUICallback;
}
//----------------------------------------------------------------------------
inline void UIFrame::SetMemUICallback(UIFrame *object, MemUICallback callback)
{
	mMemObject = object;
	mMemUICallback = callback;
}
//----------------------------------------------------------------------------
inline UIFrame::MemUICallback UIFrame::GetMemUICallback() const
{
	return mMemUICallback;
}
//----------------------------------------------------------------------------
inline void UIFrame::SetScriptHandler(const std::string &scriptHandler)
{
	mUIScriptHandler = scriptHandler;
}
//----------------------------------------------------------------------------
inline const std::string &UIFrame::GetScriptHandler() const
{
	return mUIScriptHandler;
}
//----------------------------------------------------------------------------
inline InputPushTransformController *UIFrame::GetIPTCtrl()
{
	return mIPTCtrl;
}
//----------------------------------------------------------------------------
inline UIFrameGridAlignControl *UIFrame::GetGridAlignCtrl()
{
	return mGridAlignCtrl;
}
//----------------------------------------------------------------------------