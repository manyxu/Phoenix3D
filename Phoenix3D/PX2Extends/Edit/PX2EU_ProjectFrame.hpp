// PX2EU_ProjectFrame.hpp

#ifndef PX2EU_PROJECTFRAME_HPP
#define PX2EU_PROJECTFRAME_HPP

#include "PX2EU_ProjectTree.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EU_ProjectFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_ProjectFrame);

	public:
		EU_ProjectFrame();
		virtual ~EU_ProjectFrame();

	protected:
		UIFramePtr mProjFrame;
		EU_ProjectTreePtr mTreeProject;
		EU_ProjectTreePtr mTreeScene;
		EU_ProjectTreePtr mTreeUI;
		UITabFramePtr mTableFrame;
	};

	PX2_REGISTER_STREAM(EU_ProjectFrame);
	typedef Pointer0<EU_ProjectFrame> EU_ProjectFramePtr;

}

#endif