// PX2Creater.hpp

#ifndef PX2CREATER_HPP
#define PX2CREATER_HPP

#include "PX2Singleton_NeedNew.hpp"
#include "PX2Object.hpp"
#include "PX2APoint.hpp"
#include "PX2CameraNode.hpp"
using namespace PX2;

namespace PX2
{

	class Scene;

	class PX2_ENGINE_ITEM Creater : public Singleton<Creater>
	{
	public:
		Creater();
		virtual ~Creater();

		Node *CreateNode();
		Node *CreateNode_Box();
		CameraNode *CreateNode_Camera(bool createCamera = true);
		Node *CreateNode_Model(const std::string &filename);
	};

#define PX2_CREATER Creater::GetSingleton()

}

#endif