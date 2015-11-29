// PX2BPGroup.hpp

#ifndef PX2BPGROUP_HPP
#define PX2BPGROUP_HPP

#include "PX2BPModule.hpp"
#include "PX2UIPicBox.hpp"
#include "PX2UIText.hpp"

namespace PX2
{

	class BPFile;

	class PX2_EXTENDS_ITEM BPGroup : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(BPGroup);

	public:
		BPGroup();
		~BPGroup();

		void Enable(bool enable);
		bool IsEnable() const;

		void SetTip(const std::string &tip);
		const std::string &GetTip() const;

		virtual int AttachChild(Movable* child);
		virtual int DetachChild(Movable* child);

		bool IsHasLogicGroup(BPGroup *logicGroup);
		int GetNumLogicGroups() const;
		BPGroup *GetLogicGroup(int i);

		bool IsHasFile(BPFile *logicFile);
		int GetNumLogicFiles() const;
		BPFile *GetLogicFile(int i);

	public_internal:
		void Compile(std::string &script);

	protected:
		std::vector<Pointer0<BPGroup> > mLogicGroups;
		std::vector<Pointer0<BPFile> > mLogicFiles;
		std::string mTip;
		std::string mGeneratedFilename;

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

#include "PX2BPGroup.inl"
	PX2_REGISTER_STREAM(BPGroup);
	typedef Pointer0<BPGroup> BPGroupPtr;

}

#endif