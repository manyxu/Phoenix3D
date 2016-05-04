// start.as

void start()
{
	PX2_LOGGER().LogInfo("project_start", "start");
	
	Scene @scene = Scene();
	PX2_PROJ().SetScene(scene);
	
	UIFrame @uiFrame = UIFrame();
	PX2_PROJ().SetUIFrame(uiFrame);
	uiFrame.SetAnchorHor(0.0f, 1.0f);
	uiFrame.SetAnchorVer(0.0f, 1.0f);
	
	UIFPicBox @fPicBox = UIFPicBox();
	fPicBox.SetAnchorHor(0.0f, 0.5f);
	fPicBox.SetAnchorVer(0.0f, 0.5f);
	uiFrame.AttachChild(fPicBox);
}