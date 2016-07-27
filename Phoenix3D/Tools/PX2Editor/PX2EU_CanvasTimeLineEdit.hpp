// PX2EU_CanvasTimeLineEdit.hpp

#ifndef PX2EU_CANVASTIMELINEEDIT_HPP
#define PX2EU_CANVASTIMELINEEDIT_HPP

#include "PX2EditorPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIList.hpp"
#include "PX2UICanvas.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_CanvasTimeLineEdit : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_CanvasTimeLineEdit);

	public:
		EU_CanvasTimeLineEdit();
		virtual ~EU_CanvasTimeLineEdit();

		void FitViewHorizontally();
		void FitViewVertically();
		void FitViewToAll();
		void FitViewToSelected();
		void ZoomCamera(float xDetal, float zDetal);
		void ZoomCameraTo(float xMax, float zMax);

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		void _RefreshGrid(bool doScale);
		void _TrySelectCurveCtrlPoint(const APoint &worldPos);

		virtual void OnSizeChanged();
		virtual void OnUIPicked(const UIInputData &inputData);
		virtual void OnUINotPicked(const UIInputData &inputData);

		enum MoveMode
		{
			MM_PAN,
			MM_ZOOM,
			MM_MAX_MODE
		};
		MoveMode mMoveMode;

		struct FontStr
		{
			FontStr() { x = 0; y = 0; }
			int x;
			int y;
			std::string str;
		};
		std::vector<FontStr> mFontStrs;

		bool mIsInitUpdated;
		UICanvasPtr mUICanvasGrid;

		float mLeftWidth;
		float mPixelOverCamIn;
		float mPixelOverCamOut;

		// Grid
		float mUPerGrid;
		float mVPerGrid;
		float mXStart;
		float mXEnd;
		float mZStart;
		float mZEnd;

		bool mIsPicked;
	};

	PX2_REGISTER_STREAM(EU_CanvasTimeLineEdit);
	typedef PointerRef<EU_CanvasTimeLineEdit> EU_CanvasTimeLineEditPtr;

}

#endif