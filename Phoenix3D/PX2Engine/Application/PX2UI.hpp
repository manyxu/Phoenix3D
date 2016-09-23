// PX2UI.hpp

#ifndef PX2UI_HPP
#define PX2UI_HPP

#include "PX2SizeNode.hpp"
#include "PX2CameraNode.hpp"
#include "PX2UIFrame.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UI : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_NEW(UI);
		PX2_DECLARE_STREAM(UI);

	public:
		UI();
		virtual ~UI();

		bool AddCamera(Camera *camera);
		std::vector<CameraPtr> &GetCameras();

		virtual void OnSizeChanged();

	protected:
		virtual void UpdateLeftBottomCornerOffset(Movable *parent);
		void _UpdateCamera();

		static float msUICameraY;
		CameraPtr mDefaultCamera;
		std::vector<CameraPtr> mCameras;
		CameraNodePtr mDefaultCameraNode;

	public:
		void OnEvent(Event *ent);
	};

#include "PX2UI.inl"
	PX2_REGISTER_STREAM(UI);
	typedef PointerRef<UI> UIPtr;

}

#endif