// ESPadDataAnalyzer.hpp

#ifndef ESPADDATAANALYER_HPP
#define ESPADDATAANALYER_HPP

#include "PX2CorePre.hpp"
#include "PX2Thread.hpp"
#include "PX2Runnable.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{
	
	const int SENSOR_THRESHOLD_MIN = 100;
	const int ANALYZE_DATA_SIZE = 10;

	enum SensorPosition
	{
		TIP_TOE,
		FRONT_LEFT,
		FRONT_MIDDLE,
		FRONT_RIGHT,
		HEEL_LEFT,
		HEEL_RIGHT
	};

	enum RX_STATE
	{
		UNKNOWN,
		START,
		END
	};

	enum BodyPosture
	{
		UNKOWN_BODY_POSE = 0,
		NOT_STAND_ONTO, //没有踩上板子
		WALK,
		RUN,
		TIPTOE,
		JUMP,
		JUMP_WITH_LEFT,
		JUMP_WITH_RIGHT,
		LEAN_FRONT,
		LEAN_BACK,
		LEAN_LEFT,
		LEAN_RIGHT,
		STAND_STILL,
		STAND_ON_LEFT,
		STAND_ON_RIGHT,
		TURN_LEFT,
		TURN_RIGHT,
		MAX_POSTURE
	};

	enum FootPosture
	{
		UNKNOWN_FOOT_POS,
		FOOT_HOVRE, //悬空 all = 1
		FOOT_STAND_FULLY, //双脚完全着地 1,2,3 = 1
		FOOT_LEAN_FRONT, //脚尖着地 0 = 1
		FOOT_LEAN_BACK, //脚跟着地 4,5 = 1
		FOOT_LEAN_LEFT, //左侧着地 1,4 = 1
		FOOT_LEAN_RIGHT,//右侧着地 3,5 = 1
	};

	typedef struct _feetdata
	{
		int left[6];
		int right[6];
	}FeetData;


	class FeetDataAnalyzer : public Runnable
	{
	public:
		FeetDataAnalyzer();
		virtual ~FeetDataAnalyzer();

		void Initlize(const std::string &comport_in, int baudrate);
		void Ternimate();

		virtual void Run();

		BodyPosture Analyze();
		bool GetTopData(FeetData& data);

		static std::string msPostureStr[BodyPosture::MAX_POSTURE];

	private:
		FootPosture GetFootPosture(unsigned char bitVal);
		void PushData(FeetData data);

	private:
		std::string mPort;
		int mBaudrate;

		Mutex mDataMutex;
		std::queue<FeetData> mData;

		bool mIsStopReadingData;
		Thread mRecvDataThread;
	};
	typedef Pointer0<FeetDataAnalyzer> FeetDataAnalyzerPtr;

}

#endif