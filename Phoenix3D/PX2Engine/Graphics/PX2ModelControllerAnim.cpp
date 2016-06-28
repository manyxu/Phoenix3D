// PX2ModelControllerAnim.cpp

#include "PX2ModelController.hpp"
#include "PX2Animation3DSkeleton.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
bool ModelController::AddAnim(Animation *anim)
{
	if (HasAnim(anim)) return false;

	int id = anim->GetID();
	mAnimsMap[id] = anim;
	anim->SetCharacter(this);

	return true;
}
//----------------------------------------------------------------------------
bool ModelController::HasAnim(Animation *anim)
{
	return HasAnim(anim->GetID());
}
//----------------------------------------------------------------------------
bool ModelController::HasAnim(int id)
{
	std::map<int, AnimationPtr>::iterator it = mAnimsMap.find(id);
	if (it != mAnimsMap.end())
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool ModelController::RemoveAnim(Animation *anim)
{
	return RemoveAnim(anim->GetID());
}
//----------------------------------------------------------------------------
bool ModelController::RemoveAnim(int id)
{
	std::map<int, AnimationPtr>::iterator it = mAnimsMap.find(id);
	if (it != mAnimsMap.end())
	{
		it->second->SetCharacter(0);

		mAnimsMap.erase(it);
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void ModelController::RemoveAllAnims()
{
	std::map<int, AnimationPtr>::iterator it = mAnimsMap.begin();
	for (; it != mAnimsMap.end(); it++)
	{
		it->second->SetCharacter(0);
	}

	mAnimsMap.clear();
}
//----------------------------------------------------------------------------
int ModelController::GetNumAnims() const
{
	return (int)mAnimsMap.size();
}
//----------------------------------------------------------------------------
Animation *ModelController::GetAnimByID(int id)
{
	std::map<int, AnimationPtr>::iterator it = mAnimsMap.find(id);
	if (it != mAnimsMap.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
void ModelController::SetDefaultAnimID(int id)
{
	mDefaultAnim = GetAnimByID(id);
}
//----------------------------------------------------------------------------
void ModelController::PlayAnim(Animation *anim)
{
	if (!HasAnim(anim)) return;

	bool sameAnim = false;
	if (mCurPlayingAnim == anim)
		sameAnim = true;

	if (mCurPlayingAnim)
		mCurPlayingAnim->Stop();

	if (AT_SKELETON == mAnimType)
	{
		Node *charNode = DynamicCast<Node>(GetControlledable());
		if (mCurPlayingAnim && charNode)
		{
			Animation3DSkeleton *anim3dSk = DynamicCast<Animation3DSkeleton>(mCurPlayingAnim);

			if (anim3dSk)
			{
				Node *animNode = anim3dSk->GetAnimNode();
				charNode->DetachChild(animNode);
			}

			if (!sameAnim)
			{
				mLastAnimObj = new0 AnimationObject();
				mLastAnimObj->TheAnim = mCurPlayingAnim;
				mLastAnimObj->BlendTime = 0.0f;
				mLastAnimObj->TheCharacter = this;
			}
			else
			{
				mLastAnimObj = 0;
			}
		}
	}

	mCurPlayingAnim = anim;

	if (mCurPlayingAnim)
		mCurPlayingAnim->OnPlay(this);
}
//----------------------------------------------------------------------------
void ModelController::PlayAnimByID(int id)
{
	Animation *anim = GetAnimByID(id);
	if (anim) PlayAnim(anim);
}
//----------------------------------------------------------------------------
void ModelController::StopAnim(Animation *anim)
{
	if (!HasAnim(anim))
		return;

	anim->Stop();
}
//----------------------------------------------------------------------------
void ModelController::PlayCurAnim()
{
	if (mCurPlayingAnim)
		mCurPlayingAnim->LetCharacterPlay();
}
//----------------------------------------------------------------------------
void ModelController::StopCurAnim()
{
	if (mCurPlayingAnim)
		mCurPlayingAnim->Stop();
}
//----------------------------------------------------------------------------
bool ModelController::IsHasAnimPlaying() const
{
	std::map<int, AnimationPtr>::const_iterator it = mAnimsMap.begin();
	for (; it != mAnimsMap.end(); it++)
	{
		if (it->second->IsPlaying())
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------