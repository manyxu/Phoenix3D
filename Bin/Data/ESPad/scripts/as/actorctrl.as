// actorctrl.as

class ActorCtrl : IScript
{
	ActorCtrl(ScriptController @ctrl)
	{
		@mScriptCtrl = ctrl;
	}
	
	// attach detach
	void OnAttached()
	{
	}
	
	void OnDetach()
	{
		@mScriptCtrl = null;
		
		mArrayButs_Left.Clear();
		mArrayButs_Right.Clear();
	}
	
	// update
	void OnInitUpdate()
	{	
		UIFrame @projUIFrame = PX2_PROJ().GetUIFrame();
		UIFrame @frameCenter = cast<UIFrame>(projUIFrame.GetObjectByName("FrameCenter"));
		UIFPicBox @leftFPic = cast<UIFPicBox>(projUIFrame.GetObjectByName("LeftFPic"));
		UIFPicBox @rightFPic = cast<UIFPicBox>(projUIFrame.GetObjectByName("RightFPic"));
	}
	
	void OnUpdate()
	{
	}
	
	void OnFixUpdate()
	{
	}
	
	// play
	void OnPResetPlay()
	{
	}
	
	void OnPReset()
	{
	}
	
	void OnPPlay()
	{
	}
	
	void OnPStop()
	{
	}
	
	void OnPUpdate()
	{
	}
	
	uint mNumButs = 6;
	ScriptController @mScriptCtrl;
	Array<APoint> mButPos(mNumButs);
	Array<UIButton@> mArrayButs_Left(mNumButs);
	Array<UIButton@> mArrayButs_Right(mNumButs);
	UIFText mGestureText;
}