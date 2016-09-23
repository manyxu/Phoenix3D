// PX2CurveGroupUI.hpp

#ifndef PX2UICURVEGROUP_HPP
#define PX2UICURVEGROUP_HPP

#include "PX2EditorPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2UIFText.hpp"
#include "PX2Curve.hpp"
#include "PX2CurveGroup.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM UICurveGroup : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;

	public:
		UICurveGroup (CurveGroup *group);
		virtual ~UICurveGroup ();

		void SetText (std::string text);
		void SetBackColor (Float3 color);

		Sizef GetSize () { return mSize; }
		PX2::CurveGroup *GetCurveGroup () { return mCurveGroup; }

	protected:
		UICurveGroup ();
		virtual void OnWidgetPicked(const UIInputData &inputData);
		virtual void OnWidgetNotPicked(const UIInputData &inputData);
		virtual void OnUIPicked(const UIInputData &inputData);
		virtual void OnUINotPicked(const UIInputData &inputData);
		virtual void OnEvent (Event *event);

		PX2::CurveGroupPtr mCurveGroup;
		PX2::UIFPicBoxPtr mFBackground;
		PX2::UIFPicBoxPtr mBox0;
		PX2::UIFPicBoxPtr mBox1;
		PX2::UIFPicBoxPtr mBox2;
		PX2::UIFPicBoxPtr mBox3;
		PX2::UIFPicBoxPtr mBox4;
		PX2::UIFPicBoxPtr mBox5;
		PX2::UIFPicBoxPtr mBox;
		bool mIsShowBox0;
		bool mIsShowBox1;
		bool mIsShowBox2;
		bool mIsShowBox3;
		bool mIsShowBox4;
		bool mIsShowBox5;
		bool mIsShowBox;
		PX2::UIFTextPtr mFText;
	};

	typedef PointerRef<UICurveGroup> UICurveGroupPtr;

}

#endif