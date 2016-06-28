// PX2ScriptController.hpp

#ifndef PX2SCRIPTCONTROLLER_HPP
#define PX2SCRIPTCONTROLLER_HPP

#include "PX2UnityPre.hpp"
#include "PX2Controller.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ScriptController : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(ScriptController);

	public:
		ScriptController();
		virtual ~ScriptController();

		virtual void SetControlledable(Controlledable* object);

		virtual void ResetPlay();
		virtual void Reset();
		virtual void Play();
		virtual void Stop();

		virtual bool Update(double applicationTime, double elapsedTime);
		virtual void FixUpdate();

		virtual void ScriptOnAttached();
		virtual void ScriptOnDetach();
		virtual void ScriptOnInitUpdate();
		virtual void ScriptOnUpdate();
		virtual void ScriptOnFixUpdate();

		virtual void ScriptOnResetPlay();
		virtual void ScriptOnReset();
		virtual void ScriptOnPlay();
		virtual void ScriptOnStop();
		virtual void ScriptOnPlayUpdate();

		void SetNumFixUpdatePerSeconds(int num);
		int GetNumFixUpdatePerSeconds() const;

	protected:
		bool mIsInitUpdated;
		int mNumFixUpdatePerSeconds;
		float mOneFrameSeconds;
		float mFiexUpdateTiming;
	};

	PX2_REGISTER_STREAM(ScriptController);
	typedef PointerRef<ScriptController> ScriptControllerPtr;

}

#endif