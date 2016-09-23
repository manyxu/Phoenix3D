// PX2CurveGroupUI.cpp

#include "PX2UICurveGroup.hpp"
#include "PX2Edit.hpp"
#include "PX2UICanvas.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2SelectionManager.hpp"
using namespace PX2;
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UICurveGroup);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, UICurveGroup);

UICurveGroup::UICurveGroup (CurveGroup *group) :
mCurveGroup(group)
{
	CurveGroup::CGType cgType = mCurveGroup->GetCGType();
	
	SetName("UICurveGroup");

	mIsShowBox0 = true;
	mIsShowBox1 = true;
	mIsShowBox2 = true;
	mIsShowBox3 = true;
	mIsShowBox4 = true;
	mIsShowBox5 = true;
	mIsShowBox = true;

	Sizef sizeBackground(TimeLineLeftWidth, 36.0f);
	SetSize(sizeBackground);

	mFBackground = new0 UIFPicBox();
	mFBackground->SetAnchorHor(0.0f, 1.0f);
	mFBackground->SetAnchorVer(0.0f, 1.0f);
	mFBackground->SetColor(Float3(0.659f, 0.659f, 0.659f));
	mFBackground->GetUIPicBox()->SetPicBoxType(UIPicBox::PBT_NINE);
	mFBackground->GetUIPicBox()->SetTexCornerSize(Sizef(2.0f, 2.0f), Sizef(2.0f, 2.0f));
	mFBackground->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/smallpic1.png");
	AttachChild(mFBackground);
	mFBackground->LocalTransform.SetTranslateY(-0.5f);

	float boxWidth = 8.0f;
	float boxSpace = 12.0f;
	Sizef sizeBox(boxWidth, boxWidth);
	float boxXStart = 4.0f;
	float boxZPos = 4.0f;
	mBox0 = new0 UIFPicBox();
	mBox0->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/smallpic.png");
	mBox0->SetColor(Float3::RED);
	mBox0->SetSize(sizeBox);
	mBox0->SetAnchorHor(0.0f, 0.0f);
	mBox0->SetAnchorVer(0.0f, 0.0f);
	mBox0->SetAnchorParamHor(boxXStart, boxXStart);
	mBox0->SetAnchorParamVer(boxZPos, boxZPos);
	mBox0->LocalTransform.SetTranslateY(-3.0f);
	AttachChild(mBox0);

	if (CurveGroup::CGT_FLOAT != cgType)
	{
		mBox1 = new0 UIFPicBox();
		mBox1->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/smallpic.png");
		mBox1->SetColor(Float3::GREEN);
		mBox1->SetSize(sizeBox);
		mBox1->SetAnchorHor(0.0f, 0.0f);
		mBox1->SetAnchorVer(0.0f, 0.0f);
		mBox1->SetAnchorParamHor(boxXStart + boxSpace, 0.0f);
		mBox1->SetAnchorParamVer(boxZPos, 0.0f);
		mBox1->LocalTransform.SetTranslateY(-3.0f);

		AttachChild(mBox1);
	}

	if (CurveGroup::CGT_FLOAT!=cgType && CurveGroup::CGT_FLOATRANGE!=cgType)
	{
		mBox2 = new0 UIFPicBox();
		mBox2->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/smallpic.png");
		mBox2->SetColor(Float3::BLUE);
		mBox2->SetSize(sizeBox);
		mBox2->SetAnchorHor(0.0f, 0.0f);
		mBox2->SetAnchorVer(0.0f, 0.0f);
		mBox2->SetAnchorParamHor(boxXStart + boxSpace*2.0f, 0.0f);
		mBox2->SetAnchorParamVer(boxZPos, 0.0f);
		mBox2->LocalTransform.SetTranslateY(-3.0f);

		AttachChild(mBox2);
	}

	if (CurveGroup::CGT_FLOAT3RANGE == cgType)
	{
		mBox3 = new0 UIFPicBox();
		mBox3->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/smallpic.png");
		mBox3->SetColor(Float3::RED / 2.0f);
		mBox3->SetSize(sizeBox);
		mBox3->SetAnchorHor(0.0f, 0.0f);
		mBox3->SetAnchorVer(0.0f, 0.0f);
		mBox3->SetAnchorParamHor(boxXStart + boxSpace*3.0f, 0.0f);
		mBox3->SetAnchorParamVer(boxZPos, 0.0f);
		mBox3->LocalTransform.SetTranslateY(-3.0f);

		AttachChild(mBox3);
	}

	if (CurveGroup::CGT_FLOAT3RANGE == cgType)
	{
		mBox4 = new0 UIFPicBox();
		mBox4->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/smallpic.png");
		mBox4->SetColor(Float3::GREEN / 2.0f);
		mBox4->SetSize(sizeBox);
		mBox4->SetAnchorHor(0.0f, 0.0f);
		mBox4->SetAnchorVer(0.0f, 0.0f);
		mBox4->SetAnchorParamHor(boxXStart + boxSpace*4.0f, 0.0f);
		mBox4->SetAnchorParamVer(boxZPos, 0.0f);
		mBox4->LocalTransform.SetTranslateY(-3.0f);

		AttachChild(mBox4);
	}

	if (CurveGroup::CGT_FLOAT3RANGE == cgType)
	{
		mBox5 = new0 UIFPicBox();
		mBox5->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/smallpic.png");
		mBox5->SetColor(Float3::BLUE / 2.0f);
		mBox5->SetSize(sizeBox);
		mBox5->SetAnchorHor(0.0f, 0.0f);
		mBox5->SetAnchorVer(0.0f, 0.0f);
		mBox5->SetAnchorParamHor(boxXStart + boxSpace*5.0f, 0.0f);
		mBox5->SetAnchorParamVer(boxZPos, 0.0f);
		mBox5->LocalTransform.SetTranslateY(-3.0f);

		AttachChild(mBox5);
	}

	mBox = new0 UIFPicBox();
	mBox->GetUIPicBox()->SetTexture("DataNIRVANA2/images/icons/timeline/smallpic.png");
	mBox->SetColor(Float3::YELLOW);
	mBox->SetSize(sizeBox);
	mBox->SetAnchorHor(0.0f, 0.0f);
	mBox->SetAnchorVer(0.0f, 0.0f);
	mBox->SetAnchorParamHor(sizeBackground.Width - boxSpace - boxXStart, 0.0f);
	mBox->SetAnchorParamVer(boxZPos, 0.0f);
	mBox->LocalTransform.SetTranslateY(-3.0f);
	AttachChild(mBox);

	mFText = new0 UIFText();
	AttachChild(mFText);
	mFText->SetDoPick(false);
	mFText->GetText()->SetFontScale(0.65f);
	mFText->SetAnchorHor(0.0f, 1.0f);
	mFText->SetAnchorVer(0.0f, 1.0f);
	mFText->LocalTransform.SetTranslateY(-1.0f);

	if (mCurveGroup)
	{
		SetText(mCurveGroup->GetName());
	}

	SetWidget(true);
}
//----------------------------------------------------------------------------
UICurveGroup::~UICurveGroup ()
{
}
//----------------------------------------------------------------------------
void UICurveGroup::SetText (std::string text)
{
	mFText->GetText()->SetText(text);
}
//----------------------------------------------------------------------------
void UICurveGroup::SetBackColor (Float3 color)
{
	if (mFBackground)
	{
		mFBackground->SetColor(color);
	}
}
//----------------------------------------------------------------------------
void UICurveGroup::OnWidgetPicked(const UIInputData &inputData)
{
	const APoint &worldPos = inputData.WorldPos;
	Float2 wPos(worldPos.X(), worldPos.Z());

	UIFrame *parentFrame = DynamicCast<UIFrame>(GetParent());
	if (parentFrame)
	{
		const Rectf &parentRect = parentFrame->GetWorldRect();
		if (!parentRect.IsInsize(wPos))
		{
			return;
		}
	}

	if (UIInputData::MT_LEFT == inputData.TheMouseTag && 
		UIPT_PRESSED == inputData.PickType)
	{
		PX2_EDIT.GetTimeLineEdit()->SetSelectedUICurveGroup(this);
	}
}
//----------------------------------------------------------------------------
void UICurveGroup::OnWidgetNotPicked(const UIInputData &inputData)
{
	const APoint &worldPos = inputData.WorldPos;
	Float2 wPos(worldPos.X(), worldPos.Z());

	UIFrame *parentFrame = DynamicCast<UIFrame>(GetParent());
	if (parentFrame)
	{
		const Rectf &parentRect = parentFrame->GetWorldRect();
		if (!parentRect.IsInsize(wPos))
		{
			return;
		}
	}

	if (UIInputData::MT_LEFT == inputData.TheMouseTag)
	{
		UICurveGroup *group = PX2_EDIT.GetTimeLineEdit()
			->GetSelectedUICurveGroup();
		if (this == group)
		{
			PX2_EDIT.GetTimeLineEdit()->SetSelectedUICurveGroup(0);
		}
	}
}
//----------------------------------------------------------------------------
void UICurveGroup::OnUIPicked(const UIInputData &inputData)
{
	const APoint &worldPos = inputData.WorldPos;
	Float2 wPos(worldPos.X(), worldPos.Z());

	UIFrame *parentFrame = DynamicCast<UIFrame>(GetParent());
	if (parentFrame)
	{
		const Rectf &parentRect = parentFrame->GetWorldRect();
		if (!parentRect.IsInsize(wPos))
		{
			return;
		}
	}

	if (UIInputData::MT_LEFT == inputData.TheMouseTag &&
		UIPT_RELEASED == inputData.PickType)
	{
		Rectf rect0;
		if (mBox0)
			rect0 = mBox0->GetWorldRect();
		Rectf rect1;
		if (mBox1)
			rect1 = mBox1->GetWorldRect();
		Rectf rect2;
		if (mBox2)
			rect2 = mBox2->GetWorldRect();
		Rectf rect3;
		if (mBox3)
			rect3 = mBox3->GetWorldRect();
		Rectf rect4;
		if (mBox4)
			rect4 = mBox4->GetWorldRect();
		Rectf rect5;
		if (mBox5)
			rect5 = mBox5->GetWorldRect();
		Rectf rect;
		if (mBox)
			rect = mBox->GetWorldRect();

		if (!rect0.IsEmpty() && rect0.IsInsize(wPos))
		{
			mIsShowBox0 = !mIsShowBox0;

			if (mIsShowBox0)
			{
				mBox0->SetColor(Float3::RED);
				mIsShowBox = true;
				mBox->SetColor(Float3::YELLOW);
			}
			else
				mBox0->SetColor(Float3(0.5f, 0.5f, 0.5f));

			mCurveGroup->SetVisible(0, mIsShowBox0);
		}
		else if (!rect1.IsEmpty() && rect1.IsInsize(wPos))
		{
			mIsShowBox1 = !mIsShowBox1;

			if (mIsShowBox1)
			{
				mBox1->SetColor(Float3::GREEN);
				mIsShowBox = true;
				mBox->SetColor(Float3::YELLOW);
			}
			else
				mBox1->SetColor(Float3(0.5f, 0.5f, 0.5f));

			mCurveGroup->SetVisible(1, mIsShowBox1);
		}
		else if (!rect2.IsEmpty() && rect2.IsInsize(wPos))
		{
			mIsShowBox2 = !mIsShowBox2;

			if (mIsShowBox2)
			{
				mBox2->SetColor(Float3::BLUE);
				mIsShowBox = true;
				mBox->SetColor(Float3::YELLOW);
			}
			else
				mBox2->SetColor(Float3(0.5f, 0.5f, 0.5f));

			mCurveGroup->SetVisible(2, mIsShowBox2);
		}
		else if (!rect3.IsEmpty() && rect3.IsInsize(wPos))
		{
			mIsShowBox3 = !mIsShowBox3;

			if (mIsShowBox3)
			{
				mBox3->SetColor(Float3::RED / 2.0f);
				mIsShowBox = true;
				mBox->SetColor(Float3::YELLOW);
			}
			else
				mBox3->SetColor(Float3(0.5f, 0.5f, 0.5f));

			mCurveGroup->SetVisible(3, mIsShowBox3);
		}
		else if (!rect4.IsEmpty() && rect4.IsInsize(wPos))
		{
			mIsShowBox4 = !mIsShowBox4;

			if (mIsShowBox4)
			{
				mBox4->SetColor(Float3::GREEN / 2.0f);
				mIsShowBox = true;
				mBox->SetColor(Float3::YELLOW);
			}
			else
				mBox4->SetColor(Float3(0.5f, 0.5f, 0.5f));

			mCurveGroup->SetVisible(4, mIsShowBox4);
		}
		else if (!rect5.IsEmpty() && rect5.IsInsize(wPos))
		{
			mIsShowBox5 = !mIsShowBox5;

			if (mIsShowBox5)
			{
				mBox5->SetColor(Float3::BLUE / 2.0f);
				mIsShowBox = true;
				mBox->SetColor(Float3::YELLOW);
			}
			else
				mBox5->SetColor(Float3(0.5f, 0.5f, 0.5f));

			mCurveGroup->SetVisible(5, mIsShowBox5);
		}
		else if (!rect.IsEmpty() && rect.IsInsize(wPos))
		{
			mIsShowBox = !mIsShowBox;

			if (mIsShowBox)
			{
				if (mBox0)
					mBox0->SetColor(Float3::RED);
				if (mBox1)
					mBox1->SetColor(Float3::GREEN);
				if (mBox2)
					mBox2->SetColor(Float3::BLUE);
				if (mBox3)
					mBox3->SetColor(Float3::RED / 2.0f);
				if (mBox4)
					mBox4->SetColor(Float3::GREEN / 2.0f);
				if (mBox5)
					mBox5->SetColor(Float3::BLUE / 2.0f);

				mBox->SetColor(Float3::YELLOW);
			}
			else
			{
				if (mBox0)
					mBox0->SetColor(Float3(0.5f, 0.5f, 0.5f));
				if (mBox1)
					mBox1->SetColor(Float3(0.5f, 0.5f, 0.5f));
				if (mBox2)
					mBox2->SetColor(Float3(0.5f, 0.5f, 0.5f));
				if (mBox3)
					mBox3->SetColor(Float3(0.5f, 0.5f, 0.5f));
				if (mBox4)
					mBox4->SetColor(Float3(0.5f, 0.5f, 0.5f));
				if (mBox5)
					mBox5->SetColor(Float3(0.5f, 0.5f, 0.5f));

				mBox->SetColor(Float3(0.5f, 0.5f, 0.5f));
			}

			mCurveGroup->SetVisible(mIsShowBox);
		}
	}
}
//----------------------------------------------------------------------------
void UICurveGroup::OnUINotPicked(const UIInputData &inputData)
{
}
//----------------------------------------------------------------------------
void UICurveGroup::OnEvent (Event *event)
{
	PX2_UNUSED(event);
}
//----------------------------------------------------------------------------