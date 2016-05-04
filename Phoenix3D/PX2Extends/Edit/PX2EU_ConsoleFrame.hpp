// PX2EU_ConsoleFrame.hpp

#ifndef PX2EU_CONSOLEFRAME_HPP
#define PX2EU_CONSOLEFRAME_HPP

#include "PX2UIFrame.hpp"
#include "PX2UIList.hpp"
#include "PX2Log.hpp"
#include "PX2UIEditBox.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EU_ConcoleFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_ConcoleFrame);

	public:
		EU_ConcoleFrame();
		virtual ~EU_ConcoleFrame();

		virtual void Handle(const LogBuffer *logBuffer,
			const char *timeStamp);

	protected:
		UIFramePtr mToolFrame;
		UIEditBoxPtr mEditBox;
		UIListPtr mList;
	};

	PX2_REGISTER_STREAM(EU_ConcoleFrame);
	typedef PointerRef<EU_ConcoleFrame> EU_ConcoleFramePtr;

}

#endif