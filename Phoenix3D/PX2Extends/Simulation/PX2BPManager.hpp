// PX2BPManager.hpp

#ifndef PX2BPMANAGER_HPP
#define PX2BPMANAGER_HPP

#include "PX2SimulationPre.hpp"
#include "PX2BPFile.hpp"
#include "PX2BPPackage.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2Polysegment.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM BPManager : public Singleton<BPManager>
	{
	public:
		BPManager();
		virtual ~BPManager();

		bool Initlize();

		// add gen funs
	public:
		void AddFun_General(const FunObject &funObj, const std::string &script);

		FunObject &BeginAddFun_General(const std::string &funName, const std::string &script);
		void AddInput(const std::string &name, FunParamType funParamType);
		void AddOutput(const std::string &name, FunParamType funParamType);
		void EndAddFun_General();

	public:
		// Events
		const std::vector<std::string> &GetEvents() const;
		FunObject *GetEvent(const std::string &name);

		// General Functions
		const FunObject *GetStartGF() const;
		const std::vector<std::string> &GetGFs() const;
		std::string GetGFScript(const std::string &name);
		FunObject *GetGF(const std::string &name);

		// Options
		const std::vector<std::string> &GetOptions() const;
		std::map<std::string, FunObject> &GetOptionMap();
		FunObject *GetOption(const std::string &name);

		// Params
		const std::vector<std::string> &GetParams() const;
		std::map<std::string, FunObject> &GetParamMap();
		FunObject *GetParam(const std::string &name);

		// Operators
		const std::vector<std::string> &GetOperators() const;
		std::map<std::string, FunObject> &GetOperatorMap();
		FunObject *GetOperator(const std::string &name);
		std::string GetOPScript(const std::string &name);

		// Compile
		const std::string &Compile(BPFile *file);
		int GetParamIndex();

		// Exe
		void Call(BPFile *file, bool alwaysCompile);
		void Call(BPPackage *group, bool alwaysCompile);
		void Call(const std::string &logicFilename);

		// Help
		bool CanLinkParam(PX2::BPParam *linkingParam, PX2::BPParam *linkedParam);
		PX2::Polysegment *CreatePoly(InterpCurveFloat *curve,
			const Float3 &color);
		void UpdatePoly(InterpCurveFloat *curve, PX2::Polysegment *p,
			const Float3 &color);

		// Operators
		AVector AVector_X_Float(const AVector &vec, float param);
		float Float_X_Float(float param0, float param1);

	protected:
		void _AddGenFun(const std::string &funName, const std::string &scriptStr);
		void _AddGenEvent(const std::string &eventName);
		void _AddOption(const std::string &optionFun);
		void _AddParam(const std::string &param);
		void _AddOPerator(const std::string &op, const std::string &opStr);

		FunObject mCurAddFunObj;

		int mParamIndex;

		// Events
		std::vector<std::string> mEvents;
		std::map<std::string, FunObject> mEventObjects;

		// General Fun
		std::vector<std::string> mGeneralFunctions;
		std::map<std::string, std::string> mGeneralFunctionMap;
		std::map<std::string, FunObject> mGenFunObjects;
		FunObject mFunStartObject;

		// Option
		std::vector<std::string> mOption;
		std::map<std::string, FunObject> mOptionObjects;

		// Param
		std::vector<std::string> mParam;
		std::map<std::string, FunObject> mParamObjects;

		// Operators
		std::vector<std::string> mOperators;
		std::map<std::string, std::string> mOperatorFunctionMap;
		std::map<std::string, FunObject> mOperatorsObjects;

	public:
		void SetReleasedBut(UIButton *but);
		UIButton *GetReleasedBut();

	public:
		UIButtonPtr mReleasedBut;
	};

#define PX2_BPM BPManager::GetSingleton()
#include "PX2BPManager.inl"

}

#endif