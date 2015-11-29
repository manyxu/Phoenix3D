// PX2UICanvas.hpp

#ifndef PX2UICANVAS_HPP
#define PX2UICANVAS_HPP

#include "PX2UIPre.hpp"
#include "PX2Object.hpp"
#include "PX2Size.hpp"
#include "PX2Canvas.hpp"
#include "PX2CameraNode.hpp"
#include "PX2Renderable.hpp"
#include "PX2UIDefine.hpp"
#include "PX2UIButton.hpp"
#include "PX2UICanvasController.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM UICanvas : public Canvas
	{
		PX2_DECLARE_RTTI;

	public:
		UICanvas(int viewID);
		~UICanvas();

		int GetViewID() const;

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);

		int mViewID;

		// Draw;
	public:
		virtual void Draw();

		// Node
	public:
		void SetSuperTopMoveable(Movable *movable);
		Movable *GetSuperTopMovbale();
		void PushTopMovable(Movable *movable);
		void PopTopMovable();

		Movable *GetTopestMovable();

	protected:
		MovablePtr mSuperTopMovable;
		std::deque<MovablePtr> mTopMovables;

	public:
		CameraNode *GetCameraNode();
		void TransCameraNode(const AVector &trans);
		void ScaleCameraFrustum(float addScaleValue);

		virtual void OnSizeChanged();

	protected:
		CameraNodePtr mCameraNode;
		bool mIsInittedRefreshCameraSizePos;
		float mScaleCameraFrustumValue;

		// Pick
	public:
		void SetPickAcceptRect(const Rectf &rect);
		const Rectf &GetPickAcceptRect() const;

		void SetNotPickedCallback(NotPickedCallback callback);

		std::vector<RenderablePtr> &GetPickedRenderables();

		bool IsPressed() const;
		const APoint &GetCurPickPos() const;
		const APoint &GetPressedPos() const;
		const APoint &GetReleasedPos() const;

	public_internal:
		std::set<UIFramePtr > mPickedFrames;

	protected:
		void _DoPick(float x, float z, int pickInfo,
			std::vector<RenderablePtr> &vec);
		void _CollectFrames(Movable *mov, std::set<UIFramePtr> &frames);

		Rectf mPickAcceptRect;

		float mMoveAdjugeParam;
		float mMoveAdjugeParamSquare;

		bool mIsPressed;
		APoint mCurPickPos;
		APoint mPressedPos;
		APoint mReleasedPos;

		std::vector<RenderablePtr> mPickedRenderables;
		NotPickedCallback mNotPickedCallback;

		// general
	protected:
		static float msUICameraY;

		// public
	public:
		virtual void DoExecute(Event *event);

		// parent frame
	public:
		UIFrame *GetParentUIFrame();

	public_internal:
		void SetParentUIFrame(UIFrame *parentUIFrame);

	protected:
		UIFrame *mParentUIFrame;

		// uictrl
	public:
		UICanvasController *CreateAndAddCanvasController();
		UICanvasController *GetCanvasController();

	protected:
		friend UICanvasController;

		UICanvasControllerPtr mCanvasController;
	};

#include "PX2UICanvas.inl"
	typedef Pointer0<UICanvas> UICanvasPtr;

}

#endif