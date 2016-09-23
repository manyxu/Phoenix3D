// PX2Scene.inl

//----------------------------------------------------------------------------
inline EnvirParamController *Scene::GetEnvirParamController()
{
	return mEnvirParam;
}
//----------------------------------------------------------------------------
inline std::vector<CameraPtr> &Scene::GetCameras()
{
	return mCameras;
}
//----------------------------------------------------------------------------