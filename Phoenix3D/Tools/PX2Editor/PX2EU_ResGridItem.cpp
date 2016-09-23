// PX2EU_ResGridItem.cpp

#include "PX2EU_ResGridItem.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2Edit.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2EU_ResGridFrame.hpp"
#include "PX2EditorEventType.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_ResGridItem);
PX2_IMPLEMENT_STREAM(EU_ResGridItem);
PX2_IMPLEMENT_FACTORY(EU_ResGridItem);

//----------------------------------------------------------------------------
Sizef EU_ResGridItem::sGridItemSize = Sizef(70.0f, 108.0f);
UIFramePtr EU_ResGridItem::sLastSelectItemBut = 0;
//----------------------------------------------------------------------------
EU_ResGridItem::EU_ResGridItem()
{
	mButton = new0 UIButton();
	AttachChild(mButton);
	mButton->LocalTransform.SetTranslateY(-1.0f);
	mButton->SetAnchorHor(0.0f, 1.0f);
	mButton->SetAnchorParamHor(0.0f, 0.0f);
	mButton->SetAnchorVer(0.0f, 1.0f);
	mButton->SetAnchorParamVer(0.0f, 1.0f);
	mButton->SetStateAlpha(UIButtonBase::BS_NORMAL, 0.0f);
	mButton->SetStateAlpha(UIButtonBase::BS_HOVERED, 1.0f);
	mButton->SetStateAlpha(UIButtonBase::BS_PRESSED, 1.0f);
	mButton->SetActivateColor(PX2_UISM.Color_SelectItem);
	mButton->SetMemUICallback(this, (UIFrame::MemUICallback)(&EU_ResGridItem
		::_ButCallback));

	float minusSize = 4.0f;
	Sizef picBoxSize(sGridItemSize.Width - minusSize, sGridItemSize.Width - minusSize);

	mResPicBox = new0 UIFPicBox();
	AttachChild(mResPicBox);
	mResPicBox->LocalTransform.SetTranslateY(-5.0f);
	mResPicBox->SetAnchorHor(0.5f, 0.5f);
	mResPicBox->SetAnchorVer(1.0f, 1.0f);
	mResPicBox->SetAnchorParamVer(-minusSize - picBoxSize.Height / 2.0f, 0.0f);
	mResPicBox->SetSize(picBoxSize);
	mResPicBox->SetDoPick(false);

	mText = new0 UIFText();
	AttachChild(mText);
	mText->LocalTransform.SetTranslateY(-4.0f);
	mText->SetAnchorHor(0.0f, 1.0f);
	mText->SetAnchorParamHor(minusSize, - minusSize);
	mText->SetAnchorVer(0.0f, 1.0f);
	mText->SetAnchorParamVer(minusSize, -minusSize - picBoxSize.Height - minusSize);
	mText->SetSize(0.0f, 20.0f);
	mText->GetText()->SetFontScale(0.7f);
	mText->GetText()->SetFontColor(Float3::WHITE);
	mText->GetText()->SetRectUseage(UIText::RU_CLIPWARP);
	mText->GetText()->SetAutoWarp(true);
	mText->GetText()->SetPointAsPunctuation(false);
	mText->SetDoPick(false);
}
//----------------------------------------------------------------------------
EU_ResGridItem::~EU_ResGridItem()
{
}
//----------------------------------------------------------------------------
void EU_ResGridItem::_ButCallback(UIFrame *frame, UICallType type)
{
	if (UICT_PRESSED == type)
	{
		EU_ResGridItem *item = DynamicCast<EU_ResGridItem>(frame->GetParent());
		if (item)
		{
			std::string pathFilenamePath = item->GetUserData<std::string>("PathFilenamePath");
			std::string pathFilename = item->GetUserData<std::string>("PathFilename");

			SelectResData srd;
			srd.ResPathname = pathFilename;

			Object *obj = 0;

			if (pathFilename.find(".png") != std::string::npos)
			{
				obj = PX2_RM.BlockLoad(pathFilename);
				srd.TheObject = obj;
			}

			if (obj)
			{
				PX2_EDIT.SetPreViewObject(obj);
			}

			PX2_EDIT.SetSelectedResource(srd);

			auto pos = pathFilenamePath.find(".");
			if (pos == pathFilenamePath.npos)
			{
				PX2_EDIT.ChangeSelectResDir(pathFilenamePath, Edit::CDT_ONLYSELECT);
			}
		}

		if (sLastSelectItemBut)
		{
			sLastSelectItemBut->SetActivate(false);
		}

		sLastSelectItemBut = frame;
		sLastSelectItemBut->SetActivate(true);
	}
	else if (UICT_DOUBLE_PRESSED == type)
	{
		EU_ResGridItem *item = DynamicCast<EU_ResGridItem>(frame->GetParent());
		if (item)
		{
			std::string pathFilenamePath = item->GetUserData<std::string>("PathFilenamePath");
			std::string pathFilename = item->GetUserData<std::string>("PathFilename");

			auto pos = pathFilenamePath.find(".");
			if (pos == pathFilenamePath.npos)
			{
				EU_ResGridFrame *gridFrame = GetFirstParentDerivedFromType<
					EU_ResGridFrame>();
				if (gridFrame)
				{
					PX2_EDIT.ChangeSelectResDir(pathFilenamePath, Edit::CDT_GRID_REFRESH);
				}
			}
			else
			{
				std::string outPath;
				std::string outBaseName;
				std::string outExt;
				StringHelp::SplitFullFilename(pathFilename, outPath, 
					outBaseName, outExt);
				if ("txt" == outExt || "as" == outExt || "lua" == outExt
					|| "xml" == outExt || "png"==outExt)
				{
					Event *ent = EditorEventSpace::CreateEventX(EditorEventSpace::OpenRes);
					ent->SetData<std::string>(pathFilename);
					PX2_EW.BroadcastingLocalEvent(ent);
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void EU_ResGridItem::SetPathFilename(const std::string &pathFilename)
{
	Texture2D *tex2D = 0;
	if (pathFilename.find(".") == std::string::npos)
	{ // folder
		Object *obj = PX2_RM.BlockLoad(
			"DataNIRVANA2/images/icons/res/folder.png");
		tex2D = DynamicCast<Texture2D>(obj);
	}
	else if (pathFilename.find(".png") != std::string::npos)
	{ // tex
		Object *obj = PX2_RM.BlockLoad(pathFilename);
		tex2D = DynamicCast<Texture2D>(obj);
	}
	else
	{
		Object *obj = PX2_RM.BlockLoad("Data/engine/white.png");
		tex2D = DynamicCast<Texture2D>(obj);
	}
	mResPicBox->GetUIPicBox()->SetTexture(tex2D);

	std::string filename = GetUserData<std::string>("BaseFilename");
	mText->GetText()->SetText(filename);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_ResGridItem::EU_ResGridItem(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_ResGridItem::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_ResGridItem, source);
}
//----------------------------------------------------------------------------
void EU_ResGridItem::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_ResGridItem::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_ResGridItem::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_ResGridItem::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_ResGridItem, target);
}
//----------------------------------------------------------------------------
int EU_ResGridItem::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------