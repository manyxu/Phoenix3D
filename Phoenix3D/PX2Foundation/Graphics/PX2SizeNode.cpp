// PX2SizeNode.cpp

#include "PX2SizeNode.hpp"
#include "PX2Canvas.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Node, SizeNode);
PX2_IMPLEMENT_STREAM(SizeNode);
PX2_IMPLEMENT_FACTORY(SizeNode);
PX2_IMPLEMENT_DEFAULT_NAMES(Node, SizeNode);

//----------------------------------------------------------------------------
SizeNode::SizeNode() :
mIsLayoutChanged(true),
mIsAnchorLayoutEnable(true)
{
	mSize = Sizef(100.0f, 100.0f);
	mPvoit = Float2(0.5f, 0.5f);
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
			UpdateLayout();
		}
	}

	Node::UpdateWorldData(applicationTime, elapsedTime);
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
void SizeNode::SetBorderSize(float width, float height)
{
	SetBorderSize(Sizef(width, height));
}
//----------------------------------------------------------------------------
void SizeNode::SetBorderSize(const Sizef &size)
{
	mBorderSize = size;

	OnBorderSizeChanged();
}
//----------------------------------------------------------------------------
void SizeNode::SetBorderWidth(float width)
{
	SetBorderSize(Sizef(width, mSize.Height));
}
//----------------------------------------------------------------------------
void SizeNode::SetBorderHeight(float height)
{
	SetBorderSize(Sizef(mSize.Width, height));
}
//----------------------------------------------------------------------------
void SizeNode::OnBorderSizeChanged()
{
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
	mAnchorHor = anchor;
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
	mAnchorVer = anchor;
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
	mAnchorParamHor = param;
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
	mAnchorParamVer = param;
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
void SizeNode::UpdateLayout()
{
	Movable *par = GetParent();
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
	const Float2 &pvoit = GetPvoit();

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

			LeftBottomCornerOffset.X() = -width * pvoit[0];
		}
		else
		{
			// 如果是范围，直接取中心点，作为原点
			width = parentSize.Width * (mAnchorHor[1] - mAnchorHor[0]) - mAnchorParamHor[0] - mAnchorParamHor[1];
			newPos.X() = parentLeftBottomOffset.X() + parentSize.Width*mAnchorHor[0] + mAnchorParamHor[0] + width / 2.0f;

			LeftBottomCornerOffset.X() = -width / 2.0f;
		}

		if (isVEqual)
		{
			height = localSize.Height;
			newPos.Z() = parentLeftBottomOffset.Z() +
				parentSize.Height * mAnchorVer[0] + mAnchorParamVer[0];

			LeftBottomCornerOffset.Z() = -height * pvoit[1];
		}
		else
		{
			// 如果是范围，直接取中心点，作为原点
			height = parentSize.Height * (mAnchorVer[1] - mAnchorVer[0]) - mAnchorParamVer[0] - mAnchorParamVer[1];
			newPos.Z() = parentLeftBottomOffset.Z() + parentSize.Height*mAnchorVer[0] + mAnchorParamVer[0] + height / 2.0f;

			LeftBottomCornerOffset.Z() = -height / 2.0f;
		}

		newSize = Sizef(width, height);
	}

	newPos.Y() = localPos.Y();
	LocalTransform.SetTranslate(newPos);

	if (newSize != localSize)
	{
		SetSize(newSize);
	}

	mIsLayoutChanged = false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
SizeNode::SizeNode(LoadConstructor value):
Node(value)
{
}
//----------------------------------------------------------------------------
void SizeNode::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Node::Load(source);
	PX2_VERSION_LOAD(source);

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

	PX2_END_DEBUG_STREAM_SAVE(SizeNode, target);
}
//----------------------------------------------------------------------------
int SizeNode::GetStreamingSize(Stream &stream) const
{
	int size = Node::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------