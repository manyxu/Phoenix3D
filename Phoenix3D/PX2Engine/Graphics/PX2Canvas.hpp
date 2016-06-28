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
#include "PX2Polysegment.hpp"
#include "PX2RenderTarget.hpp"
#include "PX2WireProperty.hpp"
#include "PX2CanvasRenderBind.hpp"

namespace PX2
{

	class Renderer;
	class RenderWindow;

	class PX2_ENGINE_ITEM Canvas : public SizeNode
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

		virtual void UpdateWorldData(double applicationTime, 
			double elapsedTime);
		virtual void UpdateWorldBound();

		RenderWindow *mRenderWindow;
		bool mIsMain;
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
		void AddCamera(Camera *camera);
		void RemoveCamera(Camera *camera);
		void RemoveAllCameras();

		void SetNotDrawByParent(bool isNotDrawPyParent);
		bool IsNotDrawByParent() const;

	public_internal:
		virtual void OnGetVisibleSet(Culler& culler, bool noCull);

	protected:
		std::vector<Camera *> mCameras;
		std::map<Camera*, CullerPtr> mCullers;
		CanvasRenderBindPtr mCanvasRenderBind;

		WirePropertyPtr mOverrideWireProperty;

		// Draw
	public:
		void SetPriority(int priority); // 越大越在前
		int GetPriority() const;

		virtual void ClearVisibleSet();
		virtual void ComputeVisibleSet();
		virtual void Draw(Renderer *renderer);

		void SetAfterDrawClear(bool color, bool depth, bool stencil);
		void GetAfterDrawClear(bool &color, bool &depth, bool &stencil);

	public_internal:
		static bool LessThan(const Canvas *step0, const Canvas *step1);

	protected:
		int mPriority;

		// event
	public:
		virtual void DoExecute(Event *event);

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

		// Debug
	public:
		Polysegment *GetDebugLine();
		void ClearDebugLine();
		void AddDebugLine(const APoint &fromPos, const APoint &toPos,
			const Float4 &color);

	protected:
		VertexBufferPtr mDebugPolyVB;
		PointerRef<Polysegment> mDebugPoly;
	};

	PX2_REGISTER_STREAM(Canvas);
#include "PX2Canvas.inl"
	typedef PointerRef<Canvas> CanvasPtr;

}

#endif