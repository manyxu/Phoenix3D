// PX2EU_ResList.cpp

#include "PX2EU_ResList.hpp"
#include "PX2SelectResData.hpp"
#include "PX2Edit.hpp"
#include "PX2EditEventType.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2Dir.hpp"
#include "PX2EditorEventType.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIList, EU_ResList);
PX2_IMPLEMENT_STREAM(EU_ResList);
PX2_IMPLEMENT_FACTORY(EU_ResList);

//----------------------------------------------------------------------------
EU_ResList::EU_ResList()
{
	ComeInEventWorld();
}
//----------------------------------------------------------------------------
EU_ResList::~EU_ResList()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void EU_ResList::OnSelected(UIItem *item)
{
	UIList::OnSelected(item);

	std::string filename = item->GetUserData<std::string>("filename");

	if (filename.find(".png") != std::string::npos)
	{
		Object *obj = PX2_RM.BlockLoad(filename);

		SelectResData srd;
		srd.ResPathname = filename;
		srd.TheObject = obj;
		PX2_EDIT.SetSelectedResource(srd);

		if (obj)
		{
			PX2_EDIT.SetPreViewObject(obj);
		}
	}
}
//----------------------------------------------------------------------------
void EU_ResList::OnEvent(Event *event)
{
	if (EditorEventSpace::IsEqual(event, EditorEventSpace::ChangeResDir))
	{
		const std::string &selectResDir = PX2_EDIT.GetSelectedResDir();
		if (!selectResDir.empty())
		{
			mSelectPath = selectResDir;			

			RefreshItems(selectResDir);
		}
	}
}
//----------------------------------------------------------------------------
void EU_ResList::RefreshItems(const std::string &path)
{
	RemoveAllItems();

	if (mSelectPath.empty()) return;

	std::string name;
	std::string compareStr = "Data/";
	if (path == compareStr)
		name = "Data";
	else
	{
		name = path.substr(compareStr.length(),
			path.length() - compareStr.length() - 1);
	}

	Dir d;
	std::string eachFilename;
	if (d.Open(path))
	{
		if (!d.HasFiles() && !d.HasSubDirs())
			return;

		int flags = 0;
		flags = Dir::DIR_DIRS | Dir::DIR_FILES;

		if (d.GetFirst(&eachFilename, "", flags))
		{
			do
			{
				if ((eachFilename != ".") && (eachFilename != ".."))
				{
					std::string baseFilename = eachFilename;
					std::string pathFilename = path + baseFilename;
					std::string pathFilenamePath = pathFilename + "/";

					UIItem *itemChild = AddItem(baseFilename);
					itemChild->SetName(baseFilename);
					itemChild->SetUserData("filename", pathFilename);
					itemChild->SetUserData("path", pathFilenamePath);
				}

			} while (d.GetNext(&eachFilename));
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_ResList::EU_ResList(LoadConstructor value) :
UIList(value)
{
}
//----------------------------------------------------------------------------
void EU_ResList::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIList::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_ResList, source);
}
//----------------------------------------------------------------------------
void EU_ResList::Link(InStream& source)
{
	UIList::Link(source);
}
//----------------------------------------------------------------------------
void EU_ResList::PostLink()
{
	UIList::PostLink();
}
//----------------------------------------------------------------------------
bool EU_ResList::Register(OutStream& target) const
{
	if (UIList::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_ResList::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIList::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_ResList, target);
}
//----------------------------------------------------------------------------
int EU_ResList::GetStreamingSize(Stream &stream) const
{
	int size = UIList::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------