// PX2Canvas.hpp

#ifndef PX2CANVAS_HPP
#define PX2CANVAS_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"
#include "PX2SizeNode.hpp"
#include "PX2Rect.hpp"
#include "PX2Culler.hpp"
#include "PX2Node.hpp"
#include "PX2Vector2.hpp"
#include "PX2RenderTarget.hpp"
#include "PX2WireProperty.hpp"
#include "PX2CanvasRenderBind.hpp"

namespace PX2
{

	class Renderer;
	class RenderWindow;

	class PX2_FOUNDATION_ITEM Canvas : public SizeNode
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(Canvas);

	public:
		Canvas();
		virtual ~Canvas();

		void SetMain(bool main);
		bool IsMain() const;

		void SetRenderNode(SizeNode *movable);
		SizeNode *GetRenderNode();

	public_internal:
		void SetRenderWindow(RenderWindow *rw);
		RenderWindow *GetRenderWindow();

	protected:
		virtual void OnSizeChanged();
		
		virtual void OnBeAttached();
		virtual void OnBeDetach();

		virtual void UpdateWorldData(double applicationTime, 
			double elapsedTime);
		virtual void UpdateWorldBound();

		RenderWindow *mRenderWindow;
		bool mIsMain;
		CameraPtr mUpdateBeforeCamera;
		SizeNode *mRenderNode;

		// ViewPort
	public:
		void SetSizeChangeReAdjustCamera(bool doReAdjust);
		virtual void SetScreenSize(const Sizef &size);
		const Sizef &GetScreenSize() const;

		void SetViewPort(const Rectf &viewPort);
		void SetViewPort(float left, float bottom, float width, float height);
		const Rectf &GetViewPort() const;

	protected:
		bool mIsSizeChangeReAdjustCamera;
		Sizef mScreenSize;
		Rectf mViewPort;

		// Renderer Camera Culler
	public:
		virtual void SetCamera(Camera *camera);
		Camera *GetCamera();

		Culler &GetCuller();
		void SetNotDrawByParent(bool isNotDrawPyParent);
		bool IsNotDrawByParent() const;

		bool GetPickRay(float viewPortPosX, float viewPortPosZ,
			APoint& origin, AVector& direction);

		Vector2f WorldPos3DToViewPort(const APoint &worldPos, 
			bool *isInBack=0);

		AVector ScreenVectorToCamera(const AVector &screenVec);

	public_internal:
		virtual void OnGetVisibleSet(Culler& culler, bool noCull);

	protected:
		CameraPtr mCamera;
		Culler mCuller;
		CanvasRenderBindPtr mCanvasRenderBind;

		WirePropertyPtr mOverrideWireProperty;

		// Draw
	public:
		void SetPriority(int priority); // 越大越在前
		int GetPriority() const;

		void SetBeforeDrawClear(bool color, bool depth, bool stencil);
		void GetBeforeDrawClear(bool &color, bool &depth, bool &stencil);
		void SetClearColor(const Float4 &clearColor);
		const Float4 &GetClearColor() const;

		virtual void ClearVisibleSet();
		virtual void ComputeVisibleSetAndEnv();
		virtual void Draw(Renderer *renderer);

		void SetAfterDrawClear(bool color, bool depth, bool stencil);
		void GetAfterDrawClear(bool &color, bool &depth, bool &stencil);

	public_internal:
		static bool LessThan(const Canvas *step0, const Canvas *step1);

	protected:
		int mPriority;
		bool mBeforeDoClearColor;
		bool mBeforeDoClearDepth;
		bool mBeforeDoClearStencil;
		bool mAfterDoClearColor;
		bool mAfterDoClearDepth;
		bool mAfterDoClearStencil;
		Float4 mClearColor;

		// pick
	public:
		void SetPickOnlyInSizeRange(bool inRange);
		bool IsPickOnlyInSizeRange() const;

		virtual void OnLeftDown(const APoint &worldPos);
		virtual void OnLeftUp(const APoint &worldPos);
		virtual void OnLeftDClick(const APoint &worldPos);
		virtual void OnMiddleDown(const APoint &worldPos);
		virtual void OnMiddleUp(const APoint &worldPos);
		virtual void OnMouseWheel(const APoint &worldPos, float delta);
		virtual void OnRightDown(const APoint &worldPos);
		virtual void OnRightUp(const APoint &worldPos);
		virtual void OnMotion(const APoint &worldPos);

		bool IsMoved() const;
		bool IsLeftPressed() const;
		bool IsRightPressed() const;
		bool IsMiddlePressed() const;
		const APoint &GetCurPickPos() const;
		const AVector &GetMoveDelta() const;

	protected:
		bool mIsPickOnlyInSizeRange;
		bool mIsInRange;

		bool mIsPressed;
		APoint mCurPickPos;
		APoint mLastPickPos;
		APoint mPressedPos;
		APoint mReleasedPos;
		bool mIsMoved;
		AVector mMoveDelta;

		bool mIsLeftPressed;
		APoint mLeftPressedPos;
		APoint mLeftReleasedPos;

		bool mIsRightPressed;
		APoint mRightPressedPos;
		APoint mRightReleasedPos;

		bool mIsMiddlePressed;
		APoint mMiddlePressedPos;
		APoint mMiddleReleasedPos;

		// Help
	public:
		std::pair<float, float> CalPixelToWorld();

	protected:
		std::pair<float, float> mPixelToWorld;
	};

	PX2_REGISTER_STREAM(Canvas);
	typedef PointerRef<Canvas> CanvasPtr;

#include "PX2Canvas.inl"
	typedef PointerRef<Canvas> CanvasPtr;

}

#endif