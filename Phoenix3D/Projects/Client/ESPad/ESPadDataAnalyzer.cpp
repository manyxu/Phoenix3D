// ESPadDataAnalyer.cpp

#include "ESPadDataAnalyzer.hpp"
#include "PX2Serial.hpp"
#include "PX2ScopedCS.hpp"
using namespace PX2;


std::string FeetDataAnalyzer::msPostureStr[BodyPosture::MAX_POSTURE] =
{
	"UNKOWN_BODY_POSE",
	"NOT_STAND_ONTO",
	"WALK",
	"RUN",
	"TIPTOE",
	"JUMP",
	"JUMP_WITH_LEFT",
	"JUMP_WITH_RIGHT",
	"LEAN_FRONT",
	"LEAN_BACK",
	"LEAN_LEFT",
	"LEAN_RIGHT",
	"STAND_STILL",
	"STAND_ON_LEFT",
	"STAND_ON_RIGHT",
	"TURN_LEFT",
	"TURN_RIGHT"
};
//----------------------------------------------------------------------------
FeetDataAnalyzer::FeetDataAnalyzer() :
mIsStopReadingData(true)
{
}
//----------------------------------------------------------------------------
FeetDataAnalyzer::~FeetDataAnalyzer()
{
	mIsStopReadingData = true;
	mRecvDataThread.Join();
}
//----------------------------------------------------------------------------
void FeetDataAnalyzer::Initlize(const std::string &comport_in, int baudrate)
{
	mIsStopReadingData = false;
	mPort = comport_in;
	mBaudrate = baudrate;

	mRecvDataThread.Start(*this);
}
//----------------------------------------------------------------------------
void FeetDataAnalyzer::Ternimate()
{
	mIsStopReadingData = true;
}
//----------------------------------------------------------------------------
void FeetDataAnalyzer::Run()
{
	Serial serial;
	bool initOK = serial.Init(mPort, mBaudrate);
	if (!initOK) return;

	RX_STATE data_state = RX_STATE::UNKNOWN;
	FeetData feetData;
	int dataIndex = 0;
	std::string val;

	while (!mIsStopReadingData)
	{
		char buffer[128] = { 0 };
		int size = serial.Receive(buffer, 128);

		for (int i = 0; i < size; i++)
		{
			if ((buffer[i]) == '[')
			{
				data_state = START;
				dataIndex = 0;
				memset(&feetData, 0, sizeof(feetData));
				val.clear();
			}
			else if ((buffer[i]) == ']')
			{
				//right foot last element
				feetData.right[dataIndex % 6] = atoi(val.c_str());
				data_state = END;
				PushData(feetData);
			}
			else
			{
				if (data_state == START)
				{
					if (buffer[i] == ',')
					{
						if (dataIndex < 6)
						{
							//left foot
							feetData.left[dataIndex] = atoi(val.c_str());
						}
						else
						{
							//right foot
							feetData.right[dataIndex % 6] = atoi(val.c_str());
						}
						val.clear();
						dataIndex++;
					}
					else
					{
						val += buffer[i];
					}
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
BodyPosture FeetDataAnalyzer::Analyze()
{
	mDataMutex.Enter();
	std::queue<FeetData> datas = mData;
	mDataMutex.Leave();

	if (datas.size() < ANALYZE_DATA_SIZE)
		return BodyPosture::UNKOWN_BODY_POSE;

	//from queue get 1s data, i.e. 10 data
	//convert data into bit-wise
	//use 6-bit of uchar
	std::vector<unsigned char> left_feet_data_queue;
	std::vector<unsigned char> right_feet_data_queue;
	int i, j;
	for (i = 0; i < ANALYZE_DATA_SIZE; i++)
	{
		if (!datas.empty())
		{
			unsigned char left_bitVal = 0;
			unsigned char right_bitVal = 0;
			FeetData data = datas.front();
			datas.pop();
			//left foot
			for (j = 5; j >= 0; j--)
			{
				if (data.left[j] > SENSOR_THRESHOLD_MIN)
				{
					left_bitVal += 1;
				}
				if (j != 0)
				{
					left_bitVal <<= 1;
				}
			}
			//right foot
			for (j = 5; j >= 0; j--)
			{
				if (data.right[j] > SENSOR_THRESHOLD_MIN)
				{
					right_bitVal += 1;
				}
				if (j != 0)
				{
					right_bitVal <<= 1;
				}
			}
			left_feet_data_queue.push_back(left_bitVal);
			right_feet_data_queue.push_back(right_bitVal);
		}
	}
	//now analyze the data
	//1.因为采样频率较快，存在连续相同的状态，故先合并连续相同的状态，如110011 -> 101
	std::vector<int> left_foot_poses;
	std::vector<int> right_foot_poses;
	int lastPose = -1;
	for (i = 0; i < (int)left_feet_data_queue.size(); i++)
	{
		FootPosture pose = GetFootPosture(left_feet_data_queue[i]);
		if (pose != lastPose)
		{
			left_foot_poses.push_back(pose);
			lastPose = pose;
		}
	}
	lastPose = -1;
	for (i = 0; i < (int)right_feet_data_queue.size(); i++)
	{
		FootPosture pose = GetFootPosture(right_feet_data_queue[i]);
		if (pose != lastPose)
		{
			right_foot_poses.push_back(pose);
			lastPose = pose;
		}
	}
	//2.现在分析左右脚状态.
	//2.1简单的状态分析
	if (left_foot_poses.size() == 1 && right_foot_poses.size() == 1)
	{
		if (left_foot_poses[0] == FOOT_HOVRE && right_foot_poses[0] == FOOT_HOVRE)
		{
			return NOT_STAND_ONTO;
		}
		if (left_foot_poses[0] == FOOT_STAND_FULLY && right_foot_poses[0] == FOOT_STAND_FULLY)
		{
			return STAND_STILL;
		}
		if (left_foot_poses[0] == FOOT_STAND_FULLY && right_foot_poses[0] == FOOT_HOVRE)
		{
			return STAND_ON_LEFT;
		}
		if (left_foot_poses[0] == FOOT_HOVRE && right_foot_poses[0] == FOOT_STAND_FULLY)
		{
			return STAND_ON_RIGHT;
		}
		if (left_foot_poses[0] == FOOT_LEAN_FRONT && right_foot_poses[0] == FOOT_STAND_FULLY)
		{
			return TURN_LEFT;
		}
		if (left_foot_poses[0] == FOOT_STAND_FULLY && right_foot_poses[0] == FOOT_LEAN_FRONT)
		{
			return TURN_RIGHT;
		}

		if (left_foot_poses[0] == FOOT_LEAN_FRONT && right_foot_poses[0] == FOOT_LEAN_FRONT)
		{
			return LEAN_FRONT;
		}
		if (left_foot_poses[0] == FOOT_LEAN_BACK && right_foot_poses[0] == FOOT_LEAN_BACK)
		{
			return LEAN_BACK;
		}
		if (left_foot_poses[0] == FOOT_LEAN_LEFT && right_foot_poses[0] == FOOT_LEAN_LEFT)
		{
			return LEAN_LEFT;
		}
		if (left_foot_poses[0] == FOOT_LEAN_RIGHT && right_foot_poses[0] == FOOT_LEAN_RIGHT)
		{
			return LEAN_RIGHT;
		}

	}
	else if (left_foot_poses.size() > 1 || right_foot_poses.size() > 1)
		//分析复杂的场景
		/*
		WALK,
		RUN,
		TIPTOE,
		JUMP,
		JUMP_WITH_LEFT,
		JUMP_WITH_RIGHT,
		*/
	{
		if (left_foot_poses.size() > 1 && right_foot_poses.size() == 1 && right_foot_poses[0] == FOOT_HOVRE)
		{
			return JUMP_WITH_LEFT;
		}
		else if (left_foot_poses.size() == 1 && left_foot_poses[0] == FOOT_HOVRE && right_foot_poses.size() > 1)
		{
			return JUMP_WITH_RIGHT;
		}
		else
		{
			int minSize = min(left_foot_poses.size(), right_foot_poses.size());
			int walkCount = 0;
			int jumpCount = 0;
			int tiptoeCount = 0;
			for (int i = 0; i < minSize; i++)
			{
				if ((left_foot_poses[i] == FOOT_HOVRE && right_foot_poses[i] == FOOT_STAND_FULLY)
					|| (left_foot_poses[i] == FOOT_STAND_FULLY && right_foot_poses[i] == FOOT_HOVRE)
					)
				{
					walkCount++;
				}
				if (
					((left_foot_poses[i] == FOOT_HOVRE && right_foot_poses[i] == FOOT_HOVRE)
					&& ((i + 1) < minSize) && (left_foot_poses[i + 1] == FOOT_STAND_FULLY && right_foot_poses[i + 1] == FOOT_STAND_FULLY))
					||
					((left_foot_poses[i] == FOOT_STAND_FULLY && right_foot_poses[i] == FOOT_STAND_FULLY)
					&& ((i + 1) < minSize) && (left_foot_poses[i + 1] == FOOT_STAND_FULLY && right_foot_poses[i + 1] == FOOT_HOVRE))
					)
				{
					jumpCount++;
				}
				if ((left_foot_poses[i] == FOOT_HOVRE && right_foot_poses[i] == FOOT_LEAN_FRONT)
					|| (left_foot_poses[i] == FOOT_LEAN_FRONT && right_foot_poses[i] == FOOT_HOVRE)
					)
				{
					tiptoeCount++;
				}

			}
			if (walkCount > 2)
			{
				return RUN;
			}
			else if (walkCount >= 2)
			{
				return WALK;
			}
			if (jumpCount >= 1)
			{
				return JUMP;
			}
			if (tiptoeCount >= 1)
			{
				return TIPTOE;
			}
		}
	}

	return UNKOWN_BODY_POSE;
}
//----------------------------------------------------------------------------
bool FeetDataAnalyzer::GetTopData(FeetData& data)
{
	mDataMutex.Enter();

	if (!mData.empty())
	{
		data = mData.front();
		mDataMutex.Leave();

		return true;
	}

	mDataMutex.Leave();

	return false;
}
//----------------------------------------------------------------------------
FootPosture FeetDataAnalyzer::GetFootPosture(unsigned char bitVal)
{
	if (bitVal >> FRONT_MIDDLE & 0x1)
	{
		return FOOT_STAND_FULLY;
	}

	if (bitVal & 0x1)
	{
		return FOOT_LEAN_FRONT;
	}

	if (((bitVal >> HEEL_LEFT & 0x1) || (bitVal >> HEEL_RIGHT & 0x1)) &&
		((bitVal >> FRONT_LEFT & 0x1) == 0)
		&& ((bitVal >> FRONT_MIDDLE & 0x1) == 0) && ((bitVal >> FRONT_RIGHT & 0x1) == 0))
	{
		return FOOT_LEAN_BACK;
	}

	if ((bitVal >> FRONT_LEFT & 0x1) && (bitVal >> HEEL_LEFT & 0x1) &&
		((bitVal >> FRONT_RIGHT & 0x1) == 0))
	{
		return FOOT_LEAN_LEFT;
	}

	if ((bitVal >> FRONT_RIGHT & 0x1) && (bitVal >> HEEL_RIGHT & 0x1) && 
		((bitVal >> FRONT_LEFT & 0x1) == 0))
	{
		return FOOT_LEAN_RIGHT;
	}

	return FOOT_HOVRE;
}
//----------------------------------------------------------------------------
void FeetDataAnalyzer::PushData(FeetData data)
{
	ScopedCS cs(&mDataMutex);

	if (mData.size() > ANALYZE_DATA_SIZE) // only save 2s data
	{
		mData.pop();
	}
	mData.push(data);
}
//----------------------------------------------------------------------------