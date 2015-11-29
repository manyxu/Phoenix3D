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

	class PX2_EXTENDS_ITEM UIFrame : public SizeNode
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(UIFrame);

	public:
		UIFrame();
		virtual ~UIFrame();

		typedef void (UIFrame::*MemUICallback)(UIFrame *frame, UICallType type);

		virtual int AttachChild (Movable* child);

	public:
		virtual void OnSizeChanged();
		virtual void OnPvoitChanged();

		UIPicBox *CreateAddBackgroundPicBox();

	protected:
		Pointer0<UIPicBox> mBackgroundPicBox;

	public_internal:
		// info为1表示Pressed，为2表示Released
		virtual void OnUIBeforePicked(int info);
		virtual void OnUIPicked(int info, Movable *child);
		virtual void OnUINotPicked(int info);

		// calls
	public:
		void SetUICallback(UICallback callback);
		UICallback GetUICallback() const;

		void SetMemUICallback(UIFrame *object, MemUICallback callback);
		MemUICallback GetMemUICallback() const;

		void SetScriptHandler(const std::string &scriptHandler);
		const std::string &GetScriptHandler() const;

	protected:
		UICallback mUICallback;
		UIFrame *mMemObject;
		MemUICallback mMemUICallback;
		std::string mUIScriptHandler;

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
	typedef Pointer0<UIFrame> UIFramePtr;
#include "PX2UIFrame.inl"

}

#endif