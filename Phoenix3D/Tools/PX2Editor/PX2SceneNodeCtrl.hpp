// PX2SceneNodeCtrl.hpp

#ifndef PX2SCENENODECTRLS_HPP
#define PX2SCENENODECTRLS_HPP

#include "PX2EditorPre.hpp"
#include "PX2EventHandler.hpp"
#include "PX2Node.hpp"
#include "PX2SwitchNode.hpp"
#include "PX2Polysegment.hpp"

namespace PX2
{

	class Renderer;
	class Canvas;

	class PX2_EDITOR_ITEM SceneNodeCtrl : public EventHandler
	{
	public:
		SceneNodeCtrl();
		virtual ~SceneNodeCtrl();

		enum AxisMode
		{
			AM_WORLD,
			AM_LOCAL,
			AM_PARENT,
			AM_MAX_MODE
		};
		void SetAxisMode(AxisMode mode);
		AxisMode GetAxisMode() { return mAxisMode; }

		enum LookType
		{
			LT_TOP,
			LT_FRONT,
			LT_LEFT,
			LT_PERSPECTIVE
		};
		void SetLookType(LookType type);
		LookType GetLookType() { return mLookType; }

		enum CtrlType
		{
			CT_SELECT,
			CT_TRANSLATE,
			CT_ROLATE,
			CT_SCALE
		};
		void SetCtrlType(CtrlType type);
		CtrlType GetCtrlType() { return mCtrlType; }

		enum DragType
		{
			DT_NONE,
			DT_X,
			DT_Y,
			DT_Z,
			DT_XY,
			DT_XZ,
			DT_YZ,
			DT_XYZ
		};
		void SetDragType(DragType type);
		DragType GetDragType() { return mDragType; }

		void OnLeftDown(Camera *camera, const APoint &pos, const Sizef &size);
		void OnLeftUp(Camera *camera, const APoint &pos, const Sizef &size);
		void OnMouseWheel(Camera *camera, float wheelDelta, const Sizef &size);
		void OnMotion(bool leftDown, Camera *camera,
			APoint posNow, APoint posBefore, const Sizef &size);

		Node *GetCtrlsGroup() { return mCtrlsGroup; }
		Movable *GetCurrentCtrlX();
		Movable *GetCurrentCtrlY();
		Movable *GetCurrentCtrlZ();
		Movable *GetCurrentCtrlXYZ();
		Movable *GetCurrentCtrlXY();
		Movable *GetCurrentCtrlYZ();
		Movable *GetCurrentCtrlXZ();

		APoint GetPosition();
		AVector GetXDir();
		AVector GetYDir();
		AVector GetZDir();

		// PX2 Event
		virtual void OnEvent(Event *event);

		void UpdateCtrlTrans();

	private:
		void UpdateCtrl();
		SceneNodeCtrl::DragType GetDragType(Camera *camera,
			const APoint &viewPortPos, const Sizef &size);

		AxisMode mAxisMode;
		AVector mDirX;
		AVector mDirY;
		AVector mDirZ;
		HMatrix mParentRotateMat;

		LookType mLookType;
		CtrlType mCtrlType;
		NodePtr mCtrl_P_Translate;
		NodePtr mCtrl_P_Rolate;
		NodePtr mCtrl_P_Scale;
		NodePtr mCtrl_O_Translate;
		NodePtr mCtrl_O_Rolate;
		NodePtr mCtrl_O_Scale;
		NodePtr mCtrl_Bound;
		SwitchNodePtr mCtrlsGroup;
		float mOriginScale;
		DragType mDragType;
	};
	typedef Pointer0<SceneNodeCtrl> SceneNodeCtrlPtr;

	class BoundCtrl : public EventHandler
	{
	public:
		BoundCtrl();
		virtual ~BoundCtrl();

		Node *GetCtrlsGroup() { return mCtrlsGroup; }

		virtual void OnEvent(Event *event);

		void UpdateCtrl();

	protected:
		SwitchNodePtr mCtrlsGroup;
		NodePtr mBoundNode;
	};
	typedef Pointer0<BoundCtrl> BoundCtrlPtr;

	class CameraCtrl : public EventHandler
	{
	public:
		CameraCtrl();
		virtual ~CameraCtrl();

		Node *GetNode();

		virtual void OnEvent(Event *event);

		void UpdateCtrl();

	protected:
		NodePtr mCtrlNode;
		PolysegmentPtr mPoly;
	};
	typedef Pointer0<CameraCtrl> CameraCtrlPtr;

}

#endif