// PX2UIFrameGridAlignControl.cpp

#include "PX2UIFrameGridAlignControl.hpp"
#include "PX2UIFrame.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Controller, UIFrameGridAlignControl);
PX2_IMPLEMENT_STREAM(UIFrameGridAlignControl);
PX2_IMPLEMENT_FACTORY(UIFrameGridAlignControl);

//----------------------------------------------------------------------------
UIFrameGridAlignControl::UIFrameGridAlignControl() :
mIsLayoutChanged(true)
{
	mCellSize = Sizef(100.0f, 50.0f);
	mSpacing = Float2(0.0f, 0.0f);
	mStartCornerType = SCT_UPPER_LEFT;
	mStartAxisType = SAT_HORIZONTAL;
	mChildAlignmentType = CAT_UPPER_LEFT;
	mConstraintType = CT_FLEXIBLE;
}
//----------------------------------------------------------------------------
UIFrameGridAlignControl::~UIFrameGridAlignControl()
{
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::SetCellSize(const Sizef &cellSize)
{
	mCellSize = cellSize;
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::SetSpacing(const Float2 &spacing)
{
	mSpacing = spacing;
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::SetStartCorner(
	UIFrameGridAlignControl::StartCornerType sc)
{
	mStartCornerType = sc;
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::SetStartAxis(
	UIFrameGridAlignControl::StartAxisType sa)
{
	mStartAxisType = sa;
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::SetChildAlignment(
	UIFrameGridAlignControl::ChildAlignmentType ca)
{
	mChildAlignmentType = ca;
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::SetConstraintType(
	UIFrameGridAlignControl::ConstraintType type)
{
	mConstraintType = type;
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::MarkRelatvieChange()
{
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::_Update(double applicationTime,
	double elapsedTime)
{
	Controller::_Update(applicationTime, elapsedTime);
	
	UIFrame *parent = DynamicCast<UIFrame>(GetControlledable());
	if (!parent) return;

	if (mIsLayoutChanged)
	{
		_UpdateAlignItems();

		mIsLayoutChanged = false;
	}
}
//----------------------------------------------------------------------------
int UIFrameGridAlignControl::_UpdateAlignItems()
{
	UIFrame *parent = DynamicCast<UIFrame>(GetControlledable());
	if (!parent) return 0;

	std::vector<UIFrame*> frames;
	for (int i=0; i<parent->GetNumChildren(); i++)
	{
		UIFrame *uiFrame = DynamicCast<UIFrame>(parent->GetChild(i));
		if (uiFrame)
		{
			frames.push_back(uiFrame);
			uiFrame->SetPvoit(0.5f, 0.5f);
		}
	}

	_UpdateItems(parent, frames, mStartCornerType, mStartAxisType,
		mChildAlignmentType);

	return 0;
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::_UpdateItems(UIFrame *parent, 
										   const std::vector<UIFrame*> &frames,
										   StartCornerType startCornerType,
										   StartAxisType startAxisType,
										   ChildAlignmentType childAlignmentTypeType)
{
	if (0 == (int)frames.size())
		return;

	const Sizef &size = parent->GetSize();
	const Sizef &borderSize = parent->GetBorderSize();
	const Sizef contentSize = Sizef(size.Width-borderSize.Width*2.0f, 
		size.Height-borderSize.Height*2.0f);
	const AVector &leftBottomCornerOffset = parent->LeftBottomCornerOffset;

	for (int i=0; i<(int)frames.size(); i++)
	{
		UIFrame *frame = frames[i];

		frame->SetSize(mCellSize);

		if (SCT_UPPER_LEFT == startCornerType)
		{
			frame->SetAnchorHor(Float2(0.0f, 0.0f));
			frame->SetAnchorVer(Float2(1.0f, 1.0f));
		}
		else if (SCT_UPPER_RIGHT == startCornerType)
		{
			frame->SetAnchorHor(Float2(1.0f, 1.0f));
			frame->SetAnchorVer(Float2(1.0f, 1.0f));
		}
		else if (SCT_LOWER_LEFT == startCornerType)
		{
			frame->SetAnchorHor(Float2(0.0f, 0.0f));
			frame->SetAnchorVer(Float2(0.0f, 0.0f));
		}
		else if (SCT_LOWER_RIGHT == startCornerType)
		{
			frame->SetAnchorHor(Float2(1.0f, 1.0f));
			frame->SetAnchorVer(Float2(0.0f, 0.0f));
		}

		int numWidth = 0;
		int numHeight = 0;
		if (SAT_HORIZONTAL == startAxisType)
		{
			numWidth = (int)((contentSize.Width+mSpacing[0]) / mCellSize.Width);

			int xIndex = 0;
			int zIndex = 0;
			Float2 horParam;
			Float2 verParam;
			if (CAT_UPPER_LEFT == childAlignmentTypeType)
			{
				xIndex = i % numWidth;
				zIndex = i / numWidth;

				horParam[0] = leftBottomCornerOffset.X() + xIndex * (mCellSize.Width+mSpacing[0]);
				horParam[1] = leftBottomCornerOffset.Z() + size.Height -borderSize.Height -zIndex * (mCellSize.Height+mSpacing[1]);
				frame->SetAnchorParamHor(horParam);
			}
			else if (CAT_MIDDLE_LEFT == mChildAlignmentType)
			{
				//xIndex = i % numWidth;
				//zIndex = i / numWidth;

				//horParam[0] = leftBottomCornerOffset.X() + xIndex * (mCellSize.Width+mSpacing[0]);
				//horParam[1] = leftBottomCornerOffset.Z() + size.Height -borderSize.Height -zIndex * (mCellSize.Height+mSpacing[1]);
				//frame->SetAnchorParamHor(horParam);
			}
		}
		else if (SAT_VERTICAL == startAxisType)
		{
			numHeight =(int)((contentSize.Height+mSpacing[1])/mCellSize.Height);

			int xIndex = i / numHeight;
			int zIndex = i % numHeight;
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIFrameGridAlignControl::UIFrameGridAlignControl(LoadConstructor value) :
Controller(value),
mIsLayoutChanged(true)
{
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Controller::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(Controller, source);
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::Link(InStream& source)
{
	Controller::Link(source);
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::PostLink()
{
	Controller::PostLink();

	RegistToScriptSystemAll();
}
//----------------------------------------------------------------------------
bool UIFrameGridAlignControl::Register(OutStream& target) const
{
	if (Controller::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Controller::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(Controller, target);
}
//----------------------------------------------------------------------------
int UIFrameGridAlignControl::GetStreamingSize(Stream &stream) const
{
	int size = Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------