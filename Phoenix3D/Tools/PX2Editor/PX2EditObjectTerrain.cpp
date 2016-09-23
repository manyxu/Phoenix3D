// PX2EditObjectTerrain.cpp

#include "PX2EditObjectTerrain.hpp"
#include "PX2Edit.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, EditObject, EditObjectTerrain);
PX2_IMPLEMENT_STREAM(EditObjectTerrain);
PX2_IMPLEMENT_FACTORY(EditObjectTerrain);
PX2_IMPLEMENT_DEFAULT_NAMES(EditObject, EditObjectTerrain);

//----------------------------------------------------------------------------
EditObjectTerrain::EditObjectTerrain()
{
	mInterplateMode = IM_COS;
	mBrushSize = 1.0f;
	mBrushStrength = 1.0f;
	mHeightMode = HM_RAISE;
	mFixHeightVal = 0.0f;

	mSelectLayerIndex = 0;

	mTextureMode = TM_INCREASE;
}
//----------------------------------------------------------------------------
EditObjectTerrain::~EditObjectTerrain()
{
}
//----------------------------------------------------------------------------
void EditObjectTerrain::SetInterplateMode(InterplateMode mode)
{
	mInterplateMode = mode;

	PX2_EDIT.GetTerrainEdit()->GetBrush()->SetInterplateMode(
		(TerrainBrush::InterplateMode)mode);
}
//----------------------------------------------------------------------------
EditObjectTerrain::InterplateMode EditObjectTerrain::GetInterplateMode() const
{
	return mInterplateMode;
}
//----------------------------------------------------------------------------
void EditObjectTerrain::SetBrushSize(float size)
{
	mBrushSize = size;

	PX2_EDIT.GetTerrainEdit()->GetBrush()->SetSize(size);
}
//----------------------------------------------------------------------------
float EditObjectTerrain::GetBrushSize() const
{
	return mBrushSize;
}
//----------------------------------------------------------------------------
void EditObjectTerrain::SetBrushStrength(float strength)
{
	mBrushStrength = strength;

	PX2_EDIT.GetTerrainEdit()->GetBrush()->SetStrength(strength);
}
//----------------------------------------------------------------------------
float EditObjectTerrain::GetBrushStrength() const
{
	return mBrushStrength;
}
//----------------------------------------------------------------------------
void EditObjectTerrain::SetHeightMode(HeightMode mode)
{
	mHeightMode = mode;

	PX2_EDIT.GetTerrainEdit()->GetHeightProcess()->SetHeightMode(
		(TerrainHeightProcess::HeightMode)mode);
}
//----------------------------------------------------------------------------
EditObjectTerrain::HeightMode EditObjectTerrain::GetHeightMode() const
{
	return mHeightMode;
}
//----------------------------------------------------------------------------
void EditObjectTerrain::SetHeightModeFixHeight(float fixHeight)
{
	mFixHeightVal = fixHeight;

	PX2_EDIT.GetTerrainEdit()->GetHeightProcess()->SetFixHVal(fixHeight);
}
//----------------------------------------------------------------------------
float EditObjectTerrain::GetHeightModeFixHeight() const
{
	return mFixHeightVal;
}
//----------------------------------------------------------------------------
void EditObjectTerrain::SetTextureMode(TextureMode mode)
{
	mTextureMode = mode;

	PX2_EDIT.GetTerrainEdit()->GetTextureProcess()->SetTextureMode(
		(TerrainTextureProcess::TextureMode)mode);
}
//----------------------------------------------------------------------------
EditObjectTerrain::TextureMode EditObjectTerrain::GetTextureMode() const
{
	return mTextureMode;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void EditObjectTerrain::RegistProperties()
{
	EditObject::RegistProperties();

	AddPropertyClass("EditObjectTerrain");

	std::vector<std::string> interpModes;
	interpModes.push_back("NONE");
	interpModes.push_back("LINER");
	interpModes.push_back("SQUARE");
	interpModes.push_back("COS");
	AddPropertyEnum("InterplateMode", (int)mInterplateMode, interpModes);

	AddPropertyFloatSlider("Size", GetBrushSize(), 0.0f, 10.0f);
	float strength = GetBrushStrength();
	AddPropertyFloatSlider("Strength", strength, 0.0f, 1.0f);

	std::vector<std::string> heightModes;
	heightModes.push_back("RAISE");
	heightModes.push_back("LOWER");
	heightModes.push_back("FLATTEN");
	heightModes.push_back("SMOOTH");
	heightModes.push_back("FIXH");
	heightModes.push_back("HOLE");
	heightModes.push_back("NOHOLE");
	AddPropertyEnum("HeightMode", (int)GetHeightMode(), heightModes);
	AddProperty("FixHeightVal", Object::PT_FLOAT, GetHeightModeFixHeight());

	AddProperty("SelectLayerIndex", Object::PT_INT, mSelectLayerIndex);

	std::vector<std::string> textureModes;
	textureModes.push_back("INCREASE");
	textureModes.push_back("DECREASE");
	textureModes.push_back("APPLY");
	textureModes.push_back("SMOOTH");
	textureModes.push_back("NOISE");
	AddPropertyEnum("TextureMode", (int)GetTextureMode(), textureModes);

	AddProperty("SelectLayerIndex", Object::PT_INT, mSelectLayerIndex);
	AddProperty("SelectLayerIndex", Object::PT_INT, mSelectLayerIndex);
	AddProperty("SelectLayerIndex", Object::PT_INT, mSelectLayerIndex);
	AddProperty("SelectLayerIndex", Object::PT_INT, mSelectLayerIndex);
	AddProperty("SelectLayerIndex", Object::PT_INT, mSelectLayerIndex);
	AddProperty("SelectLayerIndex", Object::PT_INT, mSelectLayerIndex);
}
//----------------------------------------------------------------------------
void EditObjectTerrain::OnPropertyChanged(const PropertyObject &obj)
{
	EditObject::OnPropertyChanged(obj);

	if ("InterplateMode" == obj.Name)
	{
		int mode = PX2_ANY_AS(obj.Data, int);
		SetInterplateMode((EditObjectTerrain::InterplateMode)mode);
	}
	else if ("Size" == obj.Name)
	{
		float size = PX2_ANY_AS(obj.Data, float);
		SetBrushSize(size);
	}
	else if ("Strength" == obj.Name)
	{
		float strength = PX2_ANY_AS(obj.Data, float);
		SetBrushStrength(strength);
	}
	else if ("HeightMode" == obj.Name)
	{
		int heightMode = PX2_ANY_AS(obj.Data, int);
		SetHeightMode((EditObjectTerrain::HeightMode)heightMode);
	}
	else if ("FixHeightVal" == obj.Name)
	{
		float fixHeightVal = PX2_ANY_AS(obj.Data, float);
		SetHeightModeFixHeight(fixHeightVal);
	}
	else if ("TextureMode" == obj.Name)
	{
		int textureMode = PX2_ANY_AS(obj.Data, int);
		SetTextureMode((EditObjectTerrain::TextureMode)textureMode);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EditObjectTerrain::EditObjectTerrain(LoadConstructor value) :
EditObject(value)
{
}
//----------------------------------------------------------------------------
void EditObjectTerrain::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	EditObject::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EditObjectTerrain, source);
}
//----------------------------------------------------------------------------
void EditObjectTerrain::Link(InStream& source)
{
	EditObject::Link(source);
}
//----------------------------------------------------------------------------
void EditObjectTerrain::PostLink()
{
	EditObject::PostLink();
}
//----------------------------------------------------------------------------
bool EditObjectTerrain::Register(OutStream& target) const
{
	if (EditObject::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void EditObjectTerrain::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	EditObject::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EditObjectTerrain, target);
}
//----------------------------------------------------------------------------
int EditObjectTerrain::GetStreamingSize(Stream &stream) const
{
	int size = EditObject::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------