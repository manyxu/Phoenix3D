// PX2EU_ConsoleFrame.cpp

#include "PX2EU_ConsoleFrame.hpp"
#include "PX2UISkinManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ConsoleLogHandler::ConsoleLogHandler():
LogHandler(LT_INFO | LT_ERROR | LT_USER)
{
}
//----------------------------------------------------------------------------
ConsoleLogHandler::~ConsoleLogHandler()
{
}
//----------------------------------------------------------------------------
void ConsoleLogHandler::Handle(const LogBuffer *logBuffer,
	const char *timeStamp)
{
	EU_ConcoleFrame *euFrame = EU_ConcoleFrame::GetSingletonPtr();
	if (euFrame)
	{
		euFrame->Handle(logBuffer, timeStamp);
	}
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_ConcoleFrame);
PX2_IMPLEMENT_STREAM(EU_ConcoleFrame);
PX2_IMPLEMENT_FACTORY(EU_ConcoleFrame);
//----------------------------------------------------------------------------
EU_ConcoleFrame::EU_ConcoleFrame()
{
	mToolFrame = new0 UIFrame();
	AttachChild(mToolFrame);
	mToolFrame->LocalTransform.SetTranslateY(-1.0f);
	UIPicBox *picBox = mToolFrame->CreateAddBackgroundPicBox();
	picBox->SetColor(PX2_UISM.Color_ToolBar);
	mToolFrame->SetAnchorHor(0.0f, 1.0f);
	mToolFrame->SetAnchorVer(1.0f, 1.0f);
	mToolFrame->SetPivot(0.5f, 1.0f);
	mToolFrame->SetSize(0.0f, PX2_UISM.Size_ToolBar);

	mEditBox = new0 UIEditBox();
	AttachChild(mEditBox);
	mEditBox->LocalTransform.SetTranslateY(-5.0f);
	mEditBox->SetAnchorHor(0.0f, 1.0f);
	mEditBox->SetAnchorVer(1.0f, 1.0f);
	mEditBox->SetAnchorParamVer(-PX2_UISM.Size_ToolBar, 0.0f);
	mEditBox->SetPivot(0.5f, 1.0f);
	mEditBox->SetSize(0.0f, PX2_UISM.Size_SearchBar);

	mList = new0 UIList();
	AttachChild(mList);
	mList->SetAnchorHor(0.0f, 1.0f);
	mList->SetAnchorVer(0.0f, 1.0f);
	mList->SetAnchorParamVer(0.0f, -PX2_UISM.Size_ToolBar - PX2_UISM.Size_SearchBar);

	mLogHandler = new0 ConsoleLogHandler();
	Logger::GetSingleton().AddHandler(mLogHandler);
}
//----------------------------------------------------------------------------
EU_ConcoleFrame::~EU_ConcoleFrame()
{
	Logger::GetSingleton().RemoveHandler(mLogHandler);
}
//----------------------------------------------------------------------------
void EU_ConcoleFrame::Handle(const LogBuffer *logBuffer,
	const char *timeStamp)
{
	std::string timeStampStr(timeStamp);
	std::string logStr = timeStampStr + std::string((char*)logBuffer->Buffer);

	int level = logBuffer->Level;

	Float3 color;
	if (PX2::LT_INFO == level)
	{
		color = Float3(240, 240, 240);
	}
	else if (PX2::LT_ERROR == level)
	{
		color = Float3::RED;
	}
	else if (PX2::LT_USER == level)
	{
		color = Float3::GREEN;
	}

	UIItem *item = mList->AddItem(logStr);
	item->GetFText()->GetText()->SetFontColor(color);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_ConcoleFrame::EU_ConcoleFrame(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_ConcoleFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_ConcoleFrame, source);
}
//----------------------------------------------------------------------------
void EU_ConcoleFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_ConcoleFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_ConcoleFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_ConcoleFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_ConcoleFrame, target);
}
//----------------------------------------------------------------------------
int EU_ConcoleFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------