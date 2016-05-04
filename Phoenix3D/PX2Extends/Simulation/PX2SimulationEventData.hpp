// PX2SimulationEventData.hpp

#ifndef PX2SIMULATIONEVENTDATA_HPP
#define PX2SIMULATIONEVENTDATA_HPP

#include "PX2SimulationPre.hpp"
#include "PX2APoint.hpp"
#include "PX2AVector.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM SimuInputData
	{
	public:
		SimuInputData();
		~SimuInputData();

		enum MouseTag
		{
			MT_NONE,
			MT_LEFT,
			MT_RIGHT,
			MT_MIDDLE,
			MT_MAX_TYPE
		};
		MouseTag TheMouseTag;

		APoint WorldPos;
		AVector MoveDelta;
		float Wheel;
	};

}

#endif