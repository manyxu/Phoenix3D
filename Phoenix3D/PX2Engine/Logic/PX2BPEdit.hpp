// PX2BPEdit.hpp

#ifndef PX2BPEDIT_HPP
#define PX2BPEDIT_HPP

#include "PX2LogicPre.hpp"
#include "PX2InterpCurve.hpp"
#include "PX2BPPackage.hpp"
#include "PX2Singleton_NeedNew.hpp"

namespace PX2
{

	class BPEdit : public Singleton<BPEdit>
	{
	public:
		BPEdit();
		~BPEdit();

		void Reset();

		void CompileBPFile(const std::string &filename, PX2::BPFile *file);

		enum LinkingState
		{
			LS_NONE,
			LS_LINKING,
			LS_LINKING_CANPUT
		};
		void SetLinkingState(LinkingState state);
		LinkingState GetLinkingState() const;

		void SetLinkingParam(BPParam *param);
		BPParam *GetLinkingParam();
		void SetLinkingWorldPos(const APoint &worldpos);
		const APoint &GetLinkingWorldPos() const;

		void SetLinkedParam(BPParam *linkedParam);
		PX2::BPParam *GetLinkedParam();

		void Disconnect(PX2::BPParam *lmParam);

	protected:
		LinkingState mLinkState;
		BPParamPtr mLinkingParam;
		BPParamPtr mLinkedParam;
		APoint mLinkingWorldPos;
	};

#define PX2_BPEDIT BPEdit::GetSingleton()

}

#endif