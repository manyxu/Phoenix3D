// PX2EU_ResTree.cpp

#include "PX2EU_ResTree.hpp"
#include "PX2Dir.hpp"
#include "PX2EditEventType.hpp"
#include "PX2EditEventData.hpp"
#include "PX2Edit.hpp"
#include "PX2ResourceManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UITree, EU_ResTree);
PX2_IMPLEMENT_STREAM(EU_ResTree);
PX2_IMPLEMENT_FACTORY(EU_ResTree);

//----------------------------------------------------------------------------
EU_ResTree::EU_ResTree(ResTreeType rtt):
mResTreeType(rtt)
{
	if (RTT_DIR == mResTreeType)
	{
		ShowRootItem(false);
		mDataItem = AddItem(mRootItem, "Data", "Data");
		mDataItem->SetName("Data");
		mDataItem->SetUserData("path", std::string("Data/"));
		RefreshItems(mDataItem, RT_DIR, true);
	}

	ComeInEventWorld();
}
//----------------------------------------------------------------------------
EU_ResTree::~EU_ResTree()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void EU_ResTreeCallback(UIFrame *frame, UICallType type)
{
	UICheckButton *checkBut = DynamicCast<UICheckButton>(frame);
	if (checkBut)
	{
		UIItem *item = checkBut->GetUserData<UIItem*>("Item");

		if (UICT_CHECKED == type)
		{
			item->Expand(false);
		}
		else if (UICT_DISCHECKED == type)
		{
			item->Expand(true);
		}
	}
}
//----------------------------------------------------------------------------
UIItem *EU_ResTree::AddItem(UIItem *parentItem, const std::string &label,
	const std::string &name, Object *obj)
{
	UIItem *item = UITree::AddItem(parentItem, label, name, obj);
	UICheckButton *cb = item->CreateButArrow();
	cb->SetUICallback(EU_ResTreeCallback);
	cb->SetUserData("Item", item);

	cb->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture(
		"DataNIRVANA2/images/icons/tree/tree_expanded.png");
	cb->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetTexture(
		"DataNIRVANA2/images/icons/tree/tree_collapsed.png");

	return item;
}
//----------------------------------------------------------------------------
void EU_ResTree::OnSelected(UIItem *item)
{
	const std::string &path = item->GetUserData<std::string>("path");
	if (path.empty()) return;

	if (RTT_DIR == mResTreeType)
	{
		PX2_EDIT.SetSelectResDir(path);

		Event *ent = EditEventSpace::CreateEventX(EditEventSpace::SelectResDir);
		PX2_EW.BroadcastingLocalEvent(ent);
	}
}
//----------------------------------------------------------------------------
void EU_ResTree::DoExecute(Event *event)
{
	PX2_UNUSED(event);
}
//----------------------------------------------------------------------------
void EU_ResTree::RefreshItems(UIItem *parent, RefreshType type, bool isExpand)
{
	std::string path = parent->GetUserData<std::string>("path");
	if (path.empty())
		return;

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
		if (type == RT_DIR)
			flags = Dir::DIR_DIRS;
		else if (type == RT_DIR_FILES)
			flags = Dir::DIR_FILES;
		else if (type == RT_DIR_ALL || RT_DIR_ALL_NOCHILDREN)
			flags = Dir::DIR_DIRS | Dir::DIR_FILES;

		if (d.GetFirst(&eachFilename, "", flags))
		{
			do 
			{
				if ((eachFilename != ".") && (eachFilename != ".."))
				{
					std::string fileName = eachFilename;
					std::string nextPath = path + fileName;
					std::string nextPath1 = nextPath + "/";

					UIItem *itemChild = AddItem(parent, fileName, fileName);
					itemChild->SetName(fileName);
					itemChild->SetUserData("path", nextPath1);
					itemChild->SetUserData("filename", nextPath);
				}

			} while (d.GetNext(&eachFilename));
		}
	}

	if (type != RT_DIR_FILES && type != RT_DIR_ALL_NOCHILDREN)
	{
		for (int i = 0; i < parent->GetNumChildren(); i++)
		{
			UIItem *childItem = DynamicCast<UIItem>(parent->GetChild(i));
			if (childItem)
			{
				RefreshItems(childItem, type, false);
			}
		}
	}

	if (parent)
	{
		parent->Expand(isExpand);
	}
}
//----------------------------------------------------------------------------
bool EU_ResTree::_IsAFile(const std::string &filename)
{
	if (filename.find(".") != std::string::npos)
		return true;

	return false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_ResTree::EU_ResTree(LoadConstructor value) :
UITree(value)
{
}
//----------------------------------------------------------------------------
void EU_ResTree::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UITree::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_ResTree, source);
}
//----------------------------------------------------------------------------
void EU_ResTree::Link(InStream& source)
{
	UITree::Link(source);
}
//----------------------------------------------------------------------------
void EU_ResTree::PostLink()
{
	UITree::PostLink();
}
//----------------------------------------------------------------------------
bool EU_ResTree::Register(OutStream& target) const
{
	if (UITree::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_ResTree::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UITree::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_ResTree, target);
}
//----------------------------------------------------------------------------
int EU_ResTree::GetStreamingSize(Stream &stream) const
{
	int size = UITree::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------