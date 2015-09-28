// PX2UISpBlockFrame.cpp

#include "PX2UISpBlockFrame.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UITabFrame.hpp"
#include "PX2UIAuiFrame.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UISpBlockFrame);
PX2_IMPLEMENT_STREAM(UISpBlockFrame);
PX2_IMPLEMENT_FACTORY(UISpBlockFrame);

//----------------------------------------------------------------------------
UISpBlockFrame::UISpBlockFrame(UILayOutPos posType) :
mPosType(posType),
mSideFrameHor0(0),
mSideFrameHor1(0),
mSideFrameVer0(0),
mSideFrameVer1(0),
mParentSpBlockFrame(0)
{
}
//----------------------------------------------------------------------------
UISpBlockFrame::~UISpBlockFrame()
{
}
//----------------------------------------------------------------------------
int UISpBlockFrame::AttachChild(Movable* child)
{
	int ret = UIFrame::AttachChild(child);

	UITabFrame *tabFrame = DynamicCast<UITabFrame>(child);
	if (tabFrame)
	{
		mUITabFrame = tabFrame;
		mUITabFrame->SetSpBlockFrame(this);
	}

	return ret;
}
//----------------------------------------------------------------------------
void UISpBlockFrame::SetSideFrameHor0(UISplitterFrame *frame)
{
	if (mSideFrameHor0)
	{
		frame->_RemoveSpBlockFrame(this);
	}

	mSideFrameHor0 = frame;

	if (frame)
	{
		frame->_AddSpBlockFrame(this);
	}
}
//----------------------------------------------------------------------------
void UISpBlockFrame::SetSideFrameHor1(UISplitterFrame *frame)
{
	if (mSideFrameHor1)
	{
		frame->_RemoveSpBlockFrame(this);
	}

	mSideFrameHor1 = frame;

	if (frame)
	{
		frame->_AddSpBlockFrame(this);
	}
}
//----------------------------------------------------------------------------
void UISpBlockFrame::SetSideFrameVer0(UISplitterFrame *frame)
{
	if (mSideFrameVer0)
	{
		frame->_RemoveSpBlockFrame(this);
	}

	mSideFrameVer0 = frame;

	if (frame)
	{
		frame->_AddSpBlockFrame(this);
	}
}
//----------------------------------------------------------------------------
void UISpBlockFrame::SetSideFrameVer1(UISplitterFrame *frame)
{
	if (mSideFrameVer1)
	{
		frame->_RemoveSpBlockFrame(this);
	}

	mSideFrameVer1 = frame;

	if (frame)
	{
		frame->_AddSpBlockFrame(this);
	}
}
//----------------------------------------------------------------------------
void UISpBlockFrame::UpdateLayout()
{
	if (mSideFrameHor0 && mSideFrameHor1 && mSideFrameVer0 && mSideFrameVer1)
	{
		UIFrame *parentFrame = DynamicCast<UIFrame>(GetParent());
		float spSize = PX2_UISM.Size_Splitter;

		float leftPos = mSideFrameVer0->LocalTransform.GetTranslate().X();
		float rightPos = mSideFrameVer1->LocalTransform.GetTranslate().X();

		float botPos = mSideFrameHor0->LocalTransform.GetTranslate().Z();
		float topPos = mSideFrameHor1->LocalTransform.GetTranslate().Z();

		float sizeWidth = (rightPos - spSize * mSideFrameVer1->GetPvoit()[0]) -
			(leftPos + spSize * (1.0f-mSideFrameVer0->GetPvoit()[0]));
		float sizeHeight = (topPos - spSize * mSideFrameHor1->GetPvoit()[1]) -
			(botPos + spSize * (1.0f-mSideFrameHor0->GetPvoit()[1]));

		SetSize(sizeWidth, sizeHeight);

		float posX = leftPos + spSize * (1.0f-mSideFrameVer0->GetPvoit()[0]) + mSize.Width/2.0f;
		float posZ = botPos + spSize * (1.0f-mSideFrameHor0->GetPvoit()[1]) + mSize.Height/2.0f;

		LocalTransform.SetTranslateXZ(posX, posZ);

		LeftBottomCornerOffset.X() = -sizeWidth/2.0f;
		LeftBottomCornerOffset.Z() = -sizeHeight/2.0f;
	}

	mIsLayoutChanged = false;
}
//----------------------------------------------------------------------------
UISpBlockFrame *UISpBlockFrame::CreateAddSideFrame(UILayOutPos pos, 
												   const Sizef &size)
{
	UIAuiFrame *auiFrame = DynamicCast<UIAuiFrame>(GetParent());
	assertion(0!=auiFrame, "parent must be AuiFrame.\n");

	std::map<UILayOutPos, UISpBlockFramePtr>::iterator it = 
		mSideFrames.find(pos);
	if (it == mSideFrames.end())
	{
		UISpBlockFrame *spBlockFrame = new0 UISpBlockFrame(pos);
		auiFrame->AttachChild(spBlockFrame);
		spBlockFrame->SetSize(size);
		spBlockFrame->SetName("SpBlockFrame");

		mSideFrames[pos] = spBlockFrame;
		spBlockFrame->SetParentSpBlockFrame(this);

		spBlockFrame->CreateAddBackgroundPicBox();

		if (UILOP_LEFT == pos)
		{
			UISplitterFrame *newSplitterFrame = new0 UISplitterFrame(false);
			auiFrame->AttachChild(newSplitterFrame);
			newSplitterFrame->SetName("SpFrame_Left");

			newSplitterFrame->SetAnchorHor(0.0f, 0.0f);
			newSplitterFrame->SetAnchorParamHor(size.Width, 0.0f);

			newSplitterFrame->SetLinkFrame0(_CalGetLinkFrame0(pos));
			newSplitterFrame->SetLinkFrame1(_CalGetLinkFrame1(pos));

			spBlockFrame->SetSideFrameVer0(GetSideFrameVer0());
			spBlockFrame->SetSideFrameVer1(newSplitterFrame);
			spBlockFrame->SetSideFrameHor0(GetSideFrameHor0());
			spBlockFrame->SetSideFrameHor1(GetSideFrameHor1());

			SetSideFrameVer0(newSplitterFrame);
		}
		else if (UILOP_RIGHT == pos)
		{
			UISplitterFrame *newSplitterFrame = new0 UISplitterFrame(false);
			auiFrame->AttachChild(newSplitterFrame);
			newSplitterFrame->SetName("SpFrame_Right");

			newSplitterFrame->SetAnchorHor(1.0f, 1.0f);
			newSplitterFrame->SetAnchorParamHor(-size.Width, 0.0f);

			newSplitterFrame->SetLinkFrame0(_CalGetLinkFrame0(pos));
			newSplitterFrame->SetLinkFrame1(_CalGetLinkFrame1(pos));

			spBlockFrame->SetSideFrameVer0(newSplitterFrame);
			spBlockFrame->SetSideFrameVer1(GetSideFrameVer1());
			spBlockFrame->SetSideFrameHor0(GetSideFrameHor0());
			spBlockFrame->SetSideFrameHor1(GetSideFrameHor1());

			SetSideFrameVer1(newSplitterFrame);
		}
		else if (UILOP_BOTTOM == pos)
		{
			UISplitterFrame *newSplitterFrame = new0 UISplitterFrame(true);
			auiFrame->AttachChild(newSplitterFrame);
			newSplitterFrame->SetName("SpFrame_Bottom");

			newSplitterFrame->SetAnchorVer(0.0f, 0.0f);
			newSplitterFrame->SetAnchorParamVer(size.Height, 0.0f);

			newSplitterFrame->SetLinkFrame0(_CalGetLinkFrame0(pos));
			newSplitterFrame->SetLinkFrame1(_CalGetLinkFrame1(pos));

			spBlockFrame->SetSideFrameVer0(GetSideFrameVer0());
			spBlockFrame->SetSideFrameVer1(GetSideFrameVer1());
			spBlockFrame->SetSideFrameHor0(GetSideFrameHor0());
			spBlockFrame->SetSideFrameHor1(newSplitterFrame);

			SetSideFrameHor0(newSplitterFrame);
		}
		else if (UILOP_TOP == pos)
		{
			UISplitterFrame *newSplitterFrame = new0 UISplitterFrame(true);
			auiFrame->AttachChild(newSplitterFrame);
			newSplitterFrame->SetName("SpFrame_Top");

			newSplitterFrame->SetAnchorVer(1.0f, 1.0f);
			newSplitterFrame->SetAnchorParamVer(-size.Height, 0.0f);

			newSplitterFrame->SetLinkFrame0(_CalGetLinkFrame0(pos));
			newSplitterFrame->SetLinkFrame1(_CalGetLinkFrame1(pos));

			spBlockFrame->SetSideFrameVer0(GetSideFrameVer0());
			spBlockFrame->SetSideFrameVer1(GetSideFrameVer1());
			spBlockFrame->SetSideFrameHor0(newSplitterFrame);
			spBlockFrame->SetSideFrameHor1(GetSideFrameHor1());

			SetSideFrameHor1(newSplitterFrame);
		}

		return spBlockFrame;
	}
	else
	{
		UISpBlockFrame *originFrame = it->second;
		originFrame->CreateAddSideFrame(pos, size);
	}

	return 0;
}
//----------------------------------------------------------------------------
UISpBlockFrame *UISpBlockFrame::GetSideFrame(UILayOutPos pos)
{
	std::map<UILayOutPos, Pointer0<UISpBlockFrame> >::iterator it =
		mSideFrames.find(pos);

	if (it != mSideFrames.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
void UISpBlockFrame::SetParentSpBlockFrame(UISpBlockFrame *frame)
{
	mParentSpBlockFrame = frame;
}
//----------------------------------------------------------------------------
UISpBlockFrame *UISpBlockFrame::_GetTopestPosFrame(UILayOutPos pos)
{
	UISpBlockFrame *parentSpBlockFrame = GetParentSpBlockFrame();
	while (parentSpBlockFrame)
	{
		UISpBlockFrame *posFrame = parentSpBlockFrame->GetSideFrame(pos);
		if (posFrame)
		{
			return posFrame;
		}
		else
		{
			parentSpBlockFrame = parentSpBlockFrame->GetParentSpBlockFrame();
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
UISplitterFrame *UISpBlockFrame::_CalGetLinkFrame0(UILayOutPos pos)
{
	if (UILOP_LEFT == pos)
	{
		UISpBlockFrame *bottomFrame = GetSideFrame(UILOP_BOTTOM);
		if (bottomFrame)
		{
			return bottomFrame->GetSideFrameHor1();
		}
		else
		{
			UISpBlockFrame *topestFrame = _GetTopestPosFrame(UILOP_BOTTOM);
			if (topestFrame)
			{
				return topestFrame->GetSideFrameHor1();
			}
			else
			{
				return GetSideFrameVer0()->GetLinkFrame0();
			}
		}
	}
	else if (UILOP_RIGHT == pos)
	{
		UISpBlockFrame *botFrame = GetSideFrame(UILOP_BOTTOM);
		if (botFrame)
		{
			return botFrame->GetSideFrameHor1();
		}
		else
		{
			UISpBlockFrame *topestFrame = _GetTopestPosFrame(UILOP_BOTTOM);
			if (topestFrame)
			{
				return topestFrame->GetSideFrameHor1();
			}
			else
			{
				return GetSideFrameVer1()->GetLinkFrame0();
			}
		}
	}
	else if (UILOP_BOTTOM == pos)
	{
		UISpBlockFrame *leftFrame = GetSideFrame(UILOP_LEFT);
		if (leftFrame)
		{
			return leftFrame->GetSideFrameVer1();
		}
		else
		{
			UISpBlockFrame *topestFrame = _GetTopestPosFrame(UILOP_LEFT);
			if (topestFrame)
			{
				return topestFrame->GetSideFrameVer1();
			}
			else
			{
				return GetSideFrameHor0()->GetLinkFrame0();
			}
		}
	}
	else if (UILOP_TOP == pos)
	{
		UISpBlockFrame *leftFrame = GetSideFrame(UILOP_LEFT);
		if (leftFrame)
		{
			return leftFrame->GetSideFrameVer1();
		}
		else
		{
			UISpBlockFrame *topestFrame = _GetTopestPosFrame(UILOP_LEFT);
			if (topestFrame)
			{
				return topestFrame->GetSideFrameVer1();
			}
			else
			{
				return GetSideFrameHor1()->GetLinkFrame0();
			}
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
UISplitterFrame *UISpBlockFrame::_CalGetLinkFrame1(UILayOutPos pos)
{
	if (UILOP_LEFT == pos)
	{
		UISpBlockFrame *topFrame = GetSideFrame(UILOP_TOP);
		if (topFrame)
		{
			return topFrame->GetSideFrameHor0();
		}
		else
		{
			UISpBlockFrame *topestFrame = _GetTopestPosFrame(UILOP_TOP);
			if (topestFrame)
			{
				return topestFrame->GetSideFrameHor0();
			}
			else
			{
				return GetSideFrameVer0()->GetLinkFrame1();
			}
		}
	}
	else if (UILOP_RIGHT == pos)
	{
		UISpBlockFrame *topFrame = GetSideFrame(UILOP_TOP);
		if (topFrame)
		{
			return topFrame->GetSideFrameHor0();
		}
		else
		{
			UISpBlockFrame *topestFrame = _GetTopestPosFrame(UILOP_TOP);
			if (topestFrame)
			{
				return topestFrame->GetSideFrameHor0();
			}
			else
			{
				return GetSideFrameVer1()->GetLinkFrame1();
			}
		}
	}
	else if (UILOP_BOTTOM == pos)
	{
		UISpBlockFrame *rightFrame = GetSideFrame(UILOP_RIGHT);
		if (rightFrame)
		{
			return rightFrame->GetSideFrameVer0();
		}
		else
		{
			UISpBlockFrame *topestFrame = _GetTopestPosFrame(UILOP_RIGHT);
			if (topestFrame)
			{
				return topestFrame->GetSideFrameVer0();
			}
			else
			{
				return GetSideFrameHor0()->GetLinkFrame1();
			}
		}
	}
	else if (UILOP_TOP == pos)
	{
		UISpBlockFrame *rightFrame = GetSideFrame(UILOP_RIGHT);
		if (rightFrame)
		{
			return rightFrame->GetSideFrameVer0();
		}
		else
		{
			UISpBlockFrame *topestFrame = _GetTopestPosFrame(UILOP_RIGHT);
			if (topestFrame)
			{
				return topestFrame->GetSideFrameVer0();
			}
			else
			{
				return GetSideFrameHor1()->GetLinkFrame1();
			}
		}
	}

	return 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UISpBlockFrame::UISpBlockFrame(LoadConstructor value) :
UIFrame(value),
mSideFrameHor0(0),
mSideFrameHor1(0),
mSideFrameVer0(0),
mSideFrameVer1(0),
mParentSpBlockFrame(0)
{
}
//----------------------------------------------------------------------------
void UISpBlockFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UISpBlockFrame, source);
}
//----------------------------------------------------------------------------
void UISpBlockFrame::Link(InStream& source)
{
	UIFrame::Link(source);

	if (mIPTCtrl)
		source.ResolveLink(mIPTCtrl);
}
//----------------------------------------------------------------------------
void UISpBlockFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UISpBlockFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UISpBlockFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UISpBlockFrame, target);
}
//----------------------------------------------------------------------------
int UISpBlockFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------