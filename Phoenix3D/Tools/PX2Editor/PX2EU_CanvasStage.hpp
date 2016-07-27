// PX2EU_CanvasStage.hpp

#ifndef PX2EU_CANVASSTAGE_HPP
#define PX2EU_CANVASSTATE_HPP

#include "PX2EditorPre.hpp"
#include "PX2UICanvas.hpp"
#include "PX2Polysegment.hpp"
#include "PX2UIObjectCtrl.hpp"
#include "PX2RedoUndo.hpp"
#include "PX2SceneNodeCtrl.hpp"
#include "PX2EditDefine.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_CanvasStage : public UICanvas, public Singleton<EU_CanvasStage>
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_CanvasStage);

	public:
		EU_CanvasStage();
		virtual ~EU_CanvasStage();

		enum ViewType
		{
			VT_TOP,
			VT_FRONT,
			VT_LEFT,
			VT_PERSPECTIVE,
			VT_MAX_TYPE
		};
		void SetViewType(ViewType viewType);
		ViewType GetViewType();

		enum ViewDetail
		{
			VD_WIREFRAME,
			VD_SOLID,
			VD_TEXTURED
		};
		void SetViewDetail(ViewDetail viewDetail);
		ViewDetail GetViewDetail() const;

		virtual void Enable(bool enable);

	public:
		virtual void OnEvent(Event *event);
		virtual void OnSizeChanged();

	protected:
		void SetOverCameraNode(CameraNode *cameraNode);
		CameraNode *GetOverCameraNode();
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);

		CameraNodePtr mStageCameraNode;

		UICanvasPtr mGridCanvas;
		RenderablePtr mGrid;
		NodePtr mGridNode;

		UICanvasPtr mSceneCanvas;

		UICanvasPtr mSceneNodeCtrlCanvas;
		SceneNodeCtrlPtr mSceneNodeCtrl;
		BoundCtrlPtr mBoundCtrl;
		NodePtr mSceneCtrlNode;

		ViewType mViewType;
		ViewDetail mViewDetail;

	public:
		virtual void OnLeftDown(const APoint &worldPos);
		virtual void OnLeftUp(const APoint &worldPos);
		virtual void OnLeftDClick(const APoint &worldPos);
		virtual void OnMiddleDown(const APoint &worldPos);
		virtual void OnMiddleUp(const APoint &worldPos);
		virtual void OnMouseWheel(const APoint &worldPos, float delta);
		virtual void OnRightDown(const APoint &worldPos);
		virtual void OnRightUp(const APoint &worldPos);
		virtual void OnMotion(const APoint &worldPos);

		void SetCameraDraging(bool drag);
		bool IsCameraDraging() const;

	protected:
		virtual std::pair<float, float> CalPixelToWorld();

	protected:
		void _CreateSceneCanvas();
		void _CreateGridGeometry();
		void _CreateNodeCtrl();
		void _ProcessKeyMove(bool isPress, float cameraMoveSpeed, float elapsedTime, float &speed);
		void _MoveCamera(float horz, float vert); //< 透视角度，沿着视线方向移动
		void _PanCamera(const float &horz, const float &vert); //< 透视角度，上下左右移动，其他视角和MoveCamera一样
		void _ZoomCamera(float zoom);
		void _RolateCamera(float horz, float vert);
		void _RoundCamera(float horz, float vert);
		void _ClickSelectPos(const APoint &scrPos);
		void _ClickSelectScene(const APoint &scrPos);

		float mCurCameraMoveSpeed_W;
		float mCurCameraMoveSpeed_S;
		float mCurCameraMoveSpeed_A;
		float mCurCameraMoveSpeed_D;

		APoint mSelectPoint;
		bool mIsSelectMovableTransChanged;
		URDoPtr mURDoCommand;

		bool mIsCameraDraging;
		APoint mStartDragingCursorPos;

		// edit menu
	public:
		void CreateMenuOnSelect(const APoint &pos);
	};

	PX2_REGISTER_STREAM(EU_CanvasStage);
	typedef PointerRef<EU_CanvasStage> EU_CanvasStagePtr;

}

#endif