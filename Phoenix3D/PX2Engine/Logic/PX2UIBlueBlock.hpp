// PX2UIBlueBlock.hpp

#ifndef PX2UIBLUEBLOCK_HPP
#define PX2UIBLUEBLOCK_HPP

#include "PX2LogicPre.hpp"
#include "PX2UIButton.hpp"
#include "PX2UIFPicBox.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIBlueblock : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(UIBlueblock);

	public:
		UIBlueblock();
		virtual ~UIBlueblock();
	};

	PX2_REGISTER_STREAM(UIBlueblock);
	typedef PointerRef<UIBlueblock> UIBlueblockPtr;

}

#endif