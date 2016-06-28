// PX2ModelControllerMovable.cpp

#include "PX2ModelController.hpp"
#include "PX2ResourceManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
void ModelController::SetMovableFilename(const std::string &filename, 
	bool shareVI)
{
	mMovableFilename = filename;

	mIsShareVI = shareVI;

	Object *obj = 0;
	if (mIsShareVI)
	{
		obj = PX2_RM.BlockLoadShareCopy(filename, true, true, false);
	}
	else
	{
		obj = PX2_RM.BlockLoadCopy(filename);
	}

	Movable *mov = DynamicCast<Movable>(obj);
	if (mov)
	{
		SetMovable(mov);
		mov->SetSaveWriteIngore(true);
		mov->ResetPlay();
	}

	if (AT_SKELETON == mAnimType)
	{
		_CalSkins();

		// 为动作树每个Node加一个BlendTransformController
		if (mModelAnimMovable)
			_CalAnimNode(mModelAnimMovable);

		SetMovableAutoWorldBoundRadius(mMovableAutoWorldBoundRadius);
	}
}
//----------------------------------------------------------------------------
void ModelController::SetMovable(Movable *mov)
{
	Node *node = DynamicCast<Node>(GetControlledable());

	if (mMovable && node)
		node->DetachChild(mMovable);

	mMovable = mov;

	if (mMovable && node)
		node->AttachChild(mMovable);

	CollectAnchors();

	mov->SetSaveWriteIngore(false);
}
//----------------------------------------------------------------------------
void ModelController::SetHeading(const AVector &heading)
{
	mHeading = heading;
}
//----------------------------------------------------------------------------
void ModelController::CollectAnchors()
{
	mAnchorMap.clear();

	Node *node = DynamicCast<Node>(GetControlledable());
	if (node)
		_CollectAnchor(node);
}
//----------------------------------------------------------------------------
APoint ModelController::GetAnchorPos(int id)
{
	if (!mMovable) return APoint::ORIGIN;

	Node *node = GetAnchor(id);

	if (node)
		return node->WorldTransform.GetTranslate();

	return APoint::ORIGIN;
}
//----------------------------------------------------------------------------
Node *ModelController::GetAnchor(int id)
{
	if (!mMovable) return 0;

	std::map<int, Node*>::iterator it = mAnchorMap.find(id);
	if (it != mAnchorMap.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
void ModelController::_CollectAnchor(Movable *mov)
{
	if (!mov) return;

	Node *node = DynamicCast<Node>(mov);
	if (node)
	{
		int anchorId = node->GetAnchorID();
		if (0 != anchorId)
		{
			mAnchorMap[anchorId] = node;
		}

		for (int i = 0; i < node->GetNumChildren(); i++)
		{
			Node *childNode = DynamicCast<Node>(node->GetChild(i));
			if (childNode)
			{
				_CollectAnchor(childNode);
			}
		}
	}
}
//----------------------------------------------------------------------------