// PX2EU_CanvasPreView.hpp

#ifndef PX2EU_CANVASPREVIEW_HPP
#define PX2EU_CANVASPREVIEW_HPP

#include "PX2EditorPre.hpp"
#include "PX2UICanvas.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2UIFText.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_CanvasPreView : public UICanvas
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_CanvasPreView);

	public:
		EU_CanvasPreView();
		virtual ~EU_CanvasPreView();

		virtual void Draw(Renderer *renderer);

		virtual void OnEvent(Event *event);
		virtual void OnSizeChanged();

		void SetObject(Object *obj);

		enum PreViewType
		{
			PVT_NONE,
			PVT_TEXTURE,
			PVT_MODEL,
			PVT_MAX_TYPE
		};

		enum PreViewTextureMode
		{
			PVTM_AUTO,
			PVTM_EXTEND,
			PVTM_REALSIZE,
			PVTM_MAXMODE
		};

	protected:
		virtual void UpdateWorldData(double applicationTime,
			double elapsedTime);
		void _ReSizeTexture();
		void _SetTexSizeExtend(float texWidth, float texHeight);
		void _SetTexSizeRealSize(float texWidth, float texHeight);

		UIFPicBoxPtr mUIFPicBox;
		UIFTextPtr mFText;
		PreViewType mPreViewType;
		PreViewTextureMode mPreViewTextureMode;
	};

	PX2_REGISTER_STREAM(EU_CanvasPreView);
	typedef PointerRef<EU_CanvasPreView> EU_CanvasPreViewPtr;

}

#endif