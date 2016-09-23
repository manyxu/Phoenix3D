// PX2Scene.cpp

#include "PX2Scene.hpp"
#include "PX2Creater.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2GraphicsEventData.hpp"
#include "PX2Float2.hpp"
#include "PX2UICanvas.hpp"
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

	mEnvirParam = new0 EnvirParamController();
	AttachController(mEnvirParam);
	mEnvirParam->ResetPlay();

	mAmbientRegionController = new0 AmbientRegionController();
	AttachController(mAmbientRegionController);
	mAmbientRegionController->ResetPlay();

	CameraNode *camNode = PX2_CREATER.CreateNode_Camera();
	AttachChild(camNode);
	camNode->SetName("MainCameraNode");
	camNode->GetCamera()->SetClearColor(Float4::BLUE);
	camNode->LocalTransform.SetTranslate(-10.0f, -10.0f, 10.0f);
	camNode->LookAt(APoint(0.0f, 0.0f, 0.0f));
}
//----------------------------------------------------------------------------
Scene::~Scene()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void Scene::SetTerrain(Terrain *terrain)
{
	mTerrain = terrain;
}
//----------------------------------------------------------------------------
Terrain *Scene::GetTerrain()
{
	return mTerrain;
}
//----------------------------------------------------------------------------
bool Scene::AddCamera(Camera *camera)
{
	if (!camera)
		return false;

	for (int i = 0; i < (int)mCameras.size(); i++)
	{
		if (mCameras[i] == camera)
			return false;
	}

	mCameras.push_back(camera);

	return true;
}
//----------------------------------------------------------------------------
void Scene::OnEvent(Event *ent)
{
	if (GraphicsES::IsEqual(ent, GraphicsES::AddObject))
	{
		AddObjectData data = ent->GetData<AddObjectData>();
		CameraNode *camNode = DynamicCast<CameraNode>(data.Obj);
		Terrain *terrain = DynamicCast<Terrain>(data.Obj);
		if (camNode)
		{
			Scene *scene = camNode->GetFirstParentDerivedFromType<Scene>();
			if (scene)
			{
				scene->AddCamera(camNode->GetCamera());
			}
		}
		else if (terrain)
		{
			SetTerrain(terrain);
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
	ComeInEventWorld();
}
//----------------------------------------------------------------------------
void Scene::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Node::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mEnvirParam);
	source.ReadPointer(mAmbientRegionController);

	PX2_END_DEBUG_STREAM_LOAD(Scene, source);
}
//----------------------------------------------------------------------------
void Scene::Link(InStream& source)
{
	Node::Link(source);

	if (mEnvirParam)
		source.ResolveLink(mEnvirParam);

	if (mAmbientRegionController)
		source.ResolveLink(mAmbientRegionController);
}
//----------------------------------------------------------------------------
void Scene::PostLink()
{
	Node::PostLink();

	if (mEnvirParam)
		mEnvirParam->PostLink();

	if (mEnvirParam)
		mEnvirParam->ResetPlay();

	if (mAmbientRegionController)
		mAmbientRegionController->PostLink();

	if (mAmbientRegionController)
		mAmbientRegionController->ResetPlay();
}
//----------------------------------------------------------------------------
bool Scene::Register(OutStream& target) const
{
	if (Node::Register(target))
	{
		if (mEnvirParam)
		{
			mEnvirParam->Register(target);
		}

		if (mAmbientRegionController)
		{
			mAmbientRegionController->Register(target);
		}

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

	target.WritePointer(mEnvirParam);
	target.WritePointer(mAmbientRegionController);

	PX2_END_DEBUG_STREAM_SAVE(Scene, target);
}
//----------------------------------------------------------------------------
int Scene::GetStreamingSize(Stream &stream) const
{
	int size = Node::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_POINTERSIZE(mEnvirParam);
	size += PX2_POINTERSIZE(mAmbientRegionController);
	
	return size;
}
//----------------------------------------------------------------------------