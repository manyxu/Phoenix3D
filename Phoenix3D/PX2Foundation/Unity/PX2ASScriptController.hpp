// PX2ASScriptController.hpp

#ifndef PX2ASSCRIPTCONTROLLER_HPP
#define PX2ASSCRIPTCONTROLLER_HPP

#include "PX2UnityPre.hpp"
#include "PX2ScriptController.hpp"

class asIScriptObject;

namespace PX2
{

	class PX2_FOUNDATION_ITEM ASScriptController : public ScriptController
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(ASScriptController);

	public:
		ASScriptController();
		virtual ~ASScriptController();

		virtual void Play();

		virtual void _Update(double applicationTime, double elapsedTime);

		asIScriptObject *ASIScriptObject;
	};

	PX2_REGISTER_STREAM(ASScriptController);
	typedef PointerRef<ASScriptController> ASScriptControllerPtr;

}

#endif