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
		}
	}
}

void start()
{
	PX2_LOGGER().LogInfo("script_as", "project_start");
	
	// scene
	Scene @scene = PX2_PROJ().GetScene();
	if (scene is null)
	{
		@scene = Scene();
		PX2_PROJ().SetScene(scene);	
	}
	
	Node @model = PX2_CREATER().CreateNode_Model("Data/ESPad/models/swk/swk.px2obj");
	scene.AttachChild(model);
	//model.LocalTransform.SetUniformScale(1.0f);
	model.LocalTransform.SetUniformScale(0.0008f);
	
	InterpCurveRotateController @rotCtrl = InterpCurveRotateController();
	model.AttachController(rotCtrl);
	rotCtrl.AddPoint(20.0f, Float3(0.0f, 0.0f, -3.1415926*2.0f), ICM_LINEAR);
	rotCtrl.MaxTime = 20.0f;
	rotCtrl.Repeat = RT_WRAP;
	rotCtrl.Play();
	
	CameraNode @mainCamNode = cast<CameraNode>(scene.GetObjectByName("MainCameraNode"));
	if (mainCamNode !is null)
	{
		mainCamNode.LocalTransform.SetTranslate(1.0f, -10.0f, 0.0f);
		mainCamNode.LookAt(APoint(0.0f, 0.0f, 0.0f));
	}
	
	
	Node @node = PX2_CREATER().CreateNode();
	scene.AttachChild(node);
	ScriptController @asCtrl = PX2_SC_AS().CreateScriptController("Data/ESPad/scripts/as/actorctrl.as", "ActorCtrl");
	node.AttachController(asCtrl);	
	
	// ui
	UIFrame @uiFrame = UIFrame();
	PX2_PROJ().SetUIFrame(uiFrame);
	uiFrame.SetAnchorHor(0.0f, 1.0f);
	uiFrame.SetAnchorVer(0.0f, 1.0f);
	uiFrame.EnableAnchorLayout(true);
	
	UIButton @but = UIButton();
	uiFrame.AttachChild(but);
	but.LocalTransform.SetTranslateY(-2.0f);
	but.SetAnchorHor(0.0f, 0.1f);
	but.SetAnchorVer(0.0f, 0.1f);
	but.SetScriptHandler("UICallback");
	
	@frameCenter = UIFrame();
	uiFrame.AttachChild(frameCenter);	
	frameCenter.SetWidget(true);
	frameCenter.SetName("FrameCenter");
	frameCenter.SetAnchorHor(0.5f, 0.5f);
	frameCenter.SetAnchorVer(0.5f, 0.5f);
	frameCenter.SetSize(520.0f, 480.0f);
	frameCenter.Show(false);
	frameCenter.CreateAddBackgroundPicBox(true);
	
	UIFrame @frameLeft = UIFrame();
	frameCenter.AttachChild(frameLeft);
	frameLeft.SetName("LeftFrame");
	frameLeft.LocalTransform.SetTranslateY(-1.0f);
	frameLeft.SetAnchorHor(0.0f, 0.5f);
	frameLeft.SetAnchorVer(0.0f, 1.0f);
	
	UIFrame @frameRight = UIFrame();
	frameCenter.AttachChild(frameRight);
	frameRight.SetName("RightFrame");
	frameRight.LocalTransform.SetTranslateY(-1.0f);
	frameRight.SetAnchorHor(0.5f, 1.0f);
	frameRight.SetAnchorVer(0.0f, 1.0f);
	
	UIFPicBox @picBoxLeft = UIFPicBox();
	frameLeft.AttachChild(picBoxLeft);
	picBoxLeft.SetName("LeftFPic");
	picBoxLeft.SetAnchorHor(0.5f, 0.5f);
	picBoxLeft.SetAnchorVer(0.5f, 0.5f);
	picBoxLeft.SetSize(256.0f, 480.0f);
	picBoxLeft.GetUIPicBox().SetTexture("Data/ESPad/images/jiaodianleft.png");
	
	UIFPicBox @picBoxRight = UIFPicBox();
	frameRight.AttachChild(picBoxRight);
	picBoxRight.SetName("RightFPic");
	picBoxRight.SetAnchorHor(0.5f, 0.5f);
	picBoxRight.SetAnchorVer(0.5f, 0.5f);
	picBoxRight.SetSize(256.0f, 480.0f);
	picBoxRight.GetUIPicBox().SetTexture("Data/ESPad/images/jiaodianright.png");
	
	// logo
	UIFPicBox @fPicBoxArduino = UIFPicBox();
	uiFrame.AttachChild(fPicBoxArduino);
	fPicBoxArduino.GetUIPicBox().SetTexture("Data/ESPad/images/arduino.png");
	fPicBoxArduino.SetPivot(1.0f, 1.0f);
	fPicBoxArduino.SetSize(80.0f, 54.0f);
	fPicBoxArduino.SetAnchorHor(1.0f, 1.0f);
	fPicBoxArduino.SetAnchorParamHor(0.0f, 0.0f);
	fPicBoxArduino.SetAnchorVer(1.0f, 1.0f);
	fPicBoxArduino.SetAnchorParamVer(0.0f, 0.0f);
	
	UIFPicBox @fPicBoxPhoenix3D = UIFPicBox();
	uiFrame.AttachChild(fPicBoxPhoenix3D);
	fPicBoxPhoenix3D.GetUIPicBox().SetTexture("Data/ESPad/images/phoenix.png");
	fPicBoxPhoenix3D.SetPivot(1.0f, 1.0f);
	fPicBoxPhoenix3D.SetSize(80.0f, 80.0f);
	fPicBoxPhoenix3D.SetAnchorHor(1.0f, 1.0f);
	fPicBoxPhoenix3D.SetAnchorParamHor(0.0f, 0.0f);
	fPicBoxPhoenix3D.SetAnchorVer(1.0f, 1.0f);
	fPicBoxPhoenix3D.SetAnchorParamVer(-54.0f, 0.0f);
	
	PX2_LOGGER().LogInfo("start", "frameCenter");
	
}