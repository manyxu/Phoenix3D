// PX2EU_ProjectFrame.hpp

#ifndef PX2EU_PROJECTFRAME_HPP
#define PX2EU_PROJECTFRAME_HPP

#include "PX2EditorPre.hpp"
#include "PX2EU_ProjectTree.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_ProjectFrame : public UIFrame, public Visitor
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_ProjectFrame);

	public:
		EU_ProjectFrame();
		virtual ~EU_ProjectFrame();

		virtual void Visit(Object *obj, int info);

	protected:
		void _CreateToolFrame();

		UIFramePtr mToolFrame;
		UIFramePtr mSearchFrame;
		EU_ProjectTreePtr mTreeProject;
	};

	PX2_REGISTER_STREAM(EU_ProjectFrame);
	typedef PointerRef<EU_ProjectFrame> EU_ProjectFramePtr;

}

#endif