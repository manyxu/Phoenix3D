// PX2UIPropertyItem.hpp

#include "PX2UIPropertyItem.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UIPropertyGrid.hpp"
#include "PX2UIComboBox.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIItem, UIPropertyItem);
PX2_IMPLEMENT_STREAM(UIPropertyItem);
PX2_IMPLEMENT_FACTORY(UIPropertyItem);
PX2_IMPLEMENT_DEFAULT_NAMES(UIItem, UIPropertyItem);

//----------------------------------------------------------------------------
UIPropertyItem::UIPropertyItem() :
mObjectID(0)
{
}
//----------------------------------------------------------------------------
UIPropertyItem::~UIPropertyItem()
{
}
//----------------------------------------------------------------------------
void UIPropertyGridClassCBCallback(UIFrame *frame, UICallType type)
{
	UICheckButton *checkBut = DynamicCast<UICheckButton>(frame);
	if (checkBut)
	{
		UIItem *item = (UIItem*)checkBut->GetUserData<Object*>("Item");

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
void EditBoxCallback(UIFrame *frame, UICallType type)
{
	UIEditBox *eb = DynamicCast<UIEditBox>(frame);
	if (eb && type == UICT_EDITBOX_ENTER)
	{
		const std::string &text = eb->GetText();
		Object *itemObj = eb->GetUserData<Object*>("item");
		UIItem *item = DynamicCast<UIItem>(itemObj);
		if (item)
		{
			Object::PropertyType pt = eb->GetUserData<Object::PropertyType>("datatype");
			if (Object::PT_STRING == pt)
			{
				item->SetValue(text);
			}
			else if (Object::PT_INT == pt)
			{
				int iVal = StringHelp::StringToInt(text);
				item->SetValue(iVal);
			}
			else if (Object::PT_FLOAT == pt)
			{
				float fVal = (float)atof(text.c_str());
				item->SetValue(fVal);
			}

			UIPropertyGrid *grid =
				item->GetFirstParentDerivedFromType<UIPropertyGrid>();
			if (grid)
			{
				grid->OnItemChanged(item);
			}
		}
	}
}
//----------------------------------------------------------------------------
void UIPropertyItem::OnAllocAll(const Any &userDat)
{
	UIButton *butBack = GetButBack();
	if (butBack) butBack->LocalTransform.SetTranslateY(-1.0f);

	UIFText *fText = GetFText();
	if (fText) fText->LocalTransform.SetTranslateY(-3.0f);

	ItemType itemType = PX2_ANY_AS(userDat, ItemType);
	if (itemType == IT_NONE)
	{
		/*_*/
	}
	else if (itemType == IT_CLASS)
	{
		UIButton *butBack = GetButBack();
		if (butBack) butBack->LocalTransform.SetTranslateY(-10.0f);

		UICheckButton *cb = CreateButArrow();
		if (cb) cb->LocalTransform.SetTranslateY(-12.0f);
		UIFText *fText = GetFText();
		if (fText) fText->LocalTransform.SetTranslateY(-12.0f);

		cb->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture(
			"DataNIRVANA2/images/icons/tree/tree_expanded.png");
		cb->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetTexture(
			"DataNIRVANA2/images/icons/tree/tree_collapsed.png");

		cb->SetUICallback(UIPropertyGridClassCBCallback);
		cb->SetUserData("Item", (Object*)this);

		if (butBack)
		{
			butBack->SetStateColor(UIButtonBase::BS_NORMAL,
				PX2_UISM.Color_PropertyClass);
			butBack->SetStateBrightness(UIButtonBase::BS_NORMAL, 1.0f);

			butBack->SetStateColor(UIButtonBase::BS_HOVERED,
				PX2_UISM.Color_PropertyClass);
			butBack->SetStateBrightness(UIButtonBase::BS_HOVERED, 1.2f);
		}
	}
	else if (itemType == IT_COMBOX)
	{
		UIComboBox *comboBox = new0 UIComboBox();
		AttachChild(comboBox);
		comboBox->LocalTransform.SetTranslateY(-2.0f);
		comboBox->GetChooseList()->GetBackgroundPicBox()->SetColor(
			PX2_UISM.Color_ViewBackground);
		mEditCtrl = comboBox;
	}
	else if (itemType == IT_EDITBOX)
	{
		UIEditBox *editBox = new0 UIEditBox();
		AttachChild(editBox);
		editBox->LocalTransform.SetTranslateY(-2.0f);
		mEditCtrl = editBox;

		UIFPicBox *picBoxBack = editBox->GetBackPicBox();
		if (picBoxBack) picBoxBack->SetColor(PX2_UISM.Color_ContentBackground);
		editBox->GetInputText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);

		editBox->SetUICallback(EditBoxCallback);
		editBox->SetUserData("item", (Object*)this);

		editBox->GetInputText()->SetFontColor(PX2_UISM.Color_ContentFont);
	}
	else if (itemType == IT_EDITBOX2)
	{
		UIFrame *editFrame = new0 UIFrame();
		AttachChild(editFrame);
		editFrame->LocalTransform.SetTranslateY(-1.0f);
		mEditCtrl = editFrame;

		UIEditBox *eb0 = new0 UIEditBox();
		editFrame->AttachChild(eb0);
		eb0->LocalTransform.SetTranslateY(-1.0f);
		eb0->SetAnchorHor(0.0f, 1.0f / 2.0f);
		eb0->SetAnchorParamHor(0.5f, -0.5f);
		eb0->SetAnchorVer(0.0f, 1.0f);
		UIFPicBox *picBoxBack0 = eb0->GetBackPicBox();
		if (picBoxBack0) picBoxBack0->SetColor(PX2_UISM.Color_ContentBackground);
		eb0->GetInputText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);
		eb0->GetInputText()->SetFontColor(PX2_UISM.Color_ContentFont);

		UIEditBox *eb1 = new0 UIEditBox();
		editFrame->AttachChild(eb1);
		eb1->LocalTransform.SetTranslateY(-1.0f);
		eb1->SetAnchorHor(1.0f / 2.0f, 2.0f / 2.0f);
		eb1->SetAnchorParamHor(0.5f, -0.5f);
		eb1->SetAnchorVer(0.0f, 1.0f);
		UIFPicBox *picBoxBack1 = eb1->GetBackPicBox();
		if (picBoxBack1) picBoxBack1->SetColor(PX2_UISM.Color_ContentBackground);
		eb0->GetInputText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);
		eb0->GetInputText()->SetFontColor(PX2_UISM.Color_ContentFont);
		eb1->GetInputText()->SetFontColor(PX2_UISM.Color_ContentFont);
	}
	else if (itemType == IT_EDITBOX3)
	{
		UIFrame *editFrame = new0 UIFrame();
		AttachChild(editFrame);
		editFrame->LocalTransform.SetTranslateY(-1.0f);
		mEditCtrl = editFrame;

		UIEditBox *eb0 = new0 UIEditBox();
		editFrame->AttachChild(eb0);
		eb0->LocalTransform.SetTranslateY(-1.0f);
		eb0->SetAnchorHor(0.0f, 1.0f / 3.0f);
		eb0->SetAnchorParamHor(0.5f, -0.5f);
		eb0->SetAnchorVer(0.0f, 1.0f);
		UIFPicBox *picBoxBack0 = eb0->GetBackPicBox();
		if (picBoxBack0) picBoxBack0->SetColor(PX2_UISM.Color_ContentBackground);
		eb0->GetInputText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);
		eb0->GetInputText()->SetFontColor(PX2_UISM.Color_ContentFont);

		UIEditBox *eb1 = new0 UIEditBox();
		editFrame->AttachChild(eb1);
		eb1->LocalTransform.SetTranslateY(-1.0f);
		eb1->SetAnchorHor(1.0f / 3.0f, 2.0f / 3.0f);
		eb1->SetAnchorParamHor(0.5f, -0.5f);
		eb1->SetAnchorVer(0.0f, 1.0f);
		UIFPicBox *picBoxBack1 = eb1->GetBackPicBox();
		if (picBoxBack1) picBoxBack1->SetColor(PX2_UISM.Color_ContentBackground);
		eb1->GetInputText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);
		eb1->GetInputText()->SetFontColor(PX2_UISM.Color_ContentFont);

		UIEditBox *eb2 = new0 UIEditBox();
		editFrame->AttachChild(eb2);
		eb2->LocalTransform.SetTranslateY(-1.0f);
		eb2->SetAnchorHor(2.0f / 3.0f, 3.0f / 3.0f);
		eb2->SetAnchorParamHor(0.5f, -0.5f);
		eb2->SetAnchorVer(0.0f, 1.0f);
		UIFPicBox *picBoxBack2 = eb2->GetBackPicBox();
		if (picBoxBack2) picBoxBack2->SetColor(PX2_UISM.Color_ContentBackground);
		eb2->GetInputText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);
		eb2->GetInputText()->SetFontColor(PX2_UISM.Color_ContentFont);
	}
	else if (itemType == IT_TRANSFORM)
	{
		/*_*/
	}

	ResetItem();
}
//----------------------------------------------------------------------------
UIFrame *UIPropertyItem::GetEditCtrl()
{
	return mEditCtrl;
}
//----------------------------------------------------------------------------
void UIPropertyItem::OnAlloc()
{
}
//----------------------------------------------------------------------------
void UIPropertyItem::OnFree()
{
	ResetItem();
}
//----------------------------------------------------------------------------
void UIPropertyItem::SetObjectID(uint32_t id)
{
	mObjectID = id;
}
//----------------------------------------------------------------------------
uint32_t UIPropertyItem::GetObjectID()
{
	return mObjectID;
}
//----------------------------------------------------------------------------
void UIPropertyItem::SetItemType(ItemType it, Object::PropertyType propType,
	const Any &data)
{
	mPropertyType = propType;
	mItemType = it;

	UIEditBox *editBox = DynamicCast<UIEditBox>(mEditCtrl);
	UIComboBox *comboBox = DynamicCast<UIComboBox>(mEditCtrl);
	
	// PT_FLOAT,
	// PT_BOOL,
	// PT_BOOLCHECK,
	// PT_COLOR3FLOAT3,
	// PT_FLOAT2,
	//	PT_FLOAT3,
	//	PT_FLOAT4,
	//	PT_APOINT3,
	//	PT_APOINT4,
	//	PT_AVECTOR3,
	//	PT_AVECTOR4,
	//	PT_SIZE,
	//	PT_RECT,
	//	PT_STRING,
	//	PT_STRINGBUTTON,
	//	PT_ENUM,
	//	PT_TRANSFORM,
	//	PT_EMFLOAT,
	//	PT_EMFLOATRANGE,
	//	PT_EMFLOAT3,
	//	PT_EMFLOAT3RANGE,
	//	PT_EMFLOAT3COLOR,
	//	PT_EMFLOAT3COLORRANGE,
	//	PT_CLASS,
	if (PT_INT == propType)
	{
		int val = PX2_ANY_AS(data, int);

		std::string strVal = StringHelp::IntToString(val);
		editBox->SetText(strVal);
		editBox->SetUserData("datatype", Object::PT_INT);
	}
	else if (PT_FLOAT == propType)
	{
		float val = PX2_ANY_AS(data, float);

		std::string strVal = StringHelp::FloatToString(val, 2);
		editBox->SetText(strVal);
		editBox->SetUserData("datatype", Object::PT_FLOAT);
	}
	else if (Object::PT_FLOAT3 == propType || Object::PT_APOINT3 == propType ||
		Object::PT_AVECTOR3 == propType || Object::PT_COLOR3FLOAT3 == propType)
	{
		float val0 = 0.0f;
		float val1 = 0.0f;
		float val2 = 0.0f;

		if (Object::PT_FLOAT3 == propType)
		{
			Float3 val = PX2_ANY_AS(data, Float3);
			val0 = val[0];
			val1 = val[1];
			val2 = val[2];
		}
		else if (Object::PT_APOINT3 == propType)
		{
			APoint val = PX2_ANY_AS(data, APoint);
			val0 = val[0];
			val1 = val[1];
			val2 = val[2];
		}
		else if (Object::PT_AVECTOR3 == propType)
		{
			AVector val = PX2_ANY_AS(data, AVector);
			val0 = val[0];
			val1 = val[1];
			val2 = val[2];
		}
		else if (Object::PT_COLOR3FLOAT3 == propType)
		{
			Float3 val = PX2_ANY_AS(data, Float3);
			val0 = val[0];
			val1 = val[1];
			val2 = val[2];
		}

		UIEditBox *eb0 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(0));
		eb0->SetText(StringHelp::FloatToString(val0, 2));
		eb0->SetUserData("datatype", Object::PT_FLOAT);

		UIEditBox *eb1 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(1));
		eb1->SetText(StringHelp::FloatToString(val1, 2));
		eb1->SetUserData("datatype", Object::PT_FLOAT);

		UIEditBox *eb2 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(2));
		eb2->SetText(StringHelp::FloatToString(val2, 2));
		eb2->SetUserData("datatype", Object::PT_FLOAT);
	}
	else if (Object::PT_FLOAT4 == propType || Object::PT_APOINT4 == propType ||
		Object::PT_AVECTOR4 == propType)
	{
		float val0 = 0.0f;
		float val1 = 0.0f;
		float val2 = 0.0f;

		if (Object::PT_FLOAT4 == propType)
		{
			Float4 val = PX2_ANY_AS(data, Float4);
			val0 = val[0];
			val1 = val[1];
			val2 = val[2];
		}
		else if (Object::PT_APOINT4 == propType)
		{
			APoint val = PX2_ANY_AS(data, APoint);
			val0 = val[0];
			val1 = val[1];
			val2 = val[2];
		}
		else if (Object::PT_AVECTOR4 == propType)
		{
			AVector val = PX2_ANY_AS(data, AVector);
			val0 = val[0];
			val1 = val[1];
			val2 = val[2];
		}

		UIEditBox *eb0 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(0));
		eb0->SetText(StringHelp::FloatToString(val0, 2));
		eb0->SetUserData("datatype", Object::PT_FLOAT);

		UIEditBox *eb1 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(1));
		eb1->SetText(StringHelp::FloatToString(val1, 2));
		eb1->SetUserData("datatype", Object::PT_FLOAT);

		UIEditBox *eb2 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(2));
		eb2->SetText(StringHelp::FloatToString(val2, 2));
		eb2->SetUserData("datatype", Object::PT_FLOAT);
	}
	else if (PT_BOOL == propType || PT_BOOLCHECK == propType)
	{
		bool bl = PX2_ANY_AS(data, bool);

		comboBox->RemoveAllChooses();
		comboBox->AddChooseStr("true");
		comboBox->AddChooseStr("false");
		comboBox->SetChooseListHeightSameWithChooses();
		if (bl) comboBox->Choose(0);
		else comboBox->Choose(1);
	}
	else if (PT_STRING == propType || PT_STRINGBUTTON == propType)
	{
		std::string val = PX2_ANY_AS(data, std::string);
		editBox->SetText(val);
		editBox->SetUserData("datatype", Object::PT_STRING);
	}
	else if (PT_TRANSFORM == propType)
	{

	}
}
//----------------------------------------------------------------------------
UIPropertyItem::ItemType UIPropertyItem::GetItemType() const
{
	return mItemType;
}
//----------------------------------------------------------------------------
void UIPropertyItem::ResetItem()
{
	RemoveAllChildItems();

	mIsNeedRecal = true;
	mIsNumAllChildExpandNeedRecal = true;

	if (IT_CLASS != mItemType)
	{
		SetLevelAdjust(-0.25f);
	}

	SetActivate(false);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIPropertyItem::UIPropertyItem(LoadConstructor value) :
UIItem(value)
{
}
//----------------------------------------------------------------------------
void UIPropertyItem::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIItem::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIPropertyItem, source);
}
//----------------------------------------------------------------------------
void UIPropertyItem::Link(InStream& source)
{
	UIItem::Link(source);

	if (mIPTCtrl)
		source.ResolveLink(mIPTCtrl);
}
//----------------------------------------------------------------------------
void UIPropertyItem::PostLink()
{
	UIItem::PostLink();
}
//----------------------------------------------------------------------------
bool UIPropertyItem::Register(OutStream& target) const
{
	if (UIItem::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIPropertyItem::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIItem::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIItem, target);
}
//----------------------------------------------------------------------------
int UIPropertyItem::GetStreamingSize(Stream &stream) const
{
	int size = UIItem::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------