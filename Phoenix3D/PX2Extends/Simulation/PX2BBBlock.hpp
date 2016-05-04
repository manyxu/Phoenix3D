// PX2BBBlock.hpp

#ifndef PX2BBBLOCK_HPP
#define PX2BBBLOCK_HPP

#include "PX2SimulationPre.hpp"
#include "PX2UIFrame.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM BBBlock : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(BBBlock);

	public:
		BBBlock();
		virtual ~BBBlock();

		enum BlockType
		{
			BT_EVENT,
			BT_FUNCTION,
			BT_PARAM,
			BT_OPERATOR,
			BT_LOGIC,
			BT_MAX_TYPE
		};

		enum ShapeType
		{
			ST_RECT_SQUAREHAT,
			ST_RECT,
			ST_IFELSE,
			ST_BOOL,
			ST_MATH_OPERAT,
			ST_MAX_TYPE
		};
	};
	
	PX2_REGISTER_STREAM(BBBlock);
	typedef Pointer0<BBBlock> BBBlockPtr;

}

#endif