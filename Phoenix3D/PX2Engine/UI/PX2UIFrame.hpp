// PX2UIFrame.hpp

#ifndef PX2UIFRAME_HPP
#define PX2UIFRAME_HPP

#include "PX2UIPre.hpp"
#include "PX2SizeNode.hpp"
#include "PX2Size.hpp"
#include "PX2UIDefine.hpp"
#include "PX2InputPushTransformController.hpp"
#include "PX2UIFrameGridAlignControl.hpp"
#include "PX2UIPicBox.hpp"

namespace PX2
{

	class UICanvas;

	class PX2_ENGINE_ITEM UIFrame : public SizeNode
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_FUNCTION;
		PX2_NEW(UIFrame);
		PX2_DECLARE_STREAM(UIFrame);

	public:
		UIFrame();
		virtual ~UIFrame();

		virtual void OnChildAttached(Movable *child);
		virtual void OnChildDetach(Movable *child);

		UICanvas *GetBelongUICanvas();

	public_internal:
		static void TravelExecuteSetCanvas(Movable *mov, Any *data, bool &goOn);

		void SetBelongUICanvas(UICanvas *canvas);
		void _SetBelongUICanvas(UICanvas *canvas);

	protected:
		UICanvas *mBelongUICanvas;

	public:
		virtual void OnSizeChanged();
		virtual void OnPivotChanged();

		UIPicBox *CreateAddBackgroundPicBox(bool setWhite=true);
		UIPicBox *GetBackgroundPicBox();
		void DestoryBackgroundPicBox();

		void SetActivateColor(const Float3 &color);
		const Float3 &GetActivateColor() const;
		void SetActivateAlpha(float alpha);
		float GetActivateAlpha() const;
		void SetActivateBrightness(float brightness);
		float GetActivateBrightness() const;

	protected:
		UIPicBoxPtr mBackgroundPicBox;
		Float3 mActivatedColor;
		float mActivatedAlpha;
		float mActivatedBrightness;

	public:
		void SetUIChildPickOnlyInSizeRange(bool onlyInRange);
		bool IsUIChildPickOnlyInSizeRange() const;

		virtual void PreUIPick(const UIInputData &inputData, UICanvas *canvas);
		virtual void OnWidgetPicked(const UIInputData &inputData);
		virtual void OnWidgetNotPicked(const UIInputData &inputData);
		virtual void OnUIPicked(const UIInputData &inputData);
		virtual void OnUINotPicked(const UIInputData &inputData);

	protected:
		bool mIsUIChildPickOnlyInSizeRange;

		// calls
	public:
		void SetWidget(bool isWidget);
		bool IsWidget() const;

		void AddUICallback(UICallback callback);
		const std::vector<UICallback> &GetUICallbacks() const;

		typedef void (UIFrame::*MemUICallback)(UIFrame *frame, UICallType type);

		void SetMemUICallback(UIFrame *object, MemUICallback callback);
		MemUICallback GetMemUICallback() const;

		void SetScriptHandler(const std::string &scriptHandler);
		const std::string &GetScriptHandler() const;

	protected:
		void _UICallbacksCalls(UICallType callType);
		void CallString(const std::string &funStr, const char *format, ...);

		bool mIsWidget;
		std::vector<UICallback> mUICallbacks;
		UIFrame *mMemObject;
		MemUICallback mMemUICallback;
		std::string mUIScriptHandler;

		// mask
	public:
		UIPicBox *CreateAddMask();
		void SetMaskVal(int maskVal);
		int GetMaskVal() const;
		UIPicBox *GetMask();
		void DestoryMask();

	protected:
		int mMaskVal;
		UIPicBoxPtr mMaskPicBox;

		// InputTrans
	public:
		InputPushTransformController *CreateAddIPTCtrl(bool doResetPlay = true);
		void DestoryIPTCtrl();
		InputPushTransformController *GetIPTCtrl();

	protected:
		InputPushTransformControllerPtr mIPTCtrl;

		// GridAlign
	public:
		UIFrameGridAlignControl *CreateAddGridAlignCtrl(bool doResetPlay = true);
		void DestoryGridAlignCtrl();
		UIFrameGridAlignControl *GetGridAlignCtrl();

	protected:
		UIFrameGridAlignControlPtr mGridAlignCtrl;
	};

	PX2_REGISTER_STREAM(UIFrame);
	typedef PointerRef<UIFrame> UIFramePtr;
#include "PX2UIFrame.inl"

}

#endif