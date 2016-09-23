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

namespace PX2
{

	class PX2_ENGINE_ITEM UICanvas : public Canvas
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(UICanvas);

	public:
		UICanvas();
		virtual ~UICanvas();

		void SetNeedAdjustChildrenMask(bool isNeed);
		bool IsNeedAdjustChildrenMask() const;

	protected:
		virtual void OnChildAttached(Movable *child);
		virtual void OnChildDetach(Movable *child);
		void _AdjustChildrenMask();
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);

		bool mIsNeedAdjustChildrenMask;

	public:
		CameraNode *GetCameraNode();

		virtual void OnSizeChanged();

	protected:
		virtual void UpdateLeftBottomCornerOffset(Movable *parent);
		void _UpdateCamera();

		CameraPtr mCamera;
		CameraNodePtr mCameraNode;

	public_internal:
		void _AddPickWidget(UIFrame *widget);
		void _RemovePickWidget(UIFrame *widget);
		void _AddInRangePickWidget(UIFrame *widget);
		void _ClearInRangePickWidget();
		void _SortInRangePickWidget();

	protected:
		void _DoPick(const UIInputData &inputData);

		float mMoveAdjugeParam;
		float mMoveAdjugeParamSquare;
		std::list<UIFrame*> mPickWidgets;
		std::list<UIFrame*> mInRangeWidgets;

		// other
	protected:
		static float msUICameraY;

		// callbacks - general use for editor
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
	};

#include "PX2UICanvas.inl"
	PX2_REGISTER_STREAM(UICanvas);
	typedef PointerRef<UICanvas> UICanvasPtr;

}

#endif