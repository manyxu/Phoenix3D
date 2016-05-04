// PX2SizeNode.cpp

#include "PX2SizeNode.hpp"
#include "PX2Canvas.hpp"
#include "PX2FunObject.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Node, SizeNode);
PX2_IMPLEMENT_STREAM(SizeNode);
PX2_IMPLEMENT_FACTORY(SizeNode);
PX2_IMPLEMENT_DEFAULT_NAMES(Node, SizeNode);

//----------------------------------------------------------------------------
SizeNode::SizeNode() :
mIsLayoutChanged(true),
mIsAnchorLayoutEnable(true),
mSize(Sizef(100.0f, 100.0f)),
mPvoit(Float2(0.5f, 0.5f)),
mSizeChangeTellToObject(0),
mSizeChangeCallback(0)
{
}
//----------------------------------------------------------------------------
SizeNode::~SizeNode()
{
}
//----------------------------------------------------------------------------
void SizeNode::UpdateWorldData(double applicationTime, double elapsedTime)
{
	if (mIsLayoutChanged)
	{
		if (mIsAnchorLayoutEnable)
		{
			UpdateLayout(GetParent());
			UpdateLeftBottomCornerOffset(GetParent());
		}
		else
		{
			UpdateLeftBottomCornerOffset(GetParent());
		}

		mIsLayoutChanged = false;
	}

	Node::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void SizeNode::OnBeAttached()
{
	Node::OnBeAttached();

	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void SizeNode::OnBeDetach()
{
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void SizeNode::SetSize(float width, float height)
{
	SetSize(Sizef(width, height));
}
//----------------------------------------------------------------------------
void SizeNode::SetSize(const Sizef &size)
{
	mSize = size;
	OnSizeChanged();
}
//----------------------------------------------------------------------------
void SizeNode::OnSizeChanged()
{
	_MarkRelatvieChange();

	if (mSizeChangeTellToObject && mSizeChangeCallback)
	{
		(mSizeChangeTellToObject->*mSizeChangeCallback)(this);
	}
}
//----------------------------------------------------------------------------
void SizeNode::SetWidth(float width)
{
	SetSize(Sizef(width, mSize.Height));
}
//----------------------------------------------------------------------------
void SizeNode::SetHeight(float height)
{
	SetSize(Sizef(mSize.Width, height));
}
//----------------------------------------------------------------------------
Rectf SizeNode::GetLocalRect() const
{
	Rectf rect;
	rect.Left = LeftBottomCornerOffset.X();
	rect.Bottom = LeftBottomCornerOffset.Z();
	rect.Right = rect.Left + GetWidth();
	rect.Top = rect.Bottom + GetHeight();

	return rect;
}
//----------------------------------------------------------------------------
Rectf SizeNode::GetWorldRect() const
{
	Rectf rect = GetLocalRect();

	const APoint worldPos = WorldTransform.GetTranslate();
	rect.Left += worldPos.X();
	rect.Right += worldPos.X();
	rect.Bottom += worldPos.Z();
	rect.Top += worldPos.Z();

	return rect;
}
//----------------------------------------------------------------------------
APoint SizeNode::WorldPosToViewPortPos(const APoint &worldPos)
{
	Rectf worldRect = GetWorldRect();

	APoint viewPortPos;
	viewPortPos.X() = worldPos.X() - worldRect.Left;
	viewPortPos.Z() = worldPos.Z() - worldRect.Bottom;

	return viewPortPos;
}
//----------------------------------------------------------------------------
bool SizeNode::IsInSizeRange(const SizeNode *node) const
{
	Rectf worldRect = GetWorldRect();
	Rectf nodeWorldRect = node->GetWorldRect();

	return  worldRect.IsInSide(nodeWorldRect);
}
//----------------------------------------------------------------------------
bool SizeNode::IsIntersectSizeRange(const SizeNode *node) const
{
	Rectf worldRect = GetWorldRect();
	Rectf nodeWorldRect = node->GetWorldRect();

	return  worldRect.IsIntersect(nodeWorldRect);
}
//----------------------------------------------------------------------------
void SizeNode::SetSizeChangeCallback(SizeNode *object, SizeChangeCallback callback)
{
	mSizeChangeTellToObject = object;
	mSizeChangeCallback = callback;
}
//----------------------------------------------------------------------------
void SizeNode::SetPivot(float x, float y)
{
	SetPivot(Float2(x, y));
}
//----------------------------------------------------------------------------
void SizeNode::SetPivot(const Float2 &pvoit)
{
	mPvoit = pvoit;

	OnPvoitChanged();
}
//----------------------------------------------------------------------------
void SizeNode::OnPvoitChanged()
{
	_MarkRelatvieChange();
}
//----------------------------------------------------------------------------
void SizeNode::EnableAnchorLayout(bool enable)
{
	mIsAnchorLayoutEnable = enable;
	_MarkRelatvieChange();
}
//----------------------------------------------------------------------------
void SizeNode::SetAnchorHor(float anchorX, float anchorY)
{
	SetAnchorHor(Float2(anchorX, anchorY));
}
//----------------------------------------------------------------------------
void SizeNode::SetAnchorHor(const Float2 &anchor)
{
	Float2 beforeAnchor = mAnchorHor;
	mAnchorHor = anchor;

	if (beforeAnchor != mAnchorHor)
		_MarkRelatvieChange();
}
//----------------------------------------------------------------------------
void SizeNode::SetAnchorVer(float anchorX, float anchorY)
{
	SetAnchorVer(Float2(anchorX, anchorY));
}
//----------------------------------------------------------------------------
void SizeNode::SetAnchorVer(const Float2 &anchor)
{
	Float2 beforeAnchor = mAnchorVer;
	mAnchorVer = anchor;

	if (beforeAnchor != mAnchorVer)
		_MarkRelatvieChange();
}
//----------------------------------------------------------------------------
void SizeNode::SetAnchorParamHor(float param0, float param1)
{
	SetAnchorParamHor(Float2(param0, param1));
}
//----------------------------------------------------------------------------
void SizeNode::SetAnchorParamHor(const Float2 &param)
{
	Float2 beforeParam = mAnchorParamHor;
	mAnchorParamHor = param;

	if (beforeParam != mAnchorParamHor)
		_MarkRelatvieChange();
}
//----------------------------------------------------------------------------
void SizeNode::SetAnchorParamVer(float param0, float param1)
{
	SetAnchorParamVer(Float2(param0, param1));
}
//----------------------------------------------------------------------------
void SizeNode::SetAnchorParamVer(const Float2 &param)
{
	Float2 beforeParam = mAnchorParamVer;
	mAnchorParamVer = param;

	if (beforeParam != mAnchorParamVer)
		_MarkRelatvieChange();
}
//----------------------------------------------------------------------------
void SizeNode::_MarkRelatvieChange()
{
	mIsLayoutChanged = true;

	for (int i = 0; i < GetNumChildren(); i++)
	{
		SizeNode *childSizeNode = DynamicCast<SizeNode>(GetChild(i));
		if (childSizeNode)
		{
			childSizeNode->_MarkRelatvieChange();
		}
	}
}
//----------------------------------------------------------------------------
void SizeNode::UpdateLayout(Movable *parent)
{
	Movable *par = parent;
	if (!par) return;

	SizeNode *parNode = DynamicCast<SizeNode>(par);

	// parent pvoit决定了从那一点作为原点，计算当前Frame原点位置
	// 例如
	// (0.0f, 0.0f)是左下角
	// (0.5f, 0.5f)是中心点	
	Sizef parentSize;
	AVector parentLeftBottomOffset;
	if (parNode)
	{
		parentSize = parNode->GetSize();
		parentLeftBottomOffset = parNode->LeftBottomCornerOffset;
	}

	APoint localPos = LocalTransform.GetTranslate();
	Sizef localSize = GetSize();

	APoint newPos;
	Sizef newSize = localSize;
	if (parNode)
	{
		float width = 0.0f;
		float height = 0.0f;

		bool isHEqual = (mAnchorHor[0] == mAnchorHor[1]);
		bool isVEqual = (mAnchorVer[0] == mAnchorVer[1]);

		if (isHEqual)
		{
			width = localSize.Width;
			newPos.X() = parentLeftBottomOffset.X() +
				parentSize.Width * mAnchorHor[0] + mAnchorParamHor[0];
		}
		else
		{
			// 如果是范围，直接取中心点，作为原点
			width = parentSize.Width * (mAnchorHor[1] - mAnchorHor[0]) - mAnchorParamHor[0] + mAnchorParamHor[1];
			newPos.X() = parentLeftBottomOffset.X() + parentSize.Width*mAnchorHor[0] + mAnchorParamHor[0] + width / 2.0f;
		}

		if (isVEqual)
		{
			height = localSize.Height;
			newPos.Z() = parentLeftBottomOffset.Z() +
				parentSize.Height * mAnchorVer[0] + mAnchorParamVer[0];
		}
		else
		{
			// 如果是范围，直接取中心点，作为原点
			height = parentSize.Height * (mAnchorVer[1] - mAnchorVer[0]) - mAnchorParamVer[0] + mAnchorParamVer[1];
			newPos.Z() = parentLeftBottomOffset.Z() + parentSize.Height*mAnchorVer[0] + mAnchorParamVer[0] + height / 2.0f;
		}

		newSize = Sizef(width, height);
	}

	newPos.Y() = localPos.Y();
	LocalTransform.SetTranslate(newPos);

	if (newSize != localSize)
	{
		SetSize(newSize);
	}
}
//----------------------------------------------------------------------------
void SizeNode::UpdateLeftBottomCornerOffset(Movable *parent)
{
	if (parent)
	{
		LeftBottomCornerOffset.X() = -mSize.Width * mPvoit[0];
		LeftBottomCornerOffset.Z() = -mSize.Height * mPvoit[1];
	}
	else
	{
		LeftBottomCornerOffset.X() = 0.0f;
		LeftBottomCornerOffset.Z() = 0.0f;
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SizeNode
//----------------------------------------------------------------------------
FunObject *SizeNode::RegistClassFunctions()
{
	FunObject *parentFunObject = Node::RegistClassFunctions();

	FunObject *thisFunObj = parentFunObject->GetAddClass("SizeNode");

	{
		FunObjectPtr funObj = new0 FunObject();
		funObj->FunName = "SetWidth";
		funObj->AddInput("handler", FPT_POINTER_THIS, (Object*)0);
		funObj->AddInput("in_width", FPT_FLOAT, 100.0f);
		thisFunObj->AddFunObject(funObj);
	}

	{
		FunObjectPtr funObj = new0 FunObject();
		funObj->FunName = "GetWidth";
		funObj->AddInput("handler", FPT_POINTER_THIS, (Object*)0);
		funObj->AddOutput("out_width", FPT_FLOAT, 100.0f);
		thisFunObj->AddFunObject(funObj);
	}

	{
		FunObjectPtr funObj = new0 FunObject();
		funObj->FunName = "SetHeight";
		funObj->AddInput("handler", FPT_POINTER_THIS, (Object*)0);
		funObj->AddInput("in_height", FPT_FLOAT, 100.0f);
		thisFunObj->AddFunObject(funObj);
	}

	{
		FunObjectPtr funObj = new0 FunObject();
		funObj->FunName = "GetHeight";
		funObj->AddInput("handler", FPT_POINTER_THIS, (Object*)0);
		funObj->AddOutput("out_height", FPT_FLOAT, 100.0f);
		thisFunObj->AddFunObject(funObj);
	}

	{
		FunObjectPtr funObj = new0 FunObject();
		funObj->FunName = "SetSize";
		funObj->AddInput("handler", FPT_POINTER_THIS, (Object*)0);
		funObj->AddInput("in_width", FPT_FLOAT, 100.0f);
		funObj->AddInput("in_height", FPT_FLOAT, 100.0f);
		thisFunObj->AddFunObject(funObj);
	}

	{
		FunObjectPtr funObj = new0 FunObject();
		funObj->FunName = "SetPivot";
		funObj->AddInput("handler", FPT_POINTER_THIS, (Object*)0);
		funObj->AddInput("in_x", FPT_FLOAT, 0.5f);
		funObj->AddInput("in_y", FPT_FLOAT, 0.5f);
		thisFunObj->AddFunObject(funObj);
	}

	{
		FunObjectPtr funObj = new0 FunObject();
		funObj->FunName = "SetAnchorHor";
		funObj->AddInput("handler", FPT_POINTER_THIS, (Object*)0);
		funObj->AddInput("in_anchorX", FPT_FLOAT, 0.5f);
		funObj->AddInput("in_anchorY", FPT_FLOAT, 0.5f);
		thisFunObj->AddFunObject(funObj);
	}

	{
		FunObjectPtr funObj = new0 FunObject();
		funObj->FunName = "SetAnchorParamHor";
		funObj->AddInput("handler", FPT_POINTER_THIS, (Object*)0);
		funObj->AddInput("in_param0", FPT_FLOAT, 0.0f);
		funObj->AddInput("in_param1", FPT_FLOAT, 0.0f);
		thisFunObj->AddFunObject(funObj);
	}

	{
		FunObjectPtr funObj = new0 FunObject();
		funObj->FunName = "SetAnchorVer";
		funObj->AddInput("handler", FPT_POINTER_THIS, (Object*)0);
		funObj->AddInput("in_anchorX", FPT_FLOAT, 0.5f);
		funObj->AddInput("in_anchorY", FPT_FLOAT, 0.5f);
		thisFunObj->AddFunObject(funObj);
	}

	{
		FunObjectPtr funObj = new0 FunObject();
		funObj->FunName = "SetAnchorParamVer";
		funObj->AddInput("handler", FPT_POINTER_THIS, (Object*)0);
		funObj->AddInput("in_param0", FPT_FLOAT, 0.0f);
		funObj->AddInput("in_param1", FPT_FLOAT, 0.0f);
		thisFunObj->AddFunObject(funObj);
	}

	return thisFunObj;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
SizeNode::SizeNode(LoadConstructor value):
Node(value),
mIsLayoutChanged(true),
mIsAnchorLayoutEnable(true),
mSize(Sizef(100.0f, 100.0f)),
mPvoit(Float2(0.5f, 0.5f)),
mSizeChangeTellToObject(0),
mSizeChangeCallback(0)
{
}
//----------------------------------------------------------------------------
void SizeNode::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Node::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadAggregate(mSize);

	source.ReadAggregate(mPvoit);

	source.ReadBool(mIsAnchorLayoutEnable);
	source.ReadAggregate(mAnchorHor);
	source.ReadAggregate(mAnchorVer);
	source.ReadAggregate(mAnchorParamHor);
	source.ReadAggregate(mAnchorParamVer);

	PX2_END_DEBUG_STREAM_LOAD(SizeNode, source);
}
//----------------------------------------------------------------------------
void SizeNode::Link(InStream& source)
{
	Node::Link(source);
}
//----------------------------------------------------------------------------
void SizeNode::PostLink()
{
	Node::PostLink();
}
//----------------------------------------------------------------------------
bool SizeNode::Register(OutStream& target) const
{
	return Node::Register(target);
}
//----------------------------------------------------------------------------
void SizeNode::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Node::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteAggregate(mSize);

	target.WriteAggregate(mPvoit);

	target.WriteBool(mIsAnchorLayoutEnable);
	target.WriteAggregate(mAnchorHor);
	target.WriteAggregate(mAnchorVer);
	target.WriteAggregate(mAnchorParamHor);
	target.WriteAggregate(mAnchorParamVer);

	PX2_END_DEBUG_STREAM_SAVE(SizeNode, target);
}
//----------------------------------------------------------------------------
int SizeNode::GetStreamingSize(Stream &stream) const
{
	int size = Node::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mSize);

	size += sizeof(mPvoit);

	size += PX2_BOOLSIZE(mIsAnchorLayoutEnable);
	size += sizeof(mAnchorHor);
	size += sizeof(mAnchorVer);
	size += sizeof(mAnchorParamHor);
	size += sizeof(mAnchorParamVer);

	return size;
}
//----------------------------------------------------------------------------