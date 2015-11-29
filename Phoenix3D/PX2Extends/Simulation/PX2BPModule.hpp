// PX2BPModule.hpp

#ifndef PX2BPMODULE_HPP
#define PX2BPMODULE_HPP

#include "PX2SimulationPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2XMLNode.hpp"
#include "PX2Any.hpp"
#include "PX2BPParam.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIPicBox.hpp"
#include "PX2UIText.hpp"
#include "PX2UIButton.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM BPModule : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(BPModule);

	public:
		// Logic
		enum ModuleType
		{
			MT_EVENT,
			MT_OBJECT,
			MT_FUNCTION_START,
			MT_FUNCTION_OBJECT,
			MT_FUNCTION_GENERAL,
			MT_PARAM,
			MT_FUNCTION_OPERATOR,
			MT_OPTION,
			MT_MAX_TYPE
		};
		BPModule(ModuleType type = MT_EVENT);
		virtual ~BPModule();

		ModuleType GetModuleType() const;

		void Enable(bool enable);
		bool IsEnable() const;

		// regist
		void RegistFunObj(const Object::FunObject &funObj);

		// Input Output
		const std::map<std::string, BPParam *> &GetInputParams() const;
		int GetNumInputs() const;
		const BPParam *GetOwnerObjectParam() const;

		const std::map<std::string, BPParam *> &GetOutputParams() const;
		int GetNumOutputs() const;

		// Compile
		void PrepareCompile();
		const std::string &GetModuleAs_ParamName() const;
		void PrepareCompile_SetNoCompile();

		void Compile(std::string &script, int numTable, bool isOriginalStart = false);

		void FunStartOnPreCompile(std::string &script);
		void FunEndOnAfterCompile(std::string &script);

		void SetCompiled(bool compiled);
		bool IsCompiled() const;

	public_internal:
		void OnDetach();
		void SetCompilingAsParam(bool isAsParam);
		bool IsCompilingAsParam() const;

	protected:
		void _ProcessOutputParams(std::string &script, int numTable);
		void _ProcessOperations(std::string &script);
		void _ProcessInputParams(std::string &script, int paramIndex0 = -1, int paramIndex1 = -1);
		void _ProcessInputParamsDepend(std::string &script, int numTable);
		void _UpdateModuleColorPos();

		ModuleType mModuleType;
		bool mIsEnable;

		std::vector<BPParamPtr> mInputParamsVec;
		std::map<std::string, BPParam*> mInputParams;
		BPParam *mOwnObjectParam;

		std::vector<BPParamPtr> mOutputParamsVec;
		std::map<std::string, BPParam*> mOutputParams;

		std::string mModuleAs_ParamName; // 当作为参数时的变量名称
		bool mIsCompilingAsParam_DoNotNeedCall;
		bool mIsCompiled;

		// UI
	public:
		void SetSize(const Sizef &size);
		const Sizef &GetSize() const;

		float GetItemHeight() const;
		float GetInOutButSize() const;

	public_internal:
		void UpdateCurve();

	protected:
		void _AdjustInOutPos();

		Sizef mSize;

		UIPicBoxPtr mBackPicBox;

		float mItemHeight;
		UITextPtr mClassText;
		UITextPtr mNameText;

		float mInOutButSize;
		BPParamPtr mActInBut;
		BPParamPtr mActOutBut;
		std::vector<BPParamPtr> mActOutButs_Operator;
		float mInOutStartPos;
	};

#include "PX2BPModule.inl"
	PX2_REGISTER_STREAM(BPModule);
	typedef Pointer0<BPModule> BPModulePtr;

}

#endif