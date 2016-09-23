// PX2LogicManager.hpp

#ifndef PX2LOGICMANAGER_HPP
#define PX2LOGICMANAGER_HPP

#include "PX2LogicPre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2LogicModule.hpp"
#include "PX2LogicModuleCatalogue.hpp"
#include "PX2UIBlueBlock.hpp"
#include "PX2InterpCurve.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM LogicManager : public Singleton<LogicManager>
	{
	public:
		LogicManager();
		virtual ~LogicManager();

		bool Initlize();
		bool InitlizeManyKitty();

		bool LoadPkgInfo(const std::string &filename);

		void AddModule(const std::string &cataName, LogicModule *lm);
		LogicModuleCatalogue *GetCatalogue(const std::string &cataName);

		std::map<std::string, LogicModuleCataloguePtr>& GetModuleCatalogues();

	protected:
		bool _InitlizeBP();

		std::map<std::string, LogicModuleCataloguePtr> mModuleCatalogues;

		// add gen funs
	public:
		void AddFun_General(const FunObject &funObj, const std::string &script);

		FunObject &BeginAddFun_General(const std::string &funName, const std::string &script);
		void AddInput(const std::string &name, FunParamType funParamType);
		void AddOutput(const std::string &name, FunParamType funParamType);
		void EndAddFun_General();

	public:
		// Events
		const std::vector<std::string> &GetEventClass() const;
		const std::map<std::string, std::vector<std::string> > &GetEvents() const;
		FunObject *GetEvent(const std::string &className, const std::string &name);

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

		// Operators
		AVector AVector_X_Float(const AVector &vec, float param);
		float Float_X_Float(float param0, float param1);

	protected:
		void _AddGenFun(const std::string &funName, const std::string &scriptStr);
		void _AddGenEvent(const std::string &className, const std::string &eventName);
		void _AddOption(const std::string &optionFun);
		void _AddParam(const std::string &param);
		void _AddOPerator(const std::string &op, const std::string &opStr);

		FunObject mCurAddFunObj;

		int mParamIndex;

		// Events
		std::vector<std::string> mEventClasses;
		std::map<std::string, std::vector<std::string> > mEvents;
		std::map<std::string, std::vector<FunObject> > mEventObjects;

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

		// BlueBlock
	public:
		void SetCatalogueSelectedBlueblock(UIBlueblock *blueBlock);
		UIBlueblock *GetCataLogueSelectedBlueblock();

	protected:
		UIBlueblockPtr mCataLogueSelectedBlueblock;
	};

#include "PX2LogicManager.inl"
#define  PX2_LOGICM LogicManager::GetSingleton()

}

#endif

