// PX2LightModelPositionConstant.hpp

#ifndef PX2LIGHTMODELPOSITIONCONSTANT_HPP
#define PX2LIGHTMODELPOSITIONCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2LightConstant.hpp"

namespace PX2
{

	class PX2_FOUNDATION_ITEM LightModelPositionConstant : public LightConstant
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(LightModelPositionConstant);

	public:
		LightModelPositionConstant (Light* light);
		virtual ~LightModelPositionConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(LightModelPositionConstant);
	typedef PointerRef<LightModelPositionConstant> LightModelPositionConstantPtr;

}

#endif
