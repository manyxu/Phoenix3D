// PX2BPParam.hpp

#ifndef PX2BPPARAM_HPP
#define PX2BPPARAM_HPP

#include "PX2SimulationPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIButton.hpp"

namespace PX2
{

	class BPModule;

	class PX2_EXTENDS_ITEM BPParam : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(BPParam);

	public:
		BPParam(bool isIn = true, bool isExe = false);
		virtual ~BPParam();

		bool IsIn() const;
		bool IsExe() const;

		void SetDataType(FunParamType type);
		Object::FunParamType GetDataType() const;
		bool IsOwnObjectParam() const;

		void SetValue(const Any &any);
		const Any &GetValue() const;
		bool GetValueB() const;
		int GetValueI() const;
		float GetValueF() const;
		APoint GetValueAPoint() const;
		AVector GetValueAVector() const;
		std::string GetValueStr() const;
		Object *GetValuePointer() const;

		std::string GetValueScriptStr();

		// OutStr
		const std::string &GetOutputScriptVarString() const;

		BPModule *GetModule();
		const BPModule *GetModule() const;

		void SetLinkingWorldPos(const APoint &worldPos);
		void SetLinkingColor(const Float3 &color);
		void DisLinking();

	public_internal:
		void PrepareCompile();
		void SetModule(BPModule *module);

		bool IsHasLinkToParam(BPParam *param);
		bool AddLinkToParam(BPParam *param);
		bool RemoveLinkToParam(BPParam *param);
		void RemoveAllLinkToParams();
		int GetNumLinkToParams() const;
		BPParam *GetLinkToParam(int index);
		std::vector<BPParam *> GetSortedParams();
		static bool SortFun(const BPParam *param0, const BPParam *param1);

		bool AddLinkMeParam(BPParam *param);
		bool IsHasLinkMeParam(BPParam *param) const;
		bool RemoveLinkMeParam(BPParam *param);
		void RemoveAllLinkMeParams();
		int GetNumLinkMeParams() const;
		std::vector<BPParam *> &GetLinkMeParams();
		BPParam *GetLinkMeParam(int i);

		void UpdateCurve();

	protected:
		bool mIsIn;
		bool mIsExe;
		Object::FunParamType mDataType;
		Any mData;
		std::string mOutputScriptVarString;

		BPModule *mModule;

		struct ParamObj
		{
			ParamObj() { TheParam = 0; }
			~ParamObj() {}

			BPParam *TheParam;
			InterpCurveFloat TheCurve;
			PolysegmentPtr ThePolysegment;
		};
		std::vector<ParamObj> mLinkToParamVec;
		std::vector<BPParam *> mLinkMeParams;

		// UI
	protected:
		UIButtonPtr mBut;
		UITextPtr mNameText;
		InterpCurveFloat mCurveLinking;
		PolysegmentPtr mPolysegmentLinking;

		Float3 mLinkingColor;
		APoint mLinkingWorldPos;
	};

	PX2_REGISTER_STREAM(BPParam);
	typedef Pointer0<BPParam> BPParamPtr;
#include "PX2BPParam.inl"

}

#endif