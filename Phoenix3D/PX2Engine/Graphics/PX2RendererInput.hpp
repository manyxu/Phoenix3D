// PX2RenderInput.hpp

#ifndef PX2RENDERINPUT_HPP
#define PX2RENDERINPUT_HPP

#include "PX2SmartPointerRef.hpp"

namespace PX2
{

	class RendererInput
	{
	public:
		RendererInput();
		virtual ~RendererInput();

		virtual bool Terminate();
	};

	typedef PointerRef<RendererInput> RendererInputPtr;

}

#endif