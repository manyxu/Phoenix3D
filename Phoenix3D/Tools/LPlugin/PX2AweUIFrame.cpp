// PX2AweUIFrame.cpp

#include "PX2AweUIFrame.hpp"
#include <Awesomium/WebURL.h>
#include <Awesomium/WebString.h>
#include <Awesomium/STLHelpers.h>
using namespace PX2;
using namespace Awesomium;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, AweUIFrame);
PX2_IMPLEMENT_STREAM(AweUIFrame);
PX2_IMPLEMENT_FACTORY(AweUIFrame);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, AweUIFrame);

#define DEPTH 4

int AweUIFrame::mNumberOfViews = 0;
//----------------------------------------------------------------------------
AweUIFrame::AweUIFrame() :
mWebCore(0),
mWebView(0),
mBitmapSurface(0),
miWidth(0),
miHeight(0),
mIsHasLoaded(false)
{
	mFPicBox = new0 UIFPicBox();
	AttachChild(mFPicBox);
	mFPicBox->LocalTransform.SetTranslateY(-1.0f);
	mFPicBox->SetAnchorHor(0.0f, 1.0f);
	mFPicBox->SetAnchorVer(0.0f, 1.0f);

	miWidth = 512;
	miHeight = 512;

	mTex2D = new0 Texture2D(Texture::TF_A8R8G8B8, miWidth, miHeight, 1);
	mFPicBox->GetUIPicBox()->SetTexture(mTex2D);
	Material *mtl = mFPicBox->GetUIPicBox()->GetMaterialInstance()->GetMaterial();
	mtl->GetPixelShader(0, 0)->SetFilter(0, Shader::SF_NEAREST_NEAREST);

	mNumberOfViews++;

	mWebCore = WebCore::instance();
	if (!mWebCore)
	{
		WebConfig config;
		mWebCore = WebCore::Initialize(config);
	}

	mWebView = mWebCore->CreateWebView(miWidth, miHeight);
	mWebView->set_js_method_handler(this);
	mWebView->set_load_listener(this);

	SetWidget(true);
}
//----------------------------------------------------------------------------
AweUIFrame::~AweUIFrame()
{
	mNumberOfViews--;

	mWebView->Destroy();

	if (mWebCore && mNumberOfViews == 0)
	{
		mWebCore->Shutdown();
	}

	mWebView = 0;
	mWebCore = 0;
	mBitmapSurface = 0;
}
//----------------------------------------------------------------------------
void AweUIFrame::SetActivate(bool act)
{
	UIFrame::SetActivate(act);

	if (act)
	{
		mWebView->Focus();
		mWebView->ResumeRendering();
	}
	else
	{
		mWebView->Unfocus();
		mWebView->PauseRendering();
	}
}
//----------------------------------------------------------------------------
void AweUIFrame::OpenURL(const std::string &address)
{
	mWebView->LoadURL(WebURL(WSLit("about:blank")));
	mWebView->LoadURL(WebURL(WSLit(address.c_str())));
}
//----------------------------------------------------------------------------
void AweUIFrame::ExecuteJavaScript(const std::string &script,
	const std::string &frame_xpath)
{
	mWebView->ExecuteJavascript(WSLit(script.c_str()),
		WSLit(frame_xpath.c_str()));
}
//----------------------------------------------------------------------------
void AweUIFrame::OnMethodCall(Awesomium::WebView* caller,
	unsigned int remote_object_id,
	const Awesomium::WebString& method_name,
	const Awesomium::JSArray& args)
{
	PX2_UNUSED(caller);
	PX2_UNUSED(remote_object_id);
	PX2_UNUSED(method_name);
	PX2_UNUSED(args);
}
//----------------------------------------------------------------------------
Awesomium::JSValue AweUIFrame::OnMethodCallWithReturnValue(
	Awesomium::WebView* caller, unsigned int remote_object_id,
	const Awesomium::WebString& method_name,
	const Awesomium::JSArray& args)
{
	PX2_UNUSED(caller);
	PX2_UNUSED(remote_object_id);
	PX2_UNUSED(method_name);
	PX2_UNUSED(args);

	return Awesomium::JSValue::Undefined();
}
//----------------------------------------------------------------------------
void AweUIFrame::OnBeginLoadingFrame(Awesomium::WebView* caller,
	int64 frame_id, bool is_main_frame, const Awesomium::WebURL& url,
	bool is_error_page)
{
	PX2_UNUSED(caller);
	PX2_UNUSED(frame_id);
	PX2_UNUSED(is_main_frame);
	PX2_UNUSED(url);
	PX2_UNUSED(is_error_page);
}
//----------------------------------------------------------------------------
void AweUIFrame::OnFailLoadingFrame(Awesomium::WebView* caller,
	int64 frame_id, bool is_main_frame, const Awesomium::WebURL& url,
	int error_code, const Awesomium::WebString& error_desc)
{
	PX2_UNUSED(caller);
	PX2_UNUSED(frame_id);
	PX2_UNUSED(is_main_frame);
	PX2_UNUSED(url);
	PX2_UNUSED(error_code);
	PX2_UNUSED(error_desc);
}
//----------------------------------------------------------------------------
void  AweUIFrame::OnFinishLoadingFrame(Awesomium::WebView* caller,
	int64 frame_id, bool is_main_frame, const Awesomium::WebURL& url)
{
	PX2_UNUSED(caller);
	PX2_UNUSED(frame_id);
	PX2_UNUSED(is_main_frame);
	PX2_UNUSED(url);
}
//----------------------------------------------------------------------------
void  AweUIFrame::OnDocumentReady(Awesomium::WebView* caller,
	const Awesomium::WebURL& url)
{
	PX2_UNUSED(caller);
	PX2_UNUSED(url);
}
//----------------------------------------------------------------------------
Awesomium::WebView* AweUIFrame::GetWebView()
{
	return mWebView;
}
//----------------------------------------------------------------------------
void AweUIFrame::UpdateWorldData(double applicationTime,
	double elapsedTime)
{
	UIFrame::UpdateWorldData(applicationTime, elapsedTime);

	mWebCore->Update();

	mBitmapSurface = (BitmapSurface*)mWebView->surface();

	if (mBitmapSurface && miWidth>0 && miHeight>0)
	{
		AllocateViewBuffer();
	}
}
//----------------------------------------------------------------------------
void AweUIFrame::AllocateViewBuffer()
{
	char* pSrc = reinterpret_cast<char*>(const_cast<unsigned char*>((
		mBitmapSurface)->buffer()));
	int iSrcWidth = mBitmapSurface->width();
	int iSrcHeight = mBitmapSurface->height();
	if (iSrcWidth != miWidth || iSrcHeight != miHeight)
		return;

	char* pDest = mTex2D->GetData(0);

	int offsetSrc = 0;
	int offsetDst = 0;

	for (int row = 0; row < miHeight; ++row)
	{
		for (int col = 0; col < miWidth; ++col)
		{
			pDest[offsetDst] = pSrc[offsetSrc];
			pDest[offsetDst + 1] = pSrc[offsetSrc + 1];
			pDest[offsetDst + 2] = pSrc[offsetSrc + 2];
			pDest[offsetDst + 3] = pSrc[offsetSrc + 3];

			offsetSrc += 4;
			offsetDst += 4;
		}
	}

	Renderer::UpdateAll(mTex2D, 0);
}
//----------------------------------------------------------------------------
void AweUIFrame::MouseButtonHelper(MouseButtonID code, bool isUp)
{
	MouseButton mouseButton;

	switch (code)
	{
	case MBID_LEFT:
		mouseButton = kMouseButton_Left;
		break;
	case MBID_RIGHT:
		mouseButton = kMouseButton_Right;
		break;
	case MBID_MIDDLE:
		mouseButton = kMouseButton_Middle;
		break;
	default:
		mouseButton = kMouseButton_Left;
		break;
	}

	isUp ? mWebView->InjectMouseUp(mouseButton) :
		mWebView->InjectMouseDown(mouseButton);
}
//----------------------------------------------------------------------------
void AweUIFrame::KeyboardButtonHelper(KeyCode code, bool isUp)
{
	WebKeyboardEvent ent;

	ent.virtual_key_code = code;
	ent.type = isUp ? WebKeyboardEvent::kTypeKeyUp :
		WebKeyboardEvent::kTypeKeyDown;

	mWebView->InjectKeyboardEvent(ent);
}
//----------------------------------------------------------------------------
void AweUIFrame::OnMousePressed(MouseButtonID code)
{
	MouseButtonHelper(code, false);
}
//----------------------------------------------------------------------------
void AweUIFrame::OnMouseReleased(MouseButtonID code)
{
	MouseButtonHelper(code, true);
}
//----------------------------------------------------------------------------
void AweUIFrame::OnKeyCodePressed(KeyCode code)
{
	KeyboardButtonHelper(code, false);
}
//----------------------------------------------------------------------------
void AweUIFrame::OnKeyCodeReleased(KeyCode code)
{
	KeyboardButtonHelper(code, true);
}
//----------------------------------------------------------------------------
void AweUIFrame::OnKeyTyped(wchar_t unichar)
{
	WebKeyboardEvent ent;
	ent.text[0] = unichar;
	ent.type = WebKeyboardEvent::kTypeChar;
	
	mWebView->InjectKeyboardEvent(ent);
}
//----------------------------------------------------------------------------
void AweUIFrame::OnCursorMoved(int x, int y)
{
	mWebView->InjectMouseMove(x, y);
}
//----------------------------------------------------------------------------
void AweUIFrame::OnMouseWheeled(int delta)
{
	mWebView->InjectMouseWheel(delta*5.0f, 0);
}
//----------------------------------------------------------------------------
void AweUIFrame::OnSizeChanged()
{
	UIFrame::OnSizeChanged();

	miWidth = (int)mSize.Width;
	miHeight = (int)mSize.Height;

	if (miWidth > 0 && miHeight > 0)
	{
		mTex2D = new0 Texture2D(Texture::TF_A8R8G8B8, 
			miWidth, miHeight, 1);
		mFPicBox->GetUIPicBox()->SetTexture(mTex2D);

		mWebView->Resize(miWidth, miHeight);
	}
}
//----------------------------------------------------------------------------
void AweUIFrame::OnUIPicked(const UIInputData &inputData)
{
	MouseButtonID mbID;
	if (UIInputData::MT_LEFT == inputData.TheMouseTag)
	{
		mbID = MBID_LEFT;
	}
	else if (UIInputData::MT_RIGHT == inputData.TheMouseTag)
	{
		mbID = MBID_RIGHT;
	}
	else if (UIInputData::MT_MIDDLE == inputData.TheMouseTag)
	{
		mbID = MBID_MIDDLE;
	}

	if (UIPT_PRESSED == inputData.PickType)
	{
		OnMousePressed(mbID);
	}
	else if (UIPT_RELEASED == inputData.PickType)
	{
		OnMouseReleased(mbID);
	}
	else if (UIPT_MOVED == inputData.PickType)
	{
		const Rectf &worldRect = GetWorldRect();

		int iPosX = (int)inputData.WorldPos.X();
		iPosX -= (int)worldRect.Left;

		int iPosZ = (int)inputData.WorldPos.Z();
		iPosZ -= (int)worldRect.Bottom;

		iPosZ = miHeight - iPosZ;

		OnCursorMoved(iPosX, iPosZ);
	}
	else if (UIPT_WHELLED == inputData.PickType)
	{
		OnMouseWheeled(inputData.Wheel);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
AweUIFrame::AweUIFrame(LoadConstructor value) :
UIFrame(value),
mWebCore(0),
mWebView(0),
mBitmapSurface(0),
miWidth(0),
miHeight(0),
mIsHasLoaded(false)
{
}
//----------------------------------------------------------------------------
void AweUIFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(AweUIFrame, source);
}
//----------------------------------------------------------------------------
void AweUIFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void AweUIFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool AweUIFrame::Register(OutStream& target) const
{
	return UIFrame::Register(target);
}
//----------------------------------------------------------------------------
void AweUIFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(AweUIFrame, target);
}
//----------------------------------------------------------------------------
int AweUIFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------