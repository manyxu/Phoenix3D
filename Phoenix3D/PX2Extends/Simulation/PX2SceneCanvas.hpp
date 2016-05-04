// PX2SceneCanvas.hpp

#ifndef PX2SCENECANVAS_HPP
#define PX2SCENECANVAS_HPP

#include "PX2SimulationPre.hpp"
#include "PX2Canvas.hpp"
#include "PX2UIPicBox.hpp"
#include "PX2UICanvas.hpp"
#include "PX2UIFrame.hpp"
#include "PX2ShadowMapMaterial.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM SceneCanvas : public Canvas
	{
		PX2_DECLARE_RTTI;

	public:
		SceneCanvas();
		virtual ~SceneCanvas();

		virtual void ComputeVisibleSetAndEnv();
		virtual void Draw(Renderer *renderer);

	protected:
		virtual void UpdateWorldData(double applicationTime, 
			double elapsedTime);

	public:
		virtual void OnSizeChanged();

	public:
		virtual void SetScreenSize(const Sizef &size);

		// Event
	public:
		virtual void DoExecute(Event *event);

		// Effects
	public:
		// bloom
		void SetUseBloom(bool useBloom);
		bool IsUseBloom() const;

		void SetShowShadowBloomEveryPass(bool isShowBloomEveryPass);
		bool IsShowShadowBloomEveryPass() const;

	protected:
		void _SetCameraF(Camera *camera, UIPicBox *uiPicBox);
		void _UpdateBloomChanged();
		void _UpdateBloomParams();
		void _UpdateShadowChanged();
		void _UpdateALightPicBoxTranslateSize();

		bool mIsShowShadowBloomEveryPass;

		bool mIsBloomChanged;
		bool mIsShadowMapChanged;

		CameraPtr mScreenCamera;

		UICanvasPtr mEffect_UICanvas;
		UIFramePtr mEffect_UIFrame;

		float mBloomShadowPicSize;

		RenderTargetPtr mEffect_RenderTarget_Normal;
		UIPicBoxPtr mEffect_UIPicBoxShow_Normal;

		RenderTargetPtr mEffect_RenderTarget_BloomBright;
		UIPicBoxPtr mEffect_UIPicBox_BloomBright;
		ShaderFloatPtr mBloom_BrightParam;

		RenderTargetPtr mEffect_RenderTarget_BlurH;
		UIPicBoxPtr mEffect_UIPicBox_BlurH;
		RenderTargetPtr mEffect_RenderTarget_BlurV;
		UIPicBoxPtr mEffect_UIPicBox_BlurV;
		Float4 mEffect_Blur_UVOffsets_H[16];
		Float4 mEffect_Blur_UVOffsets_V[16];

		UIPicBoxPtr mBloom_UIPicBox_Final;
		MaterialInstancePtr mBoom_MtlInstance;
		ShaderFloatPtr mBloom_BloomParam;

		Culler mEffect_Culler_Shadow;
		RenderTargetPtr mEffect_RenderTarget_Shadow;
		ShadowMap_MaterialPtr mEffect_Material_Shadow;
		UIPicBoxPtr mEffect_UIPicBox_Shadow;

		std::vector<UIPicBox *> mAlignPicBoxes;

		// ±à¼­Æ÷ÖÐÊ¹ÓÃ
	public_internal:
		void SetHelpGridCanvas(Canvas *canvas);
		Canvas *GetHelpGridCanvas();

	protected:
		CanvasPtr mHelpGridCanvas;
	};

	typedef PointerRef<SceneCanvas> SceneCanvasPtr;

}

#endif