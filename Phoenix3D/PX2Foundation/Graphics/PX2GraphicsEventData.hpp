// PX2GraphicsEventData.hpp

#ifndef PX2GRAPHICSEVENTDATA_HPP
#define PX2GRAPHICSEVENTDATA_HPP

#include "PX2GraphicsPre.hpp"

namespace PX2
{

	class Object;

	class PX2_FOUNDATION_ITEM AddObjectData
	{
	public:
		AddObjectData();
		~AddObjectData();

		Object *ParentObj;
		Object *Obj;
	};

}

#endif