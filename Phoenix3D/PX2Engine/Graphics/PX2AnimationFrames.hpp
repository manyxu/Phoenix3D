// PX2AnimationFrames.hpp

#ifndef PX2ANIMATIONFRAMES_HPP
#define PX2ANIMATIONFRAMES_HPP

#include "PX2Animation.hpp"
#include "PX2FramesMesh.hpp"
#include "PX2SmartPointerRef.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM AnimationFrames : public Animation
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(AnimationFrames);

	public:
		AnimationFrames();
		virtual ~AnimationFrames();

		virtual void SetFilename(const std::string &filename);

		virtual void SetPlayOnce(bool once);

		virtual void OnPlay(ModelController *character);
		virtual bool IsPlaying() const;
		virtual void Stop();

		void SetWorldRunDir(const AVector &vec);
		const AVector &GetWorldRunDir() const;

	protected:
		FramesMeshPtr mFramesMesh;
		int mNumFramePerDir;

		NodePtr mAnimNode;
		AVector mWorldRunDir;
	};

#include "PX2AnimationFrames.inl"
	PX2_REGISTER_STREAM(AnimationFrames);
	typedef PointerRef<AnimationFrames> AnimationFramesPtr;

}

#endif