// ESPadManager.cpp

#include "ESPadManager.hpp"
#include "PX2EventWorld.hpp"
#include "PX2StringHelp.hpp"
#include "PX2Project.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2UIFText.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2ServerInfoManager.hpp"
#include "PX2NetInitTerm.hpp"
#include "PX2UIButton.hpp"
#include "PX2UIText.hpp"
#include "PX2LanguageManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ESPadManager::ESPadManager()
{
}
//----------------------------------------------------------------------------
ESPadManager::~ESPadManager()
{
}
//----------------------------------------------------------------------------
void ESPadManager::Initlize()
{
	PX2_EW.ComeIn(this);

	std::string comStr;
	std::string baudrateStr;
	XMLData data;
	if (data.LoadFile("Data/ESPad/defs/config.xml"))
	{
		XMLNode node = data.GetNodeByPath("item");
		if (!node.IsNull())
		{
			comStr = node.AttributeToString("com");
			baudrateStr = node.AttributeToString("baudrate");
		}
	}
	int baudrate = StringHelp::StringToInt(baudrateStr);

	mAnalyzer = new FeetDataAnalyzer();
	
	if (!comStr.empty())
		mAnalyzer->Initlize(comStr, baudrate);

	PX2_LM.Add("Data/ESPad/defs/languageespad.csv");
}
//----------------------------------------------------------------------------
void ESPadManager::Update()
{
	UIFrame *frame = PX2_PROJ.GetUIFrame();
	if (!frame) return;

	FeetData fData;
	mAnalyzer->GetTopData(fData);

	UIFrame *leftFrame = DynamicCast<UIFrame>(
		frame->GetObjectByName("LeftFPic"));
	if (!leftFrame) return;

	UIFrame *rightFrame = DynamicCast<UIFrame>(
		frame->GetObjectByName("RightFPic"));
	if (!rightFrame) return;

	BodyPosture posture = mAnalyzer->Analyze();
	UIFText *gestureText = DynamicCast<UIFText>(frame->GetObjectByName("GestureText"));
	if (0 != gestureText)
	{
		std::string &strKey = FeetDataAnalyzer::msPostureStr[posture];
		std::string strLan = PX2_LM.V(strKey);
		gestureText->GetText()->SetText(strLan);
	}

	int butIndex = 0;
	int leftNumChild = leftFrame->GetNumChildren();
	for (int i = 0; i < leftNumChild; i++)
	{
		UIButton *but = DynamicCast<UIButton>(leftFrame->GetChild(i));
		if (but)
		{
			float alpha = (float)fData.left[butIndex] / 1024.0f;
			but->SetAlpha(alpha);

			butIndex++;
		}
	}

	butIndex = 0;
	int rightNumChild = rightFrame->GetNumChildren();
	for (int i = 0; i < rightNumChild; i++)
	{
		UIButton *but = DynamicCast<UIButton>(rightFrame->GetChild(i));
		if (but)
		{
			int rightButIndex = 0;
			if (0 == butIndex)
				rightButIndex = 0;
			else if (2 == butIndex)
				rightButIndex = 2;
			else if (1 == butIndex)
				rightButIndex = 3;
			else if (3 == butIndex)
				rightButIndex = 1;
			else if (4 == butIndex)
				rightButIndex = 5;
			else if (5 == butIndex)
				rightButIndex = 4;
			
			float alpha = (float)fData.right[rightButIndex] / 1024.0f;
			but->SetAlpha(alpha);

			butIndex++;
		}
	}

}
//----------------------------------------------------------------------------
void ESPadManager::Terminate()
{
	mAnalyzer->Ternimate();
	mAnalyzer = 0;

	PX2_EW.GoOut(this);

	PX2_PROJ.SetScene(0);
}
//----------------------------------------------------------------------------
void ESPadManager::OnEvent(Event *event)
{
	PX2_UNUSED(event);
}
//----------------------------------------------------------------------------