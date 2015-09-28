// PX2UISpBlockFrame.cpp

#include "PX2UIAuiBlockFrame.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UITabFrame.hpp"
#include "PX2UIAuiFrame.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIAuiBlockFrame);
PX2_IMPLEMENT_STREAM(UIAuiBlockFrame);
PX2_IMPLEMENT_FACTORY(UIAuiBlockFrame);

//----------------------------------------------------------------------------
UIAuiBlockFrame::UIAuiBlockFrame(UILayoutPosType posType) :
mPosType(posType),
mSideFrameHor0(0),
mSideFrameHor1(0),
mSideFrameVer0(0),
mSideFrameVer1(0),
mIsInnerHor0(false),
mIsInnerHor1(false),
mIsInnerVer0(false),
mIsInnerVer1(false),
mParentAuiBlockFrame(0),
mIsFramesSortNeedUpdate(true)
{
}
//----------------------------------------------------------------------------
UIAuiBlockFrame::~UIAuiBlockFrame()
{
}
//----------------------------------------------------------------------------
int UIAuiBlockFrame::AttachChild(Movable* child)
{
	int ret = UIFrame::AttachChild(child);

	UITabFrame *tabFrame = DynamicCast<UITabFrame>(child);
	if (tabFrame)
	{
		mUITabFrame = tabFrame;
		mUITabFrame->SetAuiBlockFrame(this);
	}

	UISplitterFrame *splitFrame = DynamicCast<UISplitterFrame>(child);
	if (splitFrame)
	{
		splitFrame->SetUpdatePriority(10.0f);

		mSplitterFrames.push_back(splitFrame);

		if (splitFrame->IsHor())
		{
			mSplitterFrames_Hor.push_back(splitFrame);
		}
		else
		{
			mSplitterFrames_Ver.push_back(splitFrame);
		}

		mIsFramesSortNeedUpdate = true;
	}

	return ret;
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::UpdateWorldData(double applicationTime, double elapsedTime)
{
	if (mIsFramesSortNeedUpdate)
	{
		_CalSideFrames(mSplitterFrames_Hor);
		_CalSideFrames(mSplitterFrames_Ver);

		mIsFramesSortNeedUpdate = false;
	}

	UIFrame::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
bool SplitterLessThan(UISplitterFrame *sp0, UISplitterFrame *sp1)
{
	UIFrame *parent = DynamicCast<UIFrame>(sp0->GetParent());

	AVector parentOffset = parent->LeftBottomCornerOffset;
	Sizef parentSize = parent->GetSize();
	parentSize = Sizef(100000.0f, 100000.0f);

	if (sp0->IsHor())
	{
		float pos0 = parentOffset.Z() + parentSize.Height * sp0->GetAnchorVer()[0] +
			sp0->GetAnchorParamVer()[0];
		float pos1 = parentOffset.Z() + parentSize.Height * sp1->GetAnchorVer()[0] +
			sp1->GetAnchorParamVer()[0];

		if (pos0 != pos1)
		{
			return pos0 < pos1;
		}
	}
	else
	{
		float pos0 = parentOffset.X() + parentSize.Width * sp0->GetAnchorHor()[0] +
			sp0->GetAnchorParamHor()[0];
		float pos1 = parentOffset.X() + parentSize.Width * sp1->GetAnchorHor()[0] +
			sp1->GetAnchorParamHor()[0];

		if (pos0 != pos1)
		{
			return pos0 < pos1;
		}
	}

	return sp0 < sp1;
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::_CalSideFrames(std::vector<UISplitterFramePtr> &frames)
{
	std::sort(frames.begin(), frames.end(), SplitterLessThan);

	int numFrames = (int)frames.size();
	for (int i = 0; i < numFrames; i++)
	{
		UISplitterFrame *frame0 = 0;
		UISplitterFrame *frame1 = 0;

		if (1 <= i) frame0 = frames[i - 1];
		if (i <= numFrames - 2) frame1 = frames[i + 1];

		//frames[i]->_SetSideFrame0(frame0);
		//frames[i]->_SetSideFrame1(frame1);
	}
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::SetSideFrameHor0(UISplitterFrame *frame, bool isInner)
{
	if (mSideFrameHor0)
	{
		frame->_RemoveAuiBlockFrame(this);
	}

	mSideFrameHor0 = frame;

	if (frame)
	{
		frame->_AddAuiBlockFrame(this);
	}
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::SetSideFrameHor1(UISplitterFrame *frame, bool isInner)
{
	if (mSideFrameHor1)
	{
		frame->_RemoveAuiBlockFrame(this);
	}

	mSideFrameHor1 = frame;

	if (frame)
	{
		frame->_AddAuiBlockFrame(this);
	}
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::SetSideFrameVer0(UISplitterFrame *frame, bool isInner)
{
	if (mSideFrameVer0)
	{
		frame->_RemoveAuiBlockFrame(this);
	}

	mSideFrameVer0 = frame;
	mIsInnerVer0 = isInner;

	if (frame)
	{
		frame->_AddAuiBlockFrame(this);
	}
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::SetSideFrameVer1(UISplitterFrame *frame, bool isInner)
{
	if (mSideFrameVer1)
	{
		frame->_RemoveAuiBlockFrame(this);
	}

	mSideFrameVer1 = frame;
	mIsInnerVer1 = isInner;

	if (frame)
	{
		frame->_AddAuiBlockFrame(this);
	}
}
//----------------------------------------------------------------------------
bool _IsSameParent(UISplitterFrame *frame0, UISplitterFrame *frame1)
{
	return frame0->GetParent() == frame1->GetParent();
}
//----------------------------------------------------------------------------
void _UpdateLayout(UIAuiBlockFrame *auiBlockFrame)
{
	UIFrame *parentFrame = DynamicCast<UIFrame>(auiBlockFrame->GetParent());
	if (!parentFrame) return;
	const AVector &plbOffset = parentFrame->LeftBottomCornerOffset;
	Transform parentWorldTransform = parentFrame->WorldTransform;
	const HMatrix& invMat = parentWorldTransform.Inverse();

	float spSize = PX2_UISM.Size_Splitter;

	UILayoutPosType posType = auiBlockFrame->GetLayOutPosType();
	UISplitterFrame *leftFrame = auiBlockFrame->GetSideFrameVer0();
	UISplitterFrame *rightFrame = auiBlockFrame->GetSideFrameVer1();
	UISplitterFrame *bottomFrame = auiBlockFrame->GetSideFrameHor0();
	UISplitterFrame *topFrame = auiBlockFrame->GetSideFrameHor1();

	const APoint &left = leftFrame->WorldTransform.GetTranslate();
	const APoint &right = rightFrame->WorldTransform.GetTranslate();
	const APoint &bottom = bottomFrame->WorldTransform.GetTranslate();
	const APoint &top = topFrame->WorldTransform.GetTranslate();

	float leftPos = left.X();
	float rightPos = rightFrame->WorldTransform.GetTranslate().X();
	float botPos = bottomFrame->WorldTransform.GetTranslate().Z();
	float topPos = topFrame->WorldTransform.GetTranslate().Z();

	float sizeWidth = (rightPos - spSize * auiBlockFrame->GetSideFrameVer1()->GetPvoit()[0]) -
		(leftPos + spSize * (1.0f - auiBlockFrame->GetSideFrameVer0()->GetPvoit()[0]));
	float sizeHeight = (topPos - spSize *  auiBlockFrame->GetSideFrameHor1()->GetPvoit()[1]) -
		(botPos + spSize * (1.0f - auiBlockFrame->GetSideFrameHor0()->GetPvoit()[1]));

	auiBlockFrame->SetSize(sizeWidth, sizeHeight);

	APoint localLeft = invMat * left;
	APoint localBot = invMat * bottom;

	float posX = localLeft.X() + spSize * (1.0f - leftFrame->GetPvoit()[0]) + sizeWidth / 2.0f;
	float posZ = localBot.Z() + spSize * (1.0f - bottomFrame->GetPvoit()[1]) + sizeHeight / 2.0f;

	auiBlockFrame->LocalTransform.SetTranslate(posX, -10.0f, posZ);

	auiBlockFrame->LeftBottomCornerOffset.X() = -sizeWidth / 2.0f;
	auiBlockFrame->LeftBottomCornerOffset.Z() = -sizeHeight / 2.0f;
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::UpdateLayout()
{
	_UpdateLayout(this);

	std::map<UILayoutPosType, Pointer0<UIAuiBlockFrame> >::iterator it = mSideFrames.begin();
	for (; it != mSideFrames.end(); it++)
	{
		UILayoutPosType posType = it->first;
		UIAuiBlockFrame *auiBlockFrame = it->second;

		_UpdateLayout(auiBlockFrame);
	}

	mIsLayoutChanged = false;
}
//----------------------------------------------------------------------------
UIAuiBlockFrame *UIAuiBlockFrame::_GetSideFrame(UILayoutPosType pos)
{
	std::map<UILayoutPosType, Pointer0<UIAuiBlockFrame> >::iterator it =
		mSideFrames.find(pos);

	if (it != mSideFrames.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
UISplitterFrame *UIAuiBlockFrame::_CalGetNewSideFrameHor0()
{
	UIAuiBlockFrame *bot = _GetSideFrame(UILPT_BOTTOM);
	if (bot)
	{
		return bot->GetSideFrameHor1();
	}

	return GetSideFrameHor0();
}
//----------------------------------------------------------------------------
UISplitterFrame *UIAuiBlockFrame::_CalGetNewSideFrameHor1()
{
	UIAuiBlockFrame *top = _GetSideFrame(UILPT_TOP);
	if (top)
	{
		return top->GetSideFrameHor0();
	}

	return GetSideFrameHor1();
}
//----------------------------------------------------------------------------
UISplitterFrame *UIAuiBlockFrame::_CalGetNewSideFrameVer0()
{
	UIAuiBlockFrame *left = _GetSideFrame(UILPT_LEFT);
	if (left)
	{
		return left->GetSideFrameVer1();
	}

	return GetSideFrameVer0();
}
//----------------------------------------------------------------------------
UISplitterFrame *UIAuiBlockFrame::_CalGetNewSideFrameVer1()
{
	UIAuiBlockFrame *right = _GetSideFrame(UILPT_RIGHT);
	if (right)
	{
		return right->GetSideFrameVer0();
	}

	return GetSideFrameVer1();
}
//----------------------------------------------------------------------------
UISplitterFrame *UIAuiBlockFrame::_CalGetLinkFrame0(UILayoutPosType pos)
{
	if (UILPT_LEFT == pos)
	{
		UIAuiBlockFrame *bottomFrame = _GetSideFrame(UILPT_BOTTOM);
		if (bottomFrame)
		{
			return bottomFrame->GetSideFrameHor1();
		}

		return GetSideFrameHor0();
	}
	else if (UILPT_RIGHT == pos)
	{
		UIAuiBlockFrame *botFrame = _GetSideFrame(UILPT_BOTTOM);
		if (botFrame)
		{
			return botFrame->GetSideFrameHor1();
		}

		return GetSideFrameHor0();
	}
	else if (UILPT_BOTTOM == pos)
	{
		UIAuiBlockFrame *leftFrame = _GetSideFrame(UILPT_LEFT);
		if (leftFrame)
		{
			return leftFrame->GetSideFrameVer1();
		}

		return GetSideFrameVer0();
	}
	else if (UILPT_TOP == pos)
	{
		UIAuiBlockFrame *leftFrame = _GetSideFrame(UILPT_LEFT);
		if (leftFrame)
		{
			return leftFrame->GetSideFrameVer1();
		}

		return GetSideFrameVer0();
	}

	return 0;
}
//----------------------------------------------------------------------------
UISplitterFrame *UIAuiBlockFrame::_CalGetLinkFrame1(UILayoutPosType pos)
{
	if (UILPT_LEFT == pos)
	{
		UIAuiBlockFrame *topFrame = _GetSideFrame(UILPT_TOP);
		if (topFrame)
		{
			return topFrame->GetSideFrameHor0();
		}

		return GetSideFrameHor1();
	}
	else if (UILPT_RIGHT == pos)
	{
		UIAuiBlockFrame *topFrame = _GetSideFrame(UILPT_TOP);
		if (topFrame)
		{
			return topFrame->GetSideFrameHor0();
		}

		return GetSideFrameHor1();
	}
	else if (UILPT_BOTTOM == pos)
	{
		UIAuiBlockFrame *rightFrame = _GetSideFrame(UILPT_RIGHT);
		if (rightFrame)
		{
			return rightFrame->GetSideFrameVer0();
		}

		return GetSideFrameVer1();
	}
	else if (UILPT_TOP == pos)
	{
		UIAuiBlockFrame *rightFrame = _GetSideFrame(UILPT_RIGHT);
		if (rightFrame)
		{
			return rightFrame->GetSideFrameVer0();
		}

		return GetSideFrameVer1();
	}

	return 0;
}
//----------------------------------------------------------------------------
UIAuiBlockFrame *UIAuiBlockFrame::CreateAddPosFrame(UILayoutPosType pos,
	const Sizef &size)
{
	UIAuiFrame *auiFrame = DynamicCast<UIAuiFrame>(GetParent());
	assertion(0 != auiFrame, "parent must be AuiFrame.\n");

	std::map<UILayoutPosType, UIAuiBlockFramePtr>::iterator it =
		mSideFrames.find(pos);
	if (it == mSideFrames.end())
	{
		UIAuiBlockFrame *auiBlockFrame = new0 UIAuiBlockFrame(pos);
		auiFrame->AttachChild(auiBlockFrame);
		auiBlockFrame->SetSize(size);
		auiBlockFrame->SetName("AuiBlockFrame");

		if (UILPT_LEFT == pos)
		{
			UISplitterFrame *newSplitterFrame = new0 UISplitterFrame(false);
			AttachChild(newSplitterFrame);
			newSplitterFrame->SetName("SpFrame_Left");

			newSplitterFrame->SetAnchorHor(0.0f, 0.0f);
			newSplitterFrame->SetAnchorParamHor(size.Width, 0.0f);
			newSplitterFrame->SetAnchorVer(0.0f, 1.0f);
			newSplitterFrame->SetAnchorParamVer(0.0f, 0.0f);

			newSplitterFrame->SetLinkFrame0(_CalGetLinkFrame0(pos));
			newSplitterFrame->SetLinkFrame1(_CalGetLinkFrame1(pos));

			auiBlockFrame->SetSideFrameVer0(_CalGetNewSideFrameVer0(), false);
			auiBlockFrame->SetSideFrameVer1(newSplitterFrame, false);
			auiBlockFrame->SetSideFrameHor0(_CalGetNewSideFrameHor0(), false);
			auiBlockFrame->SetSideFrameHor1(_CalGetNewSideFrameHor1(), false);
		}
		else if (UILPT_RIGHT == pos)
		{
			UISplitterFrame *newSplitterFrame = new0 UISplitterFrame(false);
			AttachChild(newSplitterFrame);
			newSplitterFrame->SetName("SpFrame_Right");

			newSplitterFrame->SetAnchorHor(1.0f, 1.0f);
			newSplitterFrame->SetAnchorParamHor(-size.Width, 0.0f);
			newSplitterFrame->SetAnchorVer(0.0f, 1.0f);
			newSplitterFrame->SetAnchorParamVer(0.0f, 0.0f);

			newSplitterFrame->SetLinkFrame0(_CalGetLinkFrame0(pos));
			newSplitterFrame->SetLinkFrame1(_CalGetLinkFrame1(pos));

			auiBlockFrame->SetSideFrameVer0(newSplitterFrame, false);
			auiBlockFrame->SetSideFrameVer1(_CalGetNewSideFrameVer1(), false);
			auiBlockFrame->SetSideFrameHor0(_CalGetNewSideFrameHor0(), false);
			auiBlockFrame->SetSideFrameHor1(_CalGetNewSideFrameHor1(), false);
		}
		else if (UILPT_BOTTOM == pos)
		{
			UISplitterFrame *newSplitterFrame = new0 UISplitterFrame(true);
			AttachChild(newSplitterFrame);
			newSplitterFrame->SetName("SpFrame_Bottom");

			newSplitterFrame->SetAnchorHor(0.0f, 1.0f);
			newSplitterFrame->SetAnchorParamHor(0.0f, 0.0f);
			newSplitterFrame->SetAnchorVer(0.0f, 0.0f);
			newSplitterFrame->SetAnchorParamVer(size.Height, 0.0f);

			newSplitterFrame->SetLinkFrame0(_CalGetLinkFrame0(pos));
			newSplitterFrame->SetLinkFrame1(_CalGetLinkFrame1(pos));

			auiBlockFrame->SetSideFrameVer0(_CalGetNewSideFrameVer0(), false);
			auiBlockFrame->SetSideFrameVer1(_CalGetNewSideFrameVer1(), false);
			auiBlockFrame->SetSideFrameHor0(_CalGetNewSideFrameHor0(), false);
			auiBlockFrame->SetSideFrameHor1(newSplitterFrame, false);
		}
		else if (UILPT_TOP == pos)
		{
			UISplitterFrame *newSplitterFrame = new0 UISplitterFrame(true);
			AttachChild(newSplitterFrame);
			newSplitterFrame->SetName("SpFrame_Top");

			newSplitterFrame->SetAnchorHor(0.0f, 1.0f);
			newSplitterFrame->SetAnchorParamHor(0.0f, 0.0f);
			newSplitterFrame->SetAnchorVer(1.0f, 1.0f);
			newSplitterFrame->SetAnchorParamVer(-size.Height, 0.0f);

			newSplitterFrame->SetLinkFrame0(_CalGetLinkFrame0(pos));
			newSplitterFrame->SetLinkFrame1(_CalGetLinkFrame1(pos));

			auiBlockFrame->SetSideFrameVer0(_CalGetNewSideFrameVer0(), false);
			auiBlockFrame->SetSideFrameVer1(_CalGetNewSideFrameVer1(), false);
			auiBlockFrame->SetSideFrameHor0(newSplitterFrame, false);
			auiBlockFrame->SetSideFrameHor1(_CalGetNewSideFrameHor1(), false);
		}

		mSideFrames[pos] = auiBlockFrame;
		auiBlockFrame->SetParentAuiBlockFrame(this);

		return auiBlockFrame;
	}
	else
	{
		UIAuiBlockFrame *existFrame = it->second;
		existFrame->CreateAddPosFrame(pos, size);
	}

	return 0;
}
//----------------------------------------------------------------------------
UIAuiBlockFrame *UIAuiBlockFrame::GetPosFrame(UILayoutPosType pos)
{
	std::map<UILayoutPosType, Pointer0<UIAuiBlockFrame> >::iterator it =
		mSideFrames.find(pos);

	if (it != mSideFrames.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::SetParentAuiBlockFrame(UIAuiBlockFrame *frame)
{
	mParentAuiBlockFrame = frame;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIAuiBlockFrame::UIAuiBlockFrame(LoadConstructor value) :
UIFrame(value),
mSideFrameHor0(0),
mSideFrameHor1(0),
mSideFrameVer0(0),
mSideFrameVer1(0),
mIsInnerHor0(false),
mIsInnerHor1(false),
mIsInnerVer0(false),
mIsInnerVer1(false),
mParentAuiBlockFrame(0),
mIsFramesSortNeedUpdate(true)
{
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIAuiBlockFrame, source);
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::Link(InStream& source)
{
	UIFrame::Link(source);

	if (mIPTCtrl)
		source.ResolveLink(mIPTCtrl);
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIAuiBlockFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIAuiBlockFrame, target);
}
//----------------------------------------------------------------------------
int UIAuiBlockFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------