// PX2EU_GridFrame.cpp

#include "PX2EU_GridFrame.hpp"
#include "PX2Edit.hpp"
#include "PX2EditEventType.hpp"
#include "PX2Dir.hpp"
#include "PX2EU_ResGridItem.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIGridFrame, EU_GridFrame);
PX2_IMPLEMENT_STREAM(EU_GridFrame);
PX2_IMPLEMENT_FACTORY(EU_GridFrame);

//----------------------------------------------------------------------------
EU_GridFrame::EU_GridFrame()
{
	ComeInEventWorld();

	UIFrameGridAlignControl *ctrl = mContentFrame->GetGridAlignCtrl();
	if (ctrl)
	{
		ctrl->SetBorder(2.0f, 2.0f, 2.0f, 2.0f);
		ctrl->SetCellSize(EU_ResGridItem::sGridItemSize);
	}
}
//----------------------------------------------------------------------------
EU_GridFrame::~EU_GridFrame()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void EU_GridFrame::DoExecute(Event *event)
{
	if (EditEventSpace::IsEqual(event, EditEventSpace::SelectResDir))
	{
		const std::string &selectResDir = PX2_EDIT.GetSelectResDir();
		if (!selectResDir.empty())
		{
			mSelectPath = selectResDir;

			RefreshItems(selectResDir);
		}
	}
}
//----------------------------------------------------------------------------
void EU_GridFrame::RefreshItems(const std::string &path)
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

					EU_ResGridItem *itemChild = new0 EU_ResGridItem();
					AddItem(itemChild);
					itemChild->SetUserData("BaseFilename", baseFilename);
					itemChild->SetUserData("PathFilename", pathFilename);
					itemChild->SetUserData("PathFilenamePath", pathFilenamePath);
					itemChild->SetPathFilename(pathFilename);
				}

			} while (d.GetNext(&eachFilename));
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_GridFrame::EU_GridFrame(LoadConstructor value) :
UIGridFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_GridFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIGridFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_GridFrame, source);
}
//----------------------------------------------------------------------------
void EU_GridFrame::Link(InStream& source)
{
	UIGridFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_GridFrame::PostLink()
{
	UIGridFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_GridFrame::Register(OutStream& target) const
{
	if (UIGridFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_GridFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIGridFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_GridFrame, target);
}
//----------------------------------------------------------------------------
int EU_GridFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIGridFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------