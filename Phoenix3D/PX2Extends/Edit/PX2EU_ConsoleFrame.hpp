// PX2EU_Console.hpp

#ifndef PX2EU_CONSOLEFRAME_HPP
#define PX2EU_CONSOLEFRAME_HPP

#include "PX2UIFrame.hpp"
#include "PX2UIList.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EU_ConcoleFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_ConcoleFrame);

	public:
		EU_ConcoleFrame();
		virtual ~EU_ConcoleFrame();

	protected:
		UIListPtr mList;
	};

	PX2_REGISTER_STREAM(EU_ConcoleFrame);
	typedef Pointer0<EU_ConcoleFrame> EU_ConcoleFramePtr;

}

#endif