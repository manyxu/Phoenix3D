// PX2Scene.hpp

#ifndef PX2SCENE_HPP
#define PX2SCENE_HPP

#include "PX2Node.hpp"
#include "PX2EnvirParam.hpp"
#include "PX2CameraNode.hpp"

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

		EnvirParam *GetEnvirParam();
		std::vector<CameraPtr> &GetCameras();

		virtual void UpdateWorldData(double applicationTime,
			double elapsedTime);

	protected:
		EnvirParamPtr mEnvirParam;
		std::vector<CameraPtr> mCameras;

	public:
		void OnEvent(Event *ent);
	};

#include "PX2Scene.inl"
	PX2_REGISTER_STREAM(Scene);
	typedef PointerRef<Scene> ScenePtr;

}

#endif