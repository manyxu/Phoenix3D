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
		
		string bufStr;
		if (PX2_RM().LoadBuffer("Data/ESPad/defs/shoepointpos.json", bufStr))
		{
			JSONData jd;
			if (jd.LoadBuffer(bufStr))
			{
				APoint pos;
			
				JSONValue jv0 = jd.GetMember("point0");
				string str0 = jv0.ToString();
				StringHelp().StringToXYZ(str0, pos.X(), pos.Y(), pos.Z());
				mButPos[0] = pos;
				
				JSONValue jv1 = jd.GetMember("point1");
				string str1 = jv1.ToString();
				StringHelp().StringToXYZ(str1, pos.X(), pos.Y(), pos.Z());
				mButPos[1] = pos;
				
				JSONValue jv2 = jd.GetMember("point2");
				string str2 = jv2.ToString();
				StringHelp().StringToXYZ(str2, pos.X(), pos.Y(), pos.Z());
				mButPos[2] = pos;
								
				JSONValue jv3 = jd.GetMember("point3");
				string str3 = jv3.ToString();
				StringHelp().StringToXYZ(str3, pos.X(), pos.Y(), pos.Z());
				mButPos[3] = pos;
								
				JSONValue jv4 = jd.GetMember("point4");
				string str4 = jv4.ToString();
				StringHelp().StringToXYZ(str4, pos.X(), pos.Y(), pos.Z());
				mButPos[4] = pos;
					
				JSONValue jv5 = jd.GetMember("point5");
				string str5 = jv5.ToString();
				StringHelp().StringToXYZ(str5, pos.X(), pos.Y(), pos.Z());
				mButPos[5] = pos;
			}
		}
		
		for (uint i=0; i<mNumButs; i++)
		{
			APoint pos = mButPos[i];
			float anchorHor = pos.X()/256.0f;
			float anchorVer = pos.Z()/480.0f;
			
			UIButton @butLeft = UIButton();
			leftFPic.AttachChild(butLeft);
			butLeft.SetName("LeftBut");
			butLeft.LocalTransform.SetTranslateY(-1.0f);
			butLeft.GetPicBoxAtState(BS_NORMAL).SetTexture("Data/ESPad/images/circle.png");
			butLeft.SetColor(Float3_RED);
			butLeft.SetAnchorHor(anchorHor, anchorHor);
			butLeft.SetAnchorVer(anchorVer, anchorVer);
			butLeft.SetSize(28.0f, 28.0f);
			@(mArrayButs_Left.opIndex(i)) = butLeft;
					
			UIButton @butRight = UIButton();
			rightFPic.AttachChild(butRight);
			butRight.SetName("RightBut");
			butRight.LocalTransform.SetTranslateY(-1.0f);
			butRight.GetPicBoxAtState(BS_NORMAL).SetTexture("Data/ESPad/images/circle.png");
			butRight.SetColor(Float3_RED);
			butRight.SetAnchorHor(1.0f-anchorHor, 1.0f-anchorHor);
			butRight.SetAnchorVer(anchorVer, anchorVer);
			butRight.SetSize(28.0f, 28.0f);
			@(mArrayButs_Right.opIndex(i)) = butRight;			
		}

		frameCenter.AttachChild(mGestureText);
		mGestureText.SetName("GestureText");
		mGestureText.SetAnchorHor(0.5f, 0.5f);
		mGestureText.SetAnchorVer(0.0f, 0.0f);
		mGestureText.SetSize(400.0f, 20.0f);
		mGestureText.SetPivot(0.5f, 0.0f);
		mGestureText.GetText().SetFont("Data/ESPad/fonts/PingFang.ttf", 20, 20, 0);
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