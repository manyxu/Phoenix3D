// PX2Timer.hpp

#ifndef PX2TIMER_HPP
#define PX2TIMER_HPP

#include "PX2CorePre.hpp"
#include "PX2SmartPointerRef.hpp"
#include "PX2Timestamp.hpp"

namespace PX2
{
	
	class Object;

	class PX2_ENGINE_ITEM Timer
	{
	public:
		Timer ();
		virtual ~Timer ();

		// time_t 是日历时间
		virtual bool OnTimer (double appTime) = 0;

public_internal:
		int mTimerID;
		float StartTime;
		double mInterval; // 秒
		void *Action;
		void *UserData;
	};
	typedef PointerRef<Timer> TimerPtr;

}

#endif