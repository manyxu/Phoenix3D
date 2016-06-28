// PX2Scene.cpp

#include "PX2Scene.hpp"
#include "PX2Creater.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2GraphicsEventData.hpp"
#include "PX2Float2.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Node, Scene);
PX2_IMPLEMENT_STREAM(Scene);
PX2_IMPLEMENT_FACTORY(Scene);
PX2_IMPLEMENT_DEFAULT_NAMES(Node, Scene);

//----------------------------------------------------------------------------
Scene::Scene() 
{
	ComeInEventWorld();

	SetName("Scene");

	mEnvirParam = new0 EnvirParam();

	CameraNode *camNode = PX2_CREATER.CreateNode_Camera();
	AttachChild(camNode);
	camNode->SetName("MainCameraNode");
}
//----------------------------------------------------------------------------
Scene::~Scene()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void Scene::UpdateWorldData(double applicationTime, double elapsedTime)
{
	float mHorAngle;
	float mVerAngle;

	Float3 mAmbientColor;
	Float3 mDiffuseColor;
	Float3 mSpecularColor;
	float mSpecularPow;
	float mIntensity;

	APoint mLightCameraLookPosition;
	float mLightCameraLookDistance;
	float mLightCameraExtent;

	Float3 mFogColorHeight;
	Float2 mFogParamHeight;
	Float3 mFogColorDist;
	Float2 mFogParamDist;

	mHorAngle = 45.0f;
	mVerAngle = 45.0f;
	mLightCameraExtent = 10.0f;
	mSpecularPow = 10.0f;
	mIntensity = 1.0f;
	mLightCameraLookDistance = 50.0f;

	mAmbientColor = Float3(0.4f, 0.4f, 0.4f);
	mDiffuseColor = Float3(1.0f, 1.0f, 1.0f);
	mSpecularColor = Float3(1.0f, 1.0f, 1.0f);

	mFogColorHeight = Float3::WHITE;
	mFogParamHeight = Float2(-10.0f, 0.0f);

	mFogColorDist = Float3::WHITE;
	mFogParamDist = Float2(80.0f, 120.0f);

	AVector dir = AVector::AnglesToDirection(Mathf::DEG_TO_RAD*mHorAngle,
		Mathf::DEG_TO_RAD*mVerAngle);
	dir.Normalize();

	Scene *scene = this;
	if (scene)
	{
		EnvirParam *envirParam = scene->GetEnvirParam();

		Light *lightDir = envirParam->GetLight_Dir();
		Projector *projector = envirParam->GetLight_Dir_Projector();

		lightDir->Ambient = Float4(mAmbientColor[0], mAmbientColor[1],
			mAmbientColor[2], mIntensity);
		lightDir->Intensity = mIntensity;
		lightDir->Diffuse = Float4(1.0f, mDiffuseColor[1],
			mDiffuseColor[2], 1.0f);
		lightDir->Specular = Float4(mSpecularColor[0], mSpecularColor[1],
			mSpecularColor[2], mSpecularPow);

		float upDot = dir.Dot(-AVector::UNIT_Z);
		if (upDot >= 0.99f)
		{
		}
		else
		{
			AVector upTemp = AVector::UNIT_Z;
			AVector right = dir.UnitCross(upTemp);
			AVector up = right.UnitCross(dir);

			lightDir->DVector = dir;
			lightDir->UVector = up;
			lightDir->RVector = right;

			APoint camPos = mLightCameraLookPosition - dir*mLightCameraLookDistance;
			projector->SetFrame(camPos, lightDir->DVector,
				lightDir->UVector, lightDir->RVector);
		}

		if (!projector->IsPerspective())
		{
			projector->SetFrustum(0.1f, 100.0f,
				-mLightCameraExtent, mLightCameraExtent, -mLightCameraExtent,
				mLightCameraExtent);
		}
		else
		{
			projector->SetFrustum(mLightCameraExtent, 1.0f, 1.0f, 100.0f);
		}
	}


	Node::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void Scene::DoExecute(Event *ent)
{
	if (GraphicsES::IsEqual(ent, GraphicsES::AddObject))
	{
		AddObjectData data = ent->GetData<AddObjectData>();
		CameraNode *camNode = DynamicCast<CameraNode>(data.Obj);
		if (camNode)
		{
			mCameras.push_back(camNode->GetCamera());
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void Scene::RegistProperties()
{
	Node::RegistProperties();

	AddPropertyClass("Scene");
}
//----------------------------------------------------------------------------
void Scene::OnPropertyChanged(const PropertyObject &obj)
{
	Node::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
Scene::Scene(LoadConstructor value) :
Node(value)
{
}
//----------------------------------------------------------------------------
void Scene::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Node::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(Scene, source);
}
//----------------------------------------------------------------------------
void Scene::Link(InStream& source)
{
	Node::Link(source);
}
//----------------------------------------------------------------------------
void Scene::PostLink()
{
	Node::PostLink();
}
//----------------------------------------------------------------------------
bool Scene::Register(OutStream& target) const
{
	if (Node::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Scene::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Node::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(Scene, target);
}
//----------------------------------------------------------------------------
int Scene::GetStreamingSize(Stream &stream) const
{
	int size = Node::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	
	return size;
}
//----------------------------------------------------------------------------