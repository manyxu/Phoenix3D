// PX2ModelController.hpp

#ifndef PX2MODELCONTROLLER_HPP
#define PX2MODELCONTROLLER_HPP

#include "PX2Controller.hpp"
#include "PX2Animation.hpp"
#include "PX2FString.hpp"
#include "PX2BlendTransformController.hpp"
#include "PX2AnimationObject.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ModelController : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(ModelController);

	public:
		ModelController();
		virtual ~ModelController();

	protected:
		virtual void _Update(double applicationTime, double elapsedTime);
		void _AnimationBlend();

		// Movable
	public:
		virtual void SetMovableFilename(const std::string &filename,
			bool shareVI = false);
		const std::string &GetMovableFilename() const;
		bool IsShareVI() const;

		virtual void SetMovable(Movable *mov);
		Movable *GetMovable();

		virtual void SetHeading(const AVector &heading);
		const AVector &GetHeading() const;

		void SetMovableUseAutoWorldBound(bool isUseAutoWorldBound);
		bool IsMovableUseAutoWorldBound() const;
		void SetMovableAutoWorldBoundRadius(float radius);
		float GetMovableAutoWorldBoundRadius() const;

	protected:
		void _CalSkins();
		void _CalAnimNode(Movable *mov);
		virtual void SetControlledable(Controlledable* object);

		bool mIsShareVI;
		std::string mMovableFilename;
		MovablePtr mMovable;

		bool mIsMovableAutoWorldBound;
		float mMovableAutoWorldBoundRadius;

		AVector mHeading;

		// Anchor
	public:
		void CollectAnchors();

		APoint GetAnchorPos(int id);
		Node *GetAnchor(int id);

	protected:
		void _CollectAnchor(Movable *mov);

		std::map<int, Node*> mAnchorMap;

		// anim
	public:
		// set it before add anim
		enum AnimType
		{
			AT_NONE,
			AT_FRAMES,
			AT_SKELETON,
			AT_MAX_TYPE
		};
		void SetAnimType(AnimType type);
		AnimType GetAnimType() const;

		bool AddAnim(Animation *anim);
		bool HasAnim(Animation *anim);
		bool HasAnim(int id);
		bool RemoveAnim(Animation *anim);
		bool RemoveAnim(int id);
		void RemoveAllAnims();
		int GetNumAnims() const;
		std::map<int, AnimationPtr> &GetAnimsMap();
		Animation *GetAnimByID(int id);

		void SetDefaultAnimID(int id);
		int GetDefaultAnimID() const;
		Animation *GetDefaultAnim();

		virtual void PlayAnim(Animation *anim);
		virtual void PlayAnimByID(int id);
		virtual void StopAnim(Animation *anim);
		virtual void StopCurAnim();
		virtual void PlayCurAnim();

		Animation *GetCurPlayingAnim();
		bool IsHasAnimPlaying() const;

	protected:
		AnimType mAnimType;

		// Frames

		// Skeleton
		std::map<int, AnimationPtr> mAnimsMap;
		AnimationPtr mCurPlayingAnim;
		int mDefaultAnimID;
		AnimationPtr mDefaultAnim;

		std::vector<MovablePtr> mSkinMovables;
		MovablePtr mModelAnimMovable;
		std::map<FString, BlendTransformController*> mBTCMap;
		AnimationObjectPtr mLastAnimObj;
	};
#include "PX2ModelController.inl"

}

#endif