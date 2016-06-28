// PX2UICanvasController.cpp

#include "PX2UICanvasController.hpp"
#include "PX2InputEvent.hpp"
#include "PX2InputEventData.hpp"
#include "PX2UICanvas.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Controller, UICanvasController);
PX2_IMPLEMENT_STREAM(UICanvasController);
PX2_IMPLEMENT_FACTORY(UICanvasController);
PX2_IMPLEMENT_DEFAULT_NAMES(Controller, UICanvasController);

//----------------------------------------------------------------------------
UICanvasController::UICanvasController() :
mIsRightPressed(false),
mIsMiddlePressed(false)
{
}
//----------------------------------------------------------------------------
UICanvasController::~UICanvasController()
{
}
//----------------------------------------------------------------------------
void UICanvasController::DoExecute(Event *event)
{
	UICanvas *canvas = DynamicCast<UICanvas>(this->GetControlledable());
	if (!canvas->IsActivated())
		return;

	if (!canvas->IsEnable())
		return;

	if (InputEventSpace::IsEqual(event, InputEventSpace::MousePressed))
	{
		InputEventData ied = event->GetData<InputEventData>();
		if (MBID_RIGHT == ied.MButtonID)
		{
			mIsRightPressed = true;
			mRightPressedPos = ied.MTPos;
			mCurPos = ied.MTPos;
			mLastPos = mCurPos;
		}
		else if (MBID_MIDDLE == ied.MButtonID)
		{
			mIsMiddlePressed = true;
			mMiddlePressedPos = ied.MTPos;
			mCurPos = ied.MTPos;
			mLastPos = mCurPos;
		}
	}
	else if (InputEventSpace::IsEqual(event, InputEventSpace::MouseReleased))
	{
		InputEventData ied = event->GetData<InputEventData>();
		if (MBID_RIGHT == ied.MButtonID)
		{
			mIsRightPressed = false;
		}
		else if (MBID_MIDDLE == ied.MButtonID)
		{
			mIsMiddlePressed = false;
		}
	}
	else if (InputEventSpace::IsEqual(event, InputEventSpace::MouseMoved))
	{
		InputEventData ied = event->GetData<InputEventData>();
		if (mIsMiddlePressed)
		{
			mCurPos = ied.MTPos;
			AVector moveDir = mCurPos - mLastPos;
			mLastPos = mCurPos;

			//AVector vec = canvas->ScreenVectorToCamera(moveDir);
			//canvas->TransCameraNode(-vec);

			mRightPressedPos = ied.MTPos;
		}
	}
	else if (InputEventSpace::IsEqual(event, InputEventSpace::MouseWheeled))
	{
		InputEventData ied = event->GetData<InputEventData>();

		float scaleValue = ied.MWheel;
		canvas->ScaleCameraFrustum(scaleValue);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UICanvasController::UICanvasController(LoadConstructor value) :
Controller(value),
mIsRightPressed(false)
{
}
//----------------------------------------------------------------------------
void UICanvasController::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Controller::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UICanvasController, source);
}
//----------------------------------------------------------------------------
void UICanvasController::Link(InStream& source)
{
	Controller::Link(source);
}
//----------------------------------------------------------------------------
void UICanvasController::PostLink()
{
	Controller::PostLink();
}
//----------------------------------------------------------------------------
bool UICanvasController::Register(OutStream& target) const
{
	return Controller::Register(target);
}
//----------------------------------------------------------------------------
void UICanvasController::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Controller::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UICanvasController, target);
}
//----------------------------------------------------------------------------
int UICanvasController::GetStreamingSize(Stream &stream) const
{
	int size = Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
