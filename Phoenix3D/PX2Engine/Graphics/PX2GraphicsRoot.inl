// PX2GraphicsRoot.inl

//----------------------------------------------------------------------------
inline void GraphicsRoot::SetInEditor (bool isInEditor)
{
	mIsInEditor = isInEditor;
}
//----------------------------------------------------------------------------
inline bool GraphicsRoot::IsInEditor () const
{
	return mIsInEditor;
}
//----------------------------------------------------------------------------
inline void GraphicsRoot::SetCurEnvirParam(EnvirParam *param)
{
	mCurEnvirParam = param;
}
//----------------------------------------------------------------------------
inline EnvirParam *GraphicsRoot::GetCurEnvirParam()
{
	return mCurEnvirParam;
}
//----------------------------------------------------------------------------
inline RenderWindow *GraphicsRoot::GetMainWindow()
{
	return mMainRenderWindow;
}
//----------------------------------------------------------------------------
inline void GraphicsRoot::SetPlayType(PlayType type)
{
	mPlayType = type;
}
//----------------------------------------------------------------------------
inline GraphicsRoot::PlayType GraphicsRoot::GetPlayType() const
{
	return mPlayType;
}
//----------------------------------------------------------------------------
inline TriMesh *GraphicsRoot::GetXYPlane()
{
	return mTriMeshXY;
}
//----------------------------------------------------------------------------
inline TriMesh *GraphicsRoot::GetXZPlane()
{
	return mTriMeshXZ;
}
//----------------------------------------------------------------------------
inline TriMesh *GraphicsRoot::GetYZPlane()
{
	return mTriMeshYZ;
}
//----------------------------------------------------------------------------
