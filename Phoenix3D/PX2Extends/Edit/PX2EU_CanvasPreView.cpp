// PX2EU_CanvasPreView.cpp

#include "PX2EU_CanvasPreView.hpp"
#include "PX2Project.hpp"
#include "PX2SimulationEventType.hpp"
#include "PX2EditEventType.hpp"
#include "PX2Edit.hpp"
#include "PX2ResourceManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UICanvas, EU_CanvasPreView);
PX2_IMPLEMENT_STREAM(EU_CanvasPreView);
PX2_IMPLEMENT_FACTORY(EU_CanvasPreView);

//----------------------------------------------------------------------------
EU_CanvasPreView::EU_CanvasPreView()
{
	SetName("EU_CanvasPreView");

	EnableAnchorLayout(true);
	ComeInEventWorld();

	mUIFPicBox = new0 UIFPicBox();
	AttachChild(mUIFPicBox);
	mUIFPicBox->SetAnchorHor(0.5f, 0.5f);
	mUIFPicBox->SetAnchorVer(0.5f, 0.5f);
	mUIFPicBox->Show(false);

	float height = 20.0f;
	mFText = new0 UIFText();
	AttachChild(mFText);
	mFText->SetName("PreObjectText");
	mFText->SetAnchorHor(0.0f, 1.0f);
	mFText->SetAnchorVer(1.0f, 1.0f);
	mFText->SetAnchorParamVer(-height/2.0f, 0.0f);
	mFText->SetSize(0.0f, height);
	mFText->GetText()->SetFontScale(0.8f);
	mFText->GetText()->SetAligns(TEXTALIGN_LEFT|TEXTALIGN_VCENTER);
	mFText->GetText()->SetText("");

	mPreViewType = PVT_NONE;
	mPreViewTextureMode = PVTM_AUTO;
}
//----------------------------------------------------------------------------
EU_CanvasPreView::~EU_CanvasPreView()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void EU_CanvasPreView::Draw(Renderer *renderer)
{
	UICanvas::Draw(renderer);
}
//----------------------------------------------------------------------------
void EU_CanvasPreView::DoExecute(Event *event)
{
	if (EditEventSpace::IsEqual(event, EditEventSpace::SetPreViewObject))
	{
		PX2::Object *obj = event->GetData<PX2::Object*>();
		SetObject(obj);
	}
}
//----------------------------------------------------------------------------
void EU_CanvasPreView::UpdateWorldData(double applicationTime,
	double elapsedTime)
{
	UICanvas::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void EU_CanvasPreView::_ReSizeTexture()
{
	if (!mUIFPicBox) return;

	float texWidth = 128.0f;
	float texHeight = 128.0f;

	UIPicBox::PicBoxTexMode tm = mUIFPicBox->GetUIPicBox()->GetPicBoxTexMode();
	if (UIPicBox::PBTM_TEX == tm)
	{
		const std::string &texFilename = mUIFPicBox->GetUIPicBox()
			->GetTextureFilename();
		if (!texFilename.empty())
		{
			Texture2D *tex = DynamicCast<Texture2D>(PX2_RM.BlockLoad(
				texFilename));

			if (tex)
			{
				texWidth = (float)tex->GetWidth();
				texHeight = (float)tex->GetHeight();
			}
		}
	}
	else if (UIPicBox::PBTM_TEXPACK_ELE == tm)
	{
		const TexPackElement &element = PX2_RM.GetTexPackElement(
			mUIFPicBox->GetUIPicBox()->GetTexturePackName(),
			mUIFPicBox->GetUIPicBox()->GetElementName());
		if (element.IsValid())
		{
			texWidth = (float)element.W;
			texHeight = (float)element.H;
		}
	}

	if (PVTM_AUTO == mPreViewTextureMode)
	{
		if (texWidth <= mSize.Width && texHeight <= mSize.Height)
		{
			_SetTexSizeRealSize(texWidth, texHeight);
		}
		else
		{
			_SetTexSizeExtend(texWidth, texHeight);
		}
	}
	else if (PVTM_EXTEND == mPreViewTextureMode)
	{
		_SetTexSizeExtend(texWidth, texHeight);
	}
	else if (PVTM_REALSIZE == mPreViewTextureMode)
	{
		_SetTexSizeRealSize(texWidth, texHeight);
	}
}
//----------------------------------------------------------------------------
void EU_CanvasPreView::_SetTexSizeExtend(float texWidth, float texHeight)
{
	float texWidthOverHeight = texWidth / texHeight;
	float sizeWidthOverHeight = mSize.Width / mSize.Height;

	if (texWidthOverHeight > sizeWidthOverHeight)
		mUIFPicBox->SetSize(mSize.Width, mSize.Width / texWidthOverHeight);
	else
		mUIFPicBox->SetSize(mSize.Height * texWidthOverHeight, mSize.Height);
}
//----------------------------------------------------------------------------
void EU_CanvasPreView::_SetTexSizeRealSize(float texWidth, float texHeight)
{
	mUIFPicBox->SetSize(texWidth, texHeight);
}
//----------------------------------------------------------------------------
void EU_CanvasPreView::OnSizeChanged()
{
	UICanvas::OnSizeChanged();

	if (PVT_TEXTURE == mPreViewType)
	{
		_ReSizeTexture();
	}
}
//----------------------------------------------------------------------------
void EU_CanvasPreView::SetObject(Object *obj)
{
	Texture2D *tex2D = DynamicCast<Texture2D>(obj);
	Movable *mov = DynamicCast<Movable>(obj);

	if (tex2D)
	{
		mPreViewType = PVT_TEXTURE;
		mUIFPicBox->Show(true);

		const SelectResData &data = PX2_EDIT.GetSelectedResource();
		SelectResData::SelectResType selectResType = data.GetSelectResType();

		std::string texStr = "";
		if (selectResType == SelectResData::RT_NORMAL)
		{
			mUIFPicBox->GetUIPicBox()->SetTexture(data.ResPathname);

			texStr = "width:" + StringHelp::IntToString((int)tex2D->GetWidth()) + " "
				+ "height:" + StringHelp::IntToString((int)tex2D->GetHeight()) + " ";
		}

		mFText->GetText()->SetText(texStr);

		_ReSizeTexture();
	}
	else if (mov)
	{
		mPreViewType = PVT_MODEL;
		mUIFPicBox->Show(false);
	}
	else
	{
		mPreViewType = PVT_NONE;
		mUIFPicBox->Show(false);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_CanvasPreView::EU_CanvasPreView(LoadConstructor value) :
UICanvas(value)
{
}
//----------------------------------------------------------------------------
void EU_CanvasPreView::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UICanvas::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_CanvasPreView, source);
}
//----------------------------------------------------------------------------
void EU_CanvasPreView::Link(InStream& source)
{
	UICanvas::Link(source);
}
//----------------------------------------------------------------------------
void EU_CanvasPreView::PostLink()
{
	UICanvas::PostLink();
}
//----------------------------------------------------------------------------
bool EU_CanvasPreView::Register(OutStream& target) const
{
	if (UICanvas::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_CanvasPreView::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UICanvas::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_CanvasPreView, target);
}
//----------------------------------------------------------------------------
int EU_CanvasPreView::GetStreamingSize(Stream &stream) const
{
	int size = UICanvas::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------