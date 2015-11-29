// PX2UICanvas.inl

//----------------------------------------------------------------------------
inline int UICanvas::GetViewID() const
{
	return mViewID;
}
//----------------------------------------------------------------------------
inline Movable *UICanvas::GetSuperTopMovbale()
{
	return mSuperTopMovable;
}
//----------------------------------------------------------------------------
inline CameraNode *UICanvas::GetCameraNode()
{
	return mCameraNode;
}
//----------------------------------------------------------------------------
inline void UICanvas::SetPickAcceptRect(const Rectf &rect)
{
	mPickAcceptRect = rect;
}
//----------------------------------------------------------------------------
inline const Rectf &UICanvas::GetPickAcceptRect() const
{
	return mPickAcceptRect;
}
//----------------------------------------------------------------------------
inline std::vector<RenderablePtr> &UICanvas::GetPickedRenderables()
{
	return mPickedRenderables;
}
//----------------------------------------------------------------------------
inline bool UICanvas::IsPressed() const
{
	return mIsPressed;
}
//----------------------------------------------------------------------------
inline const APoint &UICanvas::GetCurPickPos() const
{
	return mCurPickPos;
}
//----------------------------------------------------------------------------
inline const APoint &UICanvas::GetPressedPos() const
{
	return mPressedPos;
}
//----------------------------------------------------------------------------
inline const APoint &UICanvas::GetReleasedPos() const
{
	return mReleasedPos;
}
//----------------------------------------------------------------------------
inline UIFrame *UICanvas::GetParentUIFrame()
{
	return mParentUIFrame;
}
//----------------------------------------------------------------------------
inline void UICanvas::SetParentUIFrame(UIFrame *parentUIFrame)
{
	mParentUIFrame = parentUIFrame;
}
//----------------------------------------------------------------------------