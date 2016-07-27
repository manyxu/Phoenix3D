// PX2ObjCast.hpp

#ifndef PX2OBJCAST_HPP
#define PX2OBJCAST_HPP

#include "PX2Core.hpp"
#include "PX2Node.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Cast
	{
	public:
		static Movable *ToMovable(Object *obj);
		static Node *ToNode(Object *obj);
	};

}

#endif