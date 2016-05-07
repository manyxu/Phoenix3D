// start.as

UIFrame @frameCenter = null;
void UICallback(Object @obj, int type)
{	
	if (type == UICT_RELEASED)
	{
		UIFrame @frame = cast<UIFrame>(obj);
		if (frame !is null)
		{
			frameCenter.Show(!frameCenter.IsShow());
		
			/*Object @objFrame1 = cast<UIFrame>(PX2_PROJ().GetUIFrame().GetObjectByName("AFrame"));
			UIFrame @frame1 = cast<UIFrame>(objFrame1);
			if (frame1 !is null)
			{
				frame1.Show(!frame1.IsShow());
			}*/
		}
	}
}

void start()
{
	PX2_LOGGER().LogInfo("project_start", "start");
	
	Scene @scene = Scene();
	PX2_PROJ().SetScene(scene);	
	Actor @actor = PX2_CREATER().CreateActor_Box(scene, APoint(0.0f, 0.0f, 0.0f), true);
	InterpCurveRotateController @rotCtrl = InterpCurveRotateController();
	actor.AttachController(rotCtrl);
	rotCtrl.AddPoint(1.0f, Float3(3.1415926*2.0f, 0.0f, 0.0f), ICM_LINEAR);
	rotCtrl.Repeat = RT_WRAP;
	rotCtrl.MaxTime = 1.0f; 
	rotCtrl.ResetPlay();
		
	CameraActor @cameraActor = scene.GetUseCameraActor();
	cameraActor.LocalTransform.SetTranslateXZ(-6.0f, 5.0f);
	cameraActor.LookAt(APoint(0.0f, 0.0f, 0.0f));
	
	UIFrame @uiFrame = UIFrame();
	PX2_PROJ().SetUIFrame(uiFrame);
	uiFrame.SetAnchorHor(0.0f, 1.0f);
	uiFrame.SetAnchorVer(0.0f, 1.0f);
	
	UIButton @but = UIButton();
	but.SetAnchorHor(0.0f, 0.1f);
	but.SetAnchorVer(0.0f, 0.1f);
	uiFrame.AttachChild(but);
	but.SetScriptHandler("UICallback");
	
	@frameCenter = UIFrame();
	uiFrame.AttachChild(frameCenter);
	frameCenter.SetName("AFrame");
	frameCenter.SetAnchorHor(0.5f, 0.5f);
	frameCenter.SetAnchorVer(0.5f, 0.5f);
	frameCenter.SetSize(300.0f, 200.0f);
	frameCenter.CreateAddBackgroundPicBox(true);
	frameCenter.Show(false);
}