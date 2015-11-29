// PX2Canvas.hpp

#ifndef PX2CANVAS_HPP
#define PX2CANVAS_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"
#include "PX2SizeNode.hpp"
#include "PX2Rect.hpp"
#include "PX2Culler.hpp"
#include "PX2Node.hpp"
#include "PX2Vector2.hpp"
#include "PX2RenderTarget.hpp"
#include "PX2WireProperty.hpp"

namespace PX2
{

	class Renderer;

	class PX2_FOUNDATION_ITEM Canvas : public SizeNode
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(Canvas);

	public:
		Canvas();
		virtual ~Canvas();

	protected:
		virtual void UpdateWorldBound();

		CameraPtr mUpdateBeforeCamera;

		// Size
		// 在编辑器中需要随着窗口，动态调整相机等，所以Size会频繁改变
		// 在AppPlayer中，加载工程时候，设置一次和工程大小一致，不会频繁改变
	public:
		virtual void OnSizeChanged();
		void SetSizeChangeReAdjustCamera(bool doReAdjust);

	protected:
		bool mIsSizeChangeReAdjustCamera;

		// ScreenSize
		// 当屏幕改变，rendertarget可能需要重建，所以设计此接口，进行处理
	public:
		virtual void SetScreenSize(const Sizef &size);
		const Sizef &GetScreenSize() const;

	protected:
		Sizef mScreenSize;

		// ViewPort
	public:
		void SetViewPort(const Rectf &viewPort);
		void SetViewPort(float left, float bottom, float width, float height);
		const Rectf &GetViewPort() const;

	protected:
		Rectf mViewPort;

		// Renderer Camera Culler
	public:
		virtual void SetRenderer(Renderer *renderer);
		Renderer *GetRenderer();

		virtual void SetCamera(Camera *camera);
		Camera *GetCamera();

		Culler &GetCuller();
		void SetNotDrawByParent(bool isNotDrawPyParent);
		bool IsNotDrawByParent() const;

		bool GetPickRay(float screenX, float screenY, APoint& origin, AVector& direction);
		Vector2f PointWorldToViewPort(const APoint &worldPos, bool *isInBack=0);
		AVector ScreenVectorToCamera(const AVector &screenVec);

	public_internal:
		virtual void OnGetVisibleSet(Culler& culler, bool noCull);

	protected:
		Renderer *mRenderer;
		CameraPtr mCamera;
		Culler mCuller;

		WirePropertyPtr mOverrideWireProperty;

		// Draw
	public:
		void SetPriority(int priority); // 越大越在前
		int GetPriority() const;

		void SetBeforeDrawClear(bool color, bool depth, bool stencil);
		void GetBeforeDrawClear(bool &color, bool &depth, bool &stencil);
		void SetClearColor(const Float4 &clearColor);
		const Float4 &GetClearColor() const;

		virtual void ClearVisibleSet();
		virtual void ComputeVisibleSetAndEnv();
		virtual void Draw();

		void SetAfterDrawClear(bool color, bool depth, bool stencil);
		void GetAfterDrawClear(bool &color, bool &depth, bool &stencil);

	public_internal:
		static bool LessThan(const Canvas *step0, const Canvas *step1);

	protected:
		int mPriority;
		bool mBeforeDoClearColor;
		bool mBeforeDoClearDepth;
		bool mBeforeDoClearStencil;
		bool mAfterDoClearColor;
		bool mAfterDoClearDepth;
		bool mAfterDoClearStencil;
		Float4 mClearColor;

	public_internal:
		// 编辑器中调用
		void _Tick(double appTime, double elapsedTime);

		// Help
	public:
		std::pair<float, float> CalPixelToWorld();
	};

	PX2_REGISTER_STREAM(Canvas);
	typedef Pointer0<Canvas> CanvasPtr;

#include "PX2Canvas.inl"
	typedef Pointer0<Canvas> CanvasPtr;

}

#endif