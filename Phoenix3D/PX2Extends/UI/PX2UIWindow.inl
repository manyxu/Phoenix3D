// PX2UIWindow.inl

//----------------------------------------------------------------------------
inline void UIWindow::SetMain(bool main)
{
	mIsMain = main;
}
//----------------------------------------------------------------------------
inline bool UIWindow::IsMain() const
{
	return mIsMain;
}
//----------------------------------------------------------------------------
inline void UIWindow::SetFloat(bool fl)
{
	mIsFloat = fl;
}
//----------------------------------------------------------------------------
inline bool UIWindow::IsFloat() const
{
	return mIsFloat;
}
//----------------------------------------------------------------------------
inline void UIWindow::SetUICanvas(UICanvas *view)
{
	mView = view;
}
//----------------------------------------------------------------------------
inline UICanvas *UIWindow::GetUICanvas()
{
	return mView;
}
//----------------------------------------------------------------------------