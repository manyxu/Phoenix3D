// PX2EU_PropertyGrid.cpp

#include "PX2EU_PropertyGridRes.hpp"
#include "PX2SelectionManager.hpp"
#include "PX2EditEventType.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2EditorEventType.hpp"
#include "PX2Edit.hpp"
#include "PX2ResourceManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIPropertyGrid, EU_PropertyGridRes);
PX2_IMPLEMENT_STREAM(EU_PropertyGridRes);
PX2_IMPLEMENT_FACTORY(EU_PropertyGridRes);

//----------------------------------------------------------------------------
EU_PropertyGridRes::EU_PropertyGridRes()
{
	ComeInEventWorld();
	ShowRootItem(false);
}
//----------------------------------------------------------------------------
EU_PropertyGridRes::~EU_PropertyGridRes()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void EU_PropertyGridRes::OnEvent(Event *event)
{
	if (EditorEventSpace::IsEqual(event, EditorEventSpace::SelectRes))
	{
		RemoveAllProperties();

		const SelectResData &selectResData = PX2_EDIT.GetSelectedResource();
		const std::string &resPathName = selectResData.ResPathname;

		std::string outPath;
		std::string outBaseName;
		std::string outExtention;

		StringHelp::SplitFullFilename(resPathName, outPath, outBaseName,
			outExtention);

		if ("png" == outExtention || "PNG" == outExtention)
		{
			Texture2D *tex2D = DynamicCast<Texture2D>(
				PX2_RM.BlockLoad(resPathName));
			if (tex2D)
			{				
				int texWidth = tex2D->GetWidth();
				int texHeight = tex2D->GetHeight();

				AddItemType(mRootItem, "ResPath", Object::PT_STRING, "ResPath",
					resPathName, resPathName, 0.0f, 0.0f, false);
				AddItemType(mRootItem, "Width", Object::PT_INT, "Width",
					texWidth, texWidth, 0.0f, 0.0f, false);
				AddItemType(mRootItem, "Height", Object::PT_INT, "Height",
					texHeight, texHeight, 0.0f, 0.0f, false);
			}
		}
		else
		{
			AddItemType(mRootItem, "ResPath", Object::PT_STRING, "ResPath",
				resPathName, resPathName, 0.0f, 0.0f, false);
		}
	}
}
//----------------------------------------------------------------------------
void EU_PropertyGridRes::OnItemChanged(UIItem *item)
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_PropertyGridRes::EU_PropertyGridRes(LoadConstructor value) :
UIPropertyGrid(value)
{
}
//----------------------------------------------------------------------------
void EU_PropertyGridRes::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIPropertyGrid::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_PropertyGridRes, source);
}
//----------------------------------------------------------------------------
void EU_PropertyGridRes::Link(InStream& source)
{
	UIPropertyGrid::Link(source);
}
//----------------------------------------------------------------------------
void EU_PropertyGridRes::PostLink()
{
	UIPropertyGrid::PostLink();
}
//----------------------------------------------------------------------------
bool EU_PropertyGridRes::Register(OutStream& target) const
{
	if (UIPropertyGrid::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_PropertyGridRes::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIPropertyGrid::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_PropertyGridRes, target);
}
//----------------------------------------------------------------------------
int EU_PropertyGridRes::GetStreamingSize(Stream &stream) const
{
	int size = UIPropertyGrid::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------