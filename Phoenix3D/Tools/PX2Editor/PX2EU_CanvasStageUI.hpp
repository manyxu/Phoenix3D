// PX2EU_CanvasStageUI.hpp

#ifndef PX2EU_CANVASSTAGEUI_HPP
#define PX2EU_CANVASSTATEUI_HPP

#include "PX2EditorPre.hpp"
#include "PX2UICanvas.hpp"
#include "PX2Polysegment.hpp"
#include "PX2UIObjectCtrl.hpp"
#include "PX2Singleton_NeedNew.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_CanvasStageUI : public UICanvas, public Singleton<EU_CanvasStageUI>
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_CanvasStageUI);

	public:
		EU_CanvasStageUI();
		virtual ~EU_CanvasStageUI();

		virtual void Enable(bool enable);

	public:
		virtual void OnEvent(Event *event);
		virtual void OnSizeChanged();

	protected:
		virtual void UpdateWorldData(double applicationTime,
			double elapsedTime);
		void _CreateGridGeometry();
		void _CreateUICanvas();
		void _UpdateProjectRange();
		void _AdjustCameraPercent();
		void SetOverCameraNode(CameraNode *cameraNode);

		NodePtr mStageCameraNodeRoot;
		CameraNodePtr mStageCameraNode;

		UICanvasPtr mGridCanvas;
		PX2::PolysegmentPtr mProjRangeSegment;
		PX2::NodePtr mRangeNode;

		UICanvasPtr mUICanvas;

		bool mIsInitSizeChanged;

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
		void _PanCamera(const float &horz, const float &vert);
		void _ZoomCamera(float zoom);
		void _ClickSelectPos(const APoint &scrPos);  //< 设置Pick点
		void _ClickSelectUI(const APoint &scrPos); //< 选择UI物件

		APoint mSelectPoint;

		bool mIsCameraDraging;
		APoint mStartDragingCursorPos;

		// edit menu
	public:
		void CreateMenuOnSelect(const APoint &pos);
	};

	PX2_REGISTER_STREAM(EU_CanvasStageUI);
	typedef PointerRef<EU_CanvasStageUI> EU_CanvasStageUIPtr;

}

#endif