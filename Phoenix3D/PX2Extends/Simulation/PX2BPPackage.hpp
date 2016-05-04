// PX2BPPackage.hpp

#ifndef PX2BPPACKAGE_HPP
#define PX2BPPACKAGE_HPP

#include "PX2BPModule.hpp"
#include "PX2UIPicBox.hpp"
#include "PX2UIText.hpp"
#include "PX2BPFile.hpp"
#include "PX2UIFText.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM BPPackage : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(BPPackage);

	public:
		BPPackage();
		virtual ~BPPackage();

		void Enable(bool enable);
		bool IsEnable() const;

		void SetTip(const std::string &tip);
		const std::string &GetTip() const;

		virtual void OnChildAttached(Movable *child);
		virtual void OnChildDetach(Movable *child);

		bool IsHasBPPackage(BPPackage *logicGroup);
		int GetNumBPPackages() const;
		BPPackage *GetBPPackage(int i);

		bool IsHasFile(BPFile *logicFile);
		int GetNumBPFiles() const;
		BPFile *GetBPFile(int i);

	public_internal:
		void Compile(std::string &script);

	protected:
		std::vector<PointerRef<BPPackage> > mBPPackages;
		std::vector<PointerRef<BPFile> > mBPFiles;
		std::string mTip;
		std::string mGeneratedFilename;

	protected:
		UIFTextPtr mNameText;
		UIFTextPtr mTipText;
	};

#include "PX2BPPackage.inl"
	PX2_REGISTER_STREAM(BPPackage);
	typedef PointerRef<BPPackage> BPPackagePtr;

}

#endif