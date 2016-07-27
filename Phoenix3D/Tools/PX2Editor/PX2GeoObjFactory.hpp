// PX2GeoObjFactory.hpp

#ifndef PX2GEOOBJFACTORY_HPP
#define PX2GEOOBJFACTORY_HPP

#include "PX2EditorPre.hpp"
#include "PX2Node.hpp"
#include "PX2Float4.hpp"

namespace PX2
{

	class Renderer;

	class PX2_EDITOR_ITEM GeoObjFactory
	{
	public:
		GeoObjFactory();
		~GeoObjFactory();

		PX2::Node *CreateTranslateCtrl_P();
		PX2::Node *CreateRolateCtrl_P();
		PX2::Node *CreateScaleCtrl_P();

		PX2::Node *CreateTranslateCtrl_O();
		PX2::Node *CreateRolateCtrl_O();
		PX2::Node *CreateScaleCtrl_O();

		PX2::Node *CreateBound();

		PX2::Node *CreateUICrossCtrl();
		PX2::Node *CreateUIRectCtrl(float smallRectSize);

		void UpdateCtrlColor(Movable *mov, Float4 color);
		void UpdateCtrlColor1(Movable *mov, Float4 color);
	};

}

#endif