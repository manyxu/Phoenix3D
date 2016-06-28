// PX2AnimationController.cpp

#include "PX2ModelController.hpp"
#include "PX2TriMesh.hpp"
#include "PX2SkinController.hpp"
#include "PX2Animation3DSkeleton.hpp"
#include "PX2StandardMesh.hpp"
#include "PX2VertexColor4Material.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Controller, ModelController);
PX2_IMPLEMENT_STREAM(ModelController);
PX2_IMPLEMENT_FACTORY(ModelController);
PX2_IMPLEMENT_DEFAULT_NAMES(Controller, ModelController);

//----------------------------------------------------------------------------
ModelController::ModelController() :
mAnimType(AT_NONE),
mDefaultAnimID(0),
mIsMovableAutoWorldBound(true),
mMovableAutoWorldBoundRadius(2.0f)
{
	SetName("ModelController");

	//VertexFormat *vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PC);
	//StandardMesh stdMesh(vf);
	//stdMesh.SetVertexColor(Float4(1.0f, 0.0f, 0.0f, 1.0f));
	//TriMesh *mesh = stdMesh.Box(0.5f, 0.5f, 0.5f);
	//PX2::VertexColor4MaterialPtr mtl = new0 VertexColor4Material();
	//mtl->GetWireProperty(0, 0)->Enabled = true;
	//mtl->GetCullProperty(0, 0)->Enabled = false;
	//mesh->LocalTransform.SetUniformScale(0.5f);
	//mesh->SetMaterialInstance(mtl->CreateInstance());
	//CreateGetHelpNode()->DetachAllChildren();
	//CreateGetHelpNode()->AttachChild(mesh);
	//CreateGetHelpNode()->SetParentTransformIngore(false, false, true);
}
//----------------------------------------------------------------------------
ModelController::~ModelController()
{
}
//----------------------------------------------------------------------------
void ModelController::_Update(double applicationTime, double elapsedTime)
{
	PX2_UNUSED(applicationTime);
	PX2_UNUSED(elapsedTime);

	if (AT_SKELETON == mAnimType)
		_AnimationBlend();

	Node *node = DynamicCast<Node>(GetControlledable());

	if (mMovable)
	{
		if (mIsMovableAutoWorldBound)
		{
			mMovable->WorldBoundIsCurrent = true;
			if (node)
				mMovable->WorldBound.SetCenter(
				node->WorldTransform.GetTranslate());
			mMovable->WorldBound.SetRadius(mMovableAutoWorldBoundRadius);
		}
		else
		{
			mMovable->WorldBoundIsCurrent = false;
		}
	}
}
//----------------------------------------------------------------------------
void ModelController::_AnimationBlend()
{
	if (mLastAnimObj)
	{
		//mLastAnimObj->Update(appSeconds, elapsedSeconds);

		if (mLastAnimObj->IsBlendOver())
		{
			mLastAnimObj->BeforeRemove();
			mLastAnimObj = 0;
		}
	}

	float weight = 1.0f;
	if (mLastAnimObj)
	{
		weight = mLastAnimObj->BlendTime / ANIMATION_BLENDTIME;
	}

	Animation3DSkeleton *lastAnim = 0;
	float lastAnimTime = 0.0f;
	if (mLastAnimObj)
	{
		lastAnim = DynamicCast<Animation3DSkeleton>(mLastAnimObj->TheAnim);
		lastAnimTime = lastAnim->GetTime();
	}
	Animation3DSkeleton *curAnim = DynamicCast<Animation3DSkeleton>(mCurPlayingAnim);
	float curAnimTime = 0.0f;
	if (curAnim)
	{
		curAnimTime = curAnim->GetTime();
	}

	std::map<FString, KeyframeControllerPtr> *lastAnimKFCMap = 0;
	if (lastAnim)
	{
		lastAnimKFCMap = &(lastAnim->GetKeyframeCtrlMap());
	}
	std::map<FString, KeyframeControllerPtr> *curAnimKFCMap = 0;
	bool isUseBlend = true;
	if (curAnim)
	{
		curAnimKFCMap = &(curAnim->GetKeyframeCtrlMap());
		isUseBlend = curAnim->IsUseBlend();
	}

	std::map<FString, BlendTransformController*>::iterator it = mBTCMap.begin();
	for (; it != mBTCMap.end(); it++)
	{
		FString name = it->first;
		BlendTransformController *ctrl = it->second;

		if (lastAnimKFCMap && curAnimKFCMap)
		{
			if (isUseBlend)
			{
				ctrl->SetController01((*lastAnimKFCMap)[name], (*curAnimKFCMap)[name]);
				ctrl->SetWeight(weight);
			}
			else
			{
				ctrl->SetController0(0);
				ctrl->SetController1((*curAnimKFCMap)[name]);
				ctrl->SetWeight(1.0f);
			}
		}
		if (curAnimKFCMap)
		{
			ctrl->SetController0(0);
			ctrl->SetController1((*curAnimKFCMap)[name]);
			ctrl->SetWeight(1.0f);
		}
		if (!lastAnimKFCMap && !curAnimKFCMap)
		{
			ctrl->SetController01(0, 0);
		}
	}
}
//----------------------------------------------------------------------------
void ModelController::_CalSkins()
{
	Node *node = DynamicCast<Node>(mMovable);

	if (!node)
		return;

	for (int i = 0; i < node->GetNumChildren(); i++)
	{
		Movable *mov = node->GetChild(i);
		TriMesh *triMesh = DynamicCast<TriMesh>(mov);
		Node *node = DynamicCast<Node>(mov);
		if (triMesh)
		{
			bool hasSkinCtrl = false;

			for (int i = 0; i < triMesh->GetNumControllers(); i++)
			{
				SkinController *skinCtrl = DynamicCast<SkinController>(
					triMesh->GetController(i));
				if (skinCtrl)
				{
					hasSkinCtrl = true;
				}
			}

			if (hasSkinCtrl)
				mSkinMovables.push_back(triMesh);
		}
		else if (node)
		{
			bool hasSkinCtrl = false;

			for (int i = 0; i < node->GetNumChildren(); i++)
			{
				TriMesh *mesh = DynamicCast<TriMesh>(node->GetChild(i));
				if (mesh)
				{
					for (int i = 0; i < mesh->GetNumControllers(); i++)
					{
						SkinController *skinCtrl = DynamicCast<SkinController>(mesh->GetController(i));
						if (skinCtrl)
						{
							hasSkinCtrl = true;
						}
					}
				}
			}

			if (hasSkinCtrl)
			{
				mSkinMovables.push_back(node);
			}
			else
			{
				int numVaildChild = node->GetNumChildren();
				if (0 != numVaildChild)
				{
					mModelAnimMovable = node;
				}
			}
		}
	}

	for (int i = 0; i < (int)mSkinMovables.size(); i++)
	{
		mSkinMovables[i]->SetUpdatePriority(1);
	}

	if (mModelAnimMovable)
	{
		mModelAnimMovable->SetUpdatePriority(2);
	}

	Node *rootNode = DynamicCast<Node>(mMovable);
	if (rootNode)
	{
		node->SetNeedCalUpdateChild(true);
	}
}
//----------------------------------------------------------------------------
void ModelController::_CalAnimNode(Movable *mov)
{
	const std::string &movName = mov->GetName();
	Node *node = DynamicCast<Node>(mov);
	if (node)
	{
		BlendTransformController *btc = DynamicCast<BlendTransformController>(
			node->GetControllerByName("BTC"));
		if (!btc)
		{
			btc = new0 BlendTransformController(0, 0, true);
			node->AttachController(btc);
			btc->SetName("BTC");
		}
		mBTCMap[movName.c_str()] = btc;

		for (int i = 0; i < node->GetNumChildren(); i++)
		{
			_CalAnimNode(node->GetChild(i));
		}
	}
}
//----------------------------------------------------------------------------
void ModelController::SetControlledable(Controlledable* object)
{
	Controller::SetControlledable(object);

	if (object)
	{
		if (mMovable)
		{
			Node *parentNode = DynamicCast<Node>(object);
			if (parentNode)
			{
				parentNode->AttachChild(mMovable);
			}
		}
	}
	else
	{
		if (mMovable)
		{
			Node *parentNode = DynamicCast<Node>(object);
			if (parentNode)
			{
				parentNode->DetachChild(mMovable);
			}
		}
	}
}
//----------------------------------------------------------------------------
void ModelController::SetMovableAutoWorldBoundRadius(float radius)
{
	mMovableAutoWorldBoundRadius = radius;

	Node *node = DynamicCast<Node>(GetControlledable());

	if (mMovable)
	{
		if (mIsMovableAutoWorldBound)
		{
			mMovable->WorldBoundIsCurrent = true;

			if (node)
			{
				mMovable->WorldBound.SetCenter(node->WorldTransform.GetTranslate());
			}
			mMovable->WorldBound.SetRadius(mMovableAutoWorldBoundRadius);
		}
		else
		{
			mMovable->WorldBoundIsCurrent = false;
		}

		//if (node)
		//	node->UpdateWorldBound();
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void ModelController::RegistProperties()
{
	Controller::RegistProperties();

	AddPropertyClass("ModelController");

	AddProperty("IsMovableAutoWorldBound", PT_BOOL, mIsMovableAutoWorldBound);
	AddProperty("MovableAutoWorldBoundRadius", PT_FLOAT,
		mMovableAutoWorldBoundRadius);

	AddProperty("NumAnimations", PT_INT, (int)mAnimsMap.size(), false);
	AddProperty("DefaultAnimID", PT_INT, mDefaultAnimID);
}
//----------------------------------------------------------------------------
void ModelController::OnPropertyChanged(const PropertyObject &obj)
{
	Controller::OnPropertyChanged(obj);

	if ("IsMovableAutoWorldBound" == obj.Name)
	{
		SetMovableUseAutoWorldBound(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("MovableAutoWorldBoundRadius" == obj.Name)
	{
		SetMovableAutoWorldBoundRadius(PX2_ANY_AS(obj.Data, float));
	}
	else if ("DefaultAnimID" == obj.Name)
	{
		SetDefaultAnimID(PX2_ANY_AS(obj.Data, int));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
ModelController::ModelController(LoadConstructor value) :
Controller(value),
mDefaultAnimID(0),
mIsMovableAutoWorldBound(true),
mMovableAutoWorldBoundRadius(2.0f)
{
}
//----------------------------------------------------------------------------
void ModelController::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Controller::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(ModelController, source);
}
//----------------------------------------------------------------------------
void ModelController::Link(InStream& source)
{
	Controller::Link(source);
}
//----------------------------------------------------------------------------
void ModelController::PostLink()
{
	Controller::PostLink();
}
//----------------------------------------------------------------------------
bool ModelController::Register(OutStream& target) const
{
	if (Controller::Register(target))
		return true;

	return false;
}
//----------------------------------------------------------------------------
void ModelController::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Controller::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(ModelController, target);
}
//----------------------------------------------------------------------------
int ModelController::GetStreamingSize(Stream &stream) const
{
	int size = Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------