// PX2EditObjectTerrain.hpp

#ifndef PX2EDITOBJECTTERRAIN_HPP
#define PX2EDITOBJECTTERRAIN_HPP

#include "PX2EditorPre.hpp"
#include "PX2Object.hpp"
#include "PX2SmartPointerRef.hpp"
#include "PX2EditObject.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EditObjectTerrain : public EditObject
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(EditObjectTerrain);
		PX2_DECLARE_PROPERTY;

	public:
		EditObjectTerrain();
		virtual ~EditObjectTerrain();

		// brush
		enum InterplateMode
		{
			IM_NONE,
			IM_LINER,
			IM_SQUARE,
			IM_COS,
			IM_MAX_MODE
		};
		void SetInterplateMode(InterplateMode mode);
		InterplateMode GetInterplateMode() const;

		void SetBrushSize(float size);
		float GetBrushSize() const;

		void SetBrushStrength(float strength);
		float GetBrushStrength() const;

		// height process
		enum HeightMode
		{
			HM_RAISE,
			HM_LOWER,
			HM_FLATTEN,
			HM_SMOOTH,
			HM_FIXH,
			MH_HOLE,
			MH_NOHOLE,
			HM_MAX_MODE
		};
		void SetHeightMode(HeightMode mode);
		HeightMode GetHeightMode() const;

		void SetHeightModeFixHeight(float fixHeight);
		float GetHeightModeFixHeight() const;

		enum TextureMode
		{
			TM_INCREASE,
			TM_DECREASE,
			TM_APPLY,
			TM_SMOOTH,
			TM_NOISE,
			TM_MAX_MODE
		};
		void SetTextureMode(TextureMode mode);
		TextureMode GetTextureMode() const;

	protected:
		InterplateMode mInterplateMode;

		float mBrushSize;
		float mBrushStrength;

		HeightMode mHeightMode;
		float mFixHeightVal;

		int mSelectLayerIndex;

		TextureMode mTextureMode;
	};

	PX2_REGISTER_STREAM(EditObjectTerrain);
	typedef PointerRef<EditObjectTerrain> EditObjectTerrainPtr;

}

#endif