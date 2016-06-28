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

	PX2_LM.Add("Data/ESPad/defs/languageespad.csv");
}
//----------------------------------------------------------------------------
void ESPadManager::Update()
{
	UIFrame *frame = PX2_PROJ.GetUIFrame();
	if (!frame) return;
}
//----------------------------------------------------------------------------
void ESPadManager::Terminate()
{
	PX2_EW.GoOut(this);

	PX2_PROJ.SetScene(0);
}
//----------------------------------------------------------------------------
void ESPadManager::DoExecute(Event *event)
{
	PX2_UNUSED(event);
}
//----------------------------------------------------------------------------