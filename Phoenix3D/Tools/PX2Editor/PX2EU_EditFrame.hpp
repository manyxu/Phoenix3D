// PX2EU_EditFrame.hpp

#ifndef PX2EU_EDITFRAME_HPP
#define PX2EU_EDITFRAME_HPP

#include "PX2EditorPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2SmartPointerRef.hpp"
#include "PX2EU_PropertyGridEdit.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_EditFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_EditFrame);

	public:
		EU_EditFrame();
		virtual ~EU_EditFrame();

		enum EditType
		{
			ET_TERRAIN,
			ET_MAX_TYPE
		};
		void SetEditType(EditType et);
		EditType GetEditType() const;

	protected:
		void _RefreshTerrain();

		EditType mEditType;
		EU_PropertyGridEditPtr mEU_PropertyGridEdit;
	};

	PX2_REGISTER_STREAM(EU_EditFrame);
	typedef PointerRef<EU_EditFrame> EU_EditFramePtr;

}

#endif