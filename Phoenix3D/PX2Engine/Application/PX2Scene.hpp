// PX2Scene.hpp

#ifndef PX2SCENE_HPP
#define PX2SCENE_HPP

#include "PX2Node.hpp"
#include "PX2EnvirParamController.hpp"
#include "PX2AmbientRegionController.hpp"
#include "PX2CameraNode.hpp"
#include "PX2Terrain.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Scene : public Node
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_NEW(Scene);
		PX2_DECLARE_STREAM(Scene);

	public:
		Scene();
		virtual ~Scene();

		EnvirParamController *GetEnvirParamController();

		void SetTerrain(Terrain *terrain);
		Terrain *GetTerrain();

		bool AddCamera(Camera *camera);
		std::vector<CameraPtr> &GetCameras();

	protected:
		EnvirParamControllerPtr mEnvirParam;
		AmbientRegionControllerPtr mAmbientRegionController;
		TerrainPtr mTerrain;
		std::vector<CameraPtr> mCameras;

	public:
		void OnEvent(Event *ent);
	};

#include "PX2Scene.inl"
	PX2_REGISTER_STREAM(Scene);
	typedef PointerRef<Scene> ScenePtr;

}

#endif