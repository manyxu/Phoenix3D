// PX2BPFile.hpp

#ifndef PX2BPFILE_HPP
#define PX2BPFILE_HPP

#include "PX2BPModule.hpp"
#include "PX2UIPicBox.hpp"
#include "PX2UIText.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM BPFile : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(BPFile);

	public:
		BPFile();
		~BPFile();

		void Enable(bool enable);
		bool IsEnable() const;

		void SetTip(const std::string &tip);
		const std::string &GetTip() const;

		virtual int AttachChild(Movable* child);
		virtual int DetachChild(Movable* child);

		bool IsHasLogicModule(BPModule *logicModule);
		int GetNumLogicModules() const;
		BPModule *GetLogicModule(int i);

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

		// UI
	public:
		void SetSize(const Sizef &size);
		const Sizef &GetSize() const;

	protected:
		Sizef mSize;
		UIPicBoxPtr mBackPicBox;
		UITextPtr mNameText;
		UITextPtr mTipText;
	};

#include "PX2BPFile.inl"
	PX2_REGISTER_STREAM(BPFile);
	typedef Pointer0<BPFile> LogicFilePtr;

}

#endif