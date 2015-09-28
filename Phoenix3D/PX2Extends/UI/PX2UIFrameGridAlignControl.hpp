// PX2UIFrameGridAlignController.hpp

#ifndef PX2UIFRAMEGRIDALIGNCONTROLLER_HPP
#define PX2UIFRAMEGRIDALIGNCONTROLLER_HPP

#include "PX2ExtendsPre.hpp"
#include "PX2Controller.hpp"
#include "PX2Size.hpp"
#include "PX2Float2.hpp"

namespace PX2
{
	
	class UIFrame;

	class PX2_EXTENDS_ITEM UIFrameGridAlignControl : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UIFrameGridAlignControl);

	public:
		UIFrameGridAlignControl();
		virtual ~UIFrameGridAlignControl();

		void SetCellSize(const Sizef &cellSize);
		const Sizef &GetCellSize() const;

		void SetSpacing(const Float2 &spacing);
		const Float2 &GetSpacing() const;

		enum StartCornerType
		{
			SCT_UPPER_LEFT,
			SCT_UPPER_RIGHT,
			SCT_LOWER_LEFT,
			SCT_LOWER_RIGHT,
			SCT_MAX_TYPE
		};
		void SetStartCorner(StartCornerType sc);
		StartCornerType GetStartCorner() const;

		enum StartAxisType
		{
			SAT_HORIZONTAL,
			SAT_VERTICAL,
			SAT_MAX_TYPE
		};
		void SetStartAxis(StartAxisType sa);
		StartAxisType GetStartAxis() const;

		enum ChildAlignmentType
		{
			CAT_UPPER_LEFT,
			CAT_UPPER_CENTER,
			CAT_UPPER_RIGHT,
			CAT_MIDDLE_LEFT,
			CAT_MIDDLE_CENTER,
			CAT_MIDDLE_RIGHT,
			CAT_LOWER_LEFT,
			CAT_LOWER_CENTER,
			CAT_LOWER_RIGHT,
			CAT_MAX_TYPE
		};
		void SetChildAlignment(ChildAlignmentType ca);
		ChildAlignmentType GetChildAlignment() const;

		enum ConstraintType
		{
			CT_FLEXIBLE,
			CT_FIXED_COLUMN_COUNT,
			CT_FIXED_ROW_COUNT,
			CT_MAX_TYPE
		};
		void SetConstraintType(ConstraintType type);
		ConstraintType GetConstraintType() const;

		void MarkRelatvieChange();

	protected:
		virtual void _Update(double applicationTime, double elapsedTime);
		int _UpdateAlignItems();
		void _UpdateItems(UIFrame *parent, const std::vector<UIFrame*> &frames,
			StartCornerType startCorner, StartAxisType startAxis,
			ChildAlignmentType childAlignmentType);

		bool mIsLayoutChanged;
		Sizef mCellSize;
		Float2 mSpacing;
		StartCornerType mStartCornerType;
		StartAxisType mStartAxisType;
		ChildAlignmentType mChildAlignmentType;
		ConstraintType mConstraintType;
	};

#include "PX2UIFrameGridAlignControl.inl"
	PX2_REGISTER_STREAM(UIFrameGridAlignControl);
	typedef Pointer0<UIFrameGridAlignControl> UIFrameGridAlignControlPtr;

}

#endif