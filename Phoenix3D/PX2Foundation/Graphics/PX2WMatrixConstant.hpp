// PX2WMatrixConstant.hpp

#ifndef PX2WMATRIXCONSTANT_HPP
#define PX2WMATRIXCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"

namespace PX2
{

	class PX2_FOUNDATION_ITEM WMatrixConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(WMatrixConstant);

	public:
		WMatrixConstant ();
		virtual ~WMatrixConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(WMatrixConstant);
	typedef PointerRef<WMatrixConstant> WMatrixConstantPtr;

}

#endif
