// PX2UIFrame.hpp

#ifndef PX2UIFRAME_HPP
#define PX2UIFRAME_HPP

#include "PX2UIPre.hpp"
#include "PX2Node.hpp"
#include "PX2Size.hpp"
#include "PX2UIDefine.hpp"
#include "PX2InputPushTransformController.hpp"
#include "PX2UIFrameGridAlignControl.hpp"
#include "PX2UIPicBox.hpp"

namespace PX2
{

	class UIView;

	class PX2_EXTENDS_ITEM UIFrame : public Node
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(UIFrame);

	public:
		UIFrame();
		virtual ~UIFrame();

		virtual int AttachChild (Movable* child);

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);

		// Size
	public:
		void SetSize(float width, float height);
		void SetSize(const Sizef &size);
		const Sizef &GetSize() const;
		void SetWidth(float width);
		float GetWidth() const;
		void SetHeight(float height);
		float GetHeight() const;

		void SetBorderSize(float width, float height);
		void SetBorderSize(const Sizef &size);
		const Sizef &GetBorderSize() const;
		void SetBorderWidth(float width);
		float GetBorderWidth() const;
		void SetBorderHeight(float height);
		float GetBorderHeight() const;

		virtual void OnSizeChanged();
		virtual void OnBorderSizeChanged();

		UIPicBox *CreateAddBackgroundPicBox();

	protected:
		Sizef mSize;
		Sizef mBorderSize;
		Pointer0<UIPicBox> mBackgroundPicBox;

		// Pvoit
	public:
		void SetPvoit(float x, float y);
		virtual void SetPvoit(const Float2 &pvoit);
		const Float2 &GetPvoit() const;

	protected:
		Float2 mPvoit;

		// Anchor Layout
	public:
		void EnableAnchorLayout(bool enable);
		const bool IsAnchorLayoutEnable() const;

		void SetAnchorHor(float anchorX, float anchorY);
		void SetAnchorHor(const Float2 &anchor);
		const Float2 &GetAnchorHor() const;
		void SetAnchorParamHor(float param0, float param1);
		void SetAnchorParamHor(const Float2 &param);
		const Float2 &GetAnchorParamHor() const;

		void SetAnchorVer(float anchorX, float anchorY);
		void SetAnchorVer(const Float2 &anchor);
		const Float2 &GetAnchorVer() const;
		void SetAnchorParamVer(float param0, float param1);
		void SetAnchorParamVer(const Float2 &param);
		const Float2 &GetAnchorParamVer() const;

	public_internal:
		void MarkRelatvieChange();
		AVector LeftBottomCornerOffset;

	protected:
		bool mIsAnchorLayoutEnable;
		Float2 mAnchorHor;
		Float2 mAnchorVer;
		Float2 mAnchorParamHor;
		Float2 mAnchorParamVer;

	protected:
		virtual void UpdateLayout();

		bool mIsLayoutChanged;

	public_internal:
		// info为1表示Pressed，为2表示Released
		virtual void OnChildPicked(int info, Movable *child);
		virtual void OnChildUIAfterPicked(int info, Movable *child);

		// calls
	public:
		void SetUICallback(UICallback callback);
		UICallback GetUICallback() const;

		void SetScriptHandler(const std::string &scriptHandler);
		const std::string &GetScriptHandler() const;

	protected:
		UICallback mUICallback;
		std::string mUIScriptHandler;

	public_internal:
		void SetUIView(UIView *uiView);
		UIView *GetUIView();

	protected:
		UIView *mUIView;

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