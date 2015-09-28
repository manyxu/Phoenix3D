// PX2UIFrame.inl

//----------------------------------------------------------------------------
inline const Sizef &UIFrame::GetSize() const
{
	return mSize;
}
//----------------------------------------------------------------------------
inline float UIFrame::GetWidth() const
{
	return mSize.Width;
}
//----------------------------------------------------------------------------
inline float UIFrame::GetHeight() const
{
	return mSize.Height;
}
//----------------------------------------------------------------------------
inline const Sizef &UIFrame::GetBorderSize() const
{
	return mBorderSize;
}
//----------------------------------------------------------------------------
inline float UIFrame::GetBorderWidth() const
{
	return mBorderSize.Width;
}
//----------------------------------------------------------------------------
inline float UIFrame::GetBorderHeight() const
{
	return mBorderSize.Height;
}
//----------------------------------------------------------------------------
inline const Float2 &UIFrame::GetPvoit() const
{
	return mPvoit;
}
//----------------------------------------------------------------------------
inline const bool UIFrame::IsAnchorLayoutEnable() const
{
	return mIsAnchorLayoutEnable;
}
//----------------------------------------------------------------------------
inline const Float2 &UIFrame::GetAnchorHor() const
{
	return mAnchorHor;
}
//----------------------------------------------------------------------------
inline const Float2 &UIFrame::GetAnchorVer() const
{
	return mAnchorVer;
}
//----------------------------------------------------------------------------
inline const Float2 &UIFrame::GetAnchorParamHor() const
{
	return mAnchorParamHor;
}
//----------------------------------------------------------------------------
inline const Float2 &UIFrame::GetAnchorParamVer() const
{
	return mAnchorParamVer;
}
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
inline void UIFrame::SetUIView(UIView *uiView)
{
	mUIView = uiView;
}
//----------------------------------------------------------------------------
inline UIView *UIFrame::GetUIView()
{
	return mUIView;
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