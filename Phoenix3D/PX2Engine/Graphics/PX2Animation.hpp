// PX2Animation.hpp

#ifndef PX2ANIMATION_HPP
#define PX2ANIMATION_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"
#include "PX2Node.hpp"

namespace PX2
{

	class ModelController;

	class PX2_ENGINE_ITEM Animation : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(Animation);

	public:
		enum AnimType
		{
			AT_2DFRAMES,
			AT_2DSKELETON,
			AT_3DSKELETON,
			AT_MAX_TYPE
		};
		Animation();
		Animation(AnimType type);
		~Animation();

		AnimType GetAnimType() const;

		void Update(float appSeconds, float elapsedSeconds);

		void SetFilename(const std::string &filename);
		const std::string &GetFilename() const;

		void SetNormalTime(float time);
		float GetNormalTime() const;

		void SetTime(float time);
		float GetTime() const;

		void SetFrequency(float frequence);
		float GetFrequency() const;

		void SetPlayOnce(bool once);
		bool IsPlayOnce() const;
		void SetPlayedKeepPlaying(bool isPlayedKeepPlaying);
		bool IsPlayedKeepPlaying() const;

		// 是否使用动画融合
		void SetUseBlend(bool useBlend);
		bool IsUseBlend() const;

		void LetCharacterPlay();
		virtual void OnPlay(ModelController *actor);
		virtual bool IsPlaying() const;
		virtual void Stop();

		ModelController *GetCharacter();
		virtual void OnRemove(ModelController *character);

	public_internal:
		virtual void SetCharacter(ModelController *character);

	protected:
		ModelController *mCharacter;

		AnimType mAnimType;
		std::string mFilename;
		float mAnimNormalTime;
		float mAnimTime;
		float mFrequency;
		bool mIsPlayOnce;
		bool mIsPlayedKeepPlaying;
		bool mIsUseBlend;

		bool mIsPlaying;
		float mPlayedTime;
	};

#include "PX2Animation.inl"
	typedef PointerRef<Animation> AnimationPtr;

}

#endif