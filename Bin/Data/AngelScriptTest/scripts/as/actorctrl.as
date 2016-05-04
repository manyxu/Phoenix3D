// actorctrl.as

class ActorCtrl : IScript
{
	ActorCtrl(ScriptController @ctrl)
	{
		@mScriptCtrl = ctrl;

		UIFrame @frame = PX2_PROJ().GetUIFrame();
		
		if (frame !is null)
		{
			UIPicBox @picBox = UIPicBox("Data/engine/default.png", 1);
			frame.AttachChild(picBox);
			picBox.SetSize(200.0f, 200.0f);
		}
	}

	void OnStart()
	{
	}
	
	void OnUpdate()
	{
	}
	
	ScriptController @mScriptCtrl;
}