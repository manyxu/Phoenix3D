// PX2UIView.inl

//----------------------------------------------------------------------------
inline const Sizef &Canvas::GetScreenSize() const
{
	return mScreenSize;
}
//----------------------------------------------------------------------------
inline const Rectf &Canvas::GetViewPort() const
{
	return mViewPort;
}
//----------------------------------------------------------------------------
inline Renderer *Canvas::GetRenderer()
{
	return mRenderer;
}
//----------------------------------------------------------------------------
inline Camera *Canvas::GetCamera()
{
	return mCamera;
}
//----------------------------------------------------------------------------
inline Culler &Canvas::GetCuller()
{
	return mCuller;
}
//----------------------------------------------------------------------------
inline void Canvas::SetPriority(int priority)
{
	mPriority = priority;
}
//----------------------------------------------------------------------------
inline int Canvas::GetPriority() const
{
	return mPriority;
}
//----------------------------------------------------------------------------
inline const Float4 &Canvas::GetClearColor() const
{
	return mClearColor;
}
//----------------------------------------------------------------------------