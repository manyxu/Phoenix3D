// PX2LP_Manager.hpp

#ifndef PX2LP_MANAGER_HPP
#define PX2LP_MANAGER_HPP

#include "PX2LP_Pre.hpp"
#include "PX2EventHandler.hpp"
#include "PX2Singleton.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UITabFrame.hpp"

namespace PX2
{

	class PX2LP_DLL_ITEM LP_Manager : public EventHandler, public Visitor
	{
	public:
		PX2_SINGLETION(LP_Manager);

	public:
		void Initlize();
		void Terminate();
		void Update(double appSeconds, double elapsedSeconds);

		virtual void DoExecute(Event *event);
		virtual void Visit(Object *obj, int info);

	protected:
		UIFrame *CreateHeadFrame();
		UIFrame *AddTabFrame(const std::string &name, const std::string &title);
		UIFrame *CreateHubFrame();
		UIFrame *CreateEngineFrame();

		UITabFramePtr mTableFrame;
	};

}

#endif