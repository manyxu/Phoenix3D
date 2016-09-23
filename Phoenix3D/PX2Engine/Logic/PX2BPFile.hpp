// PX2BPFile.hpp

#ifndef PX2BPFILE_HPP
#define PX2BPFILE_HPP

#include "PX2BPModule.hpp"
#include "PX2UIPicBox.hpp"
#include "PX2UIFText.hpp"
#include "PX2BPModule.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM BPFile : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(BPFile);

	public:
		BPFile();
		virtual ~BPFile();

		void Enable(bool enable);
		bool IsEnable() const;

		void SetTip(const std::string &tip);
		const std::string &GetTip() const;

		virtual void OnChildAttached(Movable *child);
		virtual void OnChildDetach(Movable *child);

		bool IsHasBPModule(BPModule *logicModule);
		int GetNumBPModules() const;
		BPModule *GetBPModule(int i);

	public_internal:
		const std::string &Compile();
		const std::string &GetCompiledString() const;

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		void _PreCompileOne();
		void _UpdateModuleColorPos();

		bool mIsEnable;
		std::vector<BPModulePtr> mBPModules;
		BPModulePtr mStartModule;
		std::string mTip;
		std::string mCompiledString;

	public:
		virtual void OnSizeChanged();

	protected:
		UIFTextPtr mNameText;
		UIFTextPtr mTipText;
	};

#include "PX2BPFile.inl"
	PX2_REGISTER_STREAM(BPFile);
	typedef PointerRef<BPFile> BPFilePtr;

}

#endif