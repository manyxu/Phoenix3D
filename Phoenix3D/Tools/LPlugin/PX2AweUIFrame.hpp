// PX2AweUIFrame.hpp

#ifndef PX2AWEUIFRAME_HPP
#define PX2AWEUIFRAME_HPP

#include "PX2LP_Pre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2InputEventData.hpp"
#include <PX2Texture2D.hpp>
#include <Awesomium/WebCore.h>
#include <Awesomium/WebView.h>
#include <Awesomium/BitmapSurface.h>

namespace PX2
{

	class PX2LP_DLL_ITEM AweUIFrame : public UIFrame, 
		public Awesomium::JSMethodHandler, 
		public Awesomium::WebViewListener::Load
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_FUNCTION;
		PX2_DECLARE_STREAM(AweUIFrame);

	public:
		AweUIFrame();
		virtual ~AweUIFrame();

		virtual void SetActivate(bool act);

		void OpenURL(const std::string &address);
		void ExecuteJavaScript(const std::string &script,
			const std::string &frame_xpath);

		Awesomium::WebView* GetWebView();

		virtual void OnMethodCall(Awesomium::WebView* caller,
			unsigned int remote_object_id,
			const Awesomium::WebString& method_name,
			const Awesomium::JSArray& args);

		virtual Awesomium::JSValue OnMethodCallWithReturnValue(
			Awesomium::WebView* caller, unsigned int remote_object_id,
			const Awesomium::WebString& method_name,
			const Awesomium::JSArray& args);

		virtual void OnBeginLoadingFrame(Awesomium::WebView* caller, 
			int64 frame_id, bool is_main_frame, const Awesomium::WebURL& url,
			bool is_error_page);
		virtual void OnFailLoadingFrame(Awesomium::WebView* caller,
			int64 frame_id, bool is_main_frame, const Awesomium::WebURL& url,
			int error_code, const Awesomium::WebString& error_desc);
		virtual void OnFinishLoadingFrame(Awesomium::WebView* caller,
			int64 frame_id, bool is_main_frame, const Awesomium::WebURL& url);
		virtual void OnDocumentReady(Awesomium::WebView* caller,
			const Awesomium::WebURL& url);

		virtual void OnSizeChanged();

		virtual void OnWidgetPicked(const UIInputData &inputData);

	protected:
		virtual void UpdateWorldData(double applicationTime,
			double elapsedTime);
		void AllocateViewBuffer();

		void OnMousePressed(MouseButtonID code);
		void OnMouseReleased(MouseButtonID code);
		void OnKeyCodePressed(KeyCode code);
		void OnKeyCodeReleased(KeyCode code);
		void OnKeyTyped(wchar_t unichar);
		void OnCursorMoved(int x, int y);
		void OnMouseWheeled(int delta);

		void MouseButtonHelper(MouseButtonID code, bool isUp);
		void KeyboardButtonHelper(KeyCode code, bool isUp);

		Awesomium::WebCore *mWebCore;
		Awesomium::WebView *mWebView;
		Awesomium::BitmapSurface *mBitmapSurface;

		int	miWidth;
		int miHeight;
		bool mIsHasLoaded;

		static int mNumberOfViews;

		UIFPicBoxPtr mFPicBox;
		Texture2DPtr mTex2D;
	};

	PX2_REGISTER_STREAM(AweUIFrame);
	typedef Pointer0<AweUIFrame> AweUIFramePtr;

}

#endif