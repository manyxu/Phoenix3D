// PX2EU_CanvasTimeLineEdit.cpp

#include "PX2EU_CanvasTimeLineEdit.hpp"
#include "PX2Edit.hpp"
#include "PX2EditTimeLineEdit.hpp"
#include "PX2Renderer.hpp"
#include "PX2Picker.hpp"
#include "PX2Canvas.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_CanvasTimeLineEdit);
PX2_IMPLEMENT_STREAM(EU_CanvasTimeLineEdit);
PX2_IMPLEMENT_FACTORY(EU_CanvasTimeLineEdit);

//----------------------------------------------------------------------------
EU_CanvasTimeLineEdit::EU_CanvasTimeLineEdit():
mMoveMode(MM_PAN),
mIsInitUpdated(false),
mLeftWidth(120.0f),
mPixelOverCamIn(1.0f),
mPixelOverCamOut(1.0f),
mUPerGrid(0.0f),
mVPerGrid(0.0f),
mXStart(0.0f),
mXEnd(0.0f),
mZStart(0.0f),
mZEnd(0.0f)
{
	//mUICanvasGrid = new0 UICanvas();
	//AttachChild(mUICanvasGrid);
	//mUICanvasGrid->SetBeforeDrawClear(true, true, true);
	//mUICanvasGrid->SetClearColor(Float4::WHITE);
	//mUICanvasGrid->SetAnchorHor(0.0f, 1.0f);
	//mUICanvasGrid->SetAnchorParamHor(mLeftWidth, 0.0f);
	//mUICanvasGrid->SetAnchorVer(0.0f, 1.0f);

	Node *gridRoot = PX2_EDIT.GetTimeLineEdit()
		->GetCurEditNode_GridRoot();
	//mUICanvasGrid->AttachChild(gridRoot);

	float rWidth = 2.0f*mSize.Width;
	float scaleX = rWidth / 22.5f;
	float scaleZ = scaleX * (1.0f);
	PX2_EDIT.GetTimeLineEdit()->SetCtrlsScale(Float2(scaleX, scaleZ));

	UIFrame *uiFrame = PX2_EDIT.GetTimeLineEdit()
		->GetCurveEditNode_UIGroup();
	AttachChild(uiFrame);
	uiFrame->CreateAddBackgroundPicBox();
	uiFrame->SetAnchorHor(0.0f, 0.0f);
	uiFrame->SetAnchorParamHor(mLeftWidth / 2.0f, 0.0f);
	uiFrame->SetAnchorVer(0.0f, 1.0f);
	uiFrame->SetSize(mLeftWidth, 0.0f);
}
//----------------------------------------------------------------------------
EU_CanvasTimeLineEdit::~EU_CanvasTimeLineEdit()
{
}
//----------------------------------------------------------------------------
void EU_CanvasTimeLineEdit::FitViewHorizontally()
{

}
//----------------------------------------------------------------------------
void EU_CanvasTimeLineEdit::FitViewVertically()
{

}
//----------------------------------------------------------------------------
void EU_CanvasTimeLineEdit::FitViewToAll()
{

}
//----------------------------------------------------------------------------
void EU_CanvasTimeLineEdit::FitViewToSelected()
{

}
//----------------------------------------------------------------------------
void EU_CanvasTimeLineEdit::ZoomCamera(float xDetal, float zDetal)
{
	//Camera *camera = mUICanvasGrid->GetCamera();

	//float dMin = 0.0f;
	//float dMax = 0.0f;
	//float uMin = 0.0f;
	//float uMax = 0.0f;
	//float rMin = 0.0f;
	//float rMax = 0.0f;
	//camera->GetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);
	//float uMaxTemp = uMax;
	//float rMaxTemp = rMax;

	//if (xDetal > 0.0f)
	//{
	//	rMaxTemp *= xDetal;
	//}
	//else
	//{
	//	rMaxTemp /= -xDetal;
	//}

	//if (zDetal > 0.0f)
	//{
	//	uMaxTemp *= zDetal;
	//}
	//else
	//{
	//	uMaxTemp /= -zDetal;
	//}

	//if (uMaxTemp > 0.0f)
	//{
	//	uMax = uMaxTemp;
	//}
	//if (rMaxTemp > 0.0f)
	//{
	//	rMax = rMaxTemp;
	//}

	//uMin = -uMax;
	//rMin = -rMax;
	//camera->SetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);

	//_RefreshGrid(true);
}
//----------------------------------------------------------------------------
void EU_CanvasTimeLineEdit::ZoomCameraTo(float xMax, float zMax)
{
	if (xMax <= 0.0f || zMax <= 0.0f) return;

	//Camera *camera = mUICanvasGrid->GetCamera();

	//float dMin = 0.0f;
	//float dMax = 0.0f;
	//float uMin = 0.0f;
	//float uMax = 0.0f;
	//float rMin = 0.0f;
	//float rMax = 0.0f;
	//camera->GetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);

	//rMax = xMax;
	//rMin = -rMax;
	//uMax = zMax;
	//uMin = -uMax;
	//camera->SetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);

	//_RefreshGrid(true);
}
//----------------------------------------------------------------------------
float _GetTimeLineGridSpacing(int gridNum)
{
	if (gridNum & 0x01) // Odd numbers
	{
		return Mathf::Pow(10.f, 0.5f*((float)(gridNum - 1)) + 1.f);
	}
	else // Even numbers
	{
		return 0.5f * Mathf::Pow(10.f, 0.5f*((float)(gridNum)) + 1.f);
	}
}
//----------------------------------------------------------------------------
void EU_CanvasTimeLineEdit::UpdateWorldData(double applicationTime, 
	double elapsedTime)
{
	UIFrame::UpdateWorldData(applicationTime, elapsedTime);

	if (!mIsInitUpdated)
	{
		_RefreshGrid(true);

		mIsInitUpdated = true;
	}
}
//----------------------------------------------------------------------------
void EU_CanvasTimeLineEdit::_RefreshGrid(bool doScale)
{
	if (!mUICanvasGrid) return;

	//Polysegment *gird = PX2_EDIT.GetTimeLineEdit()->GetGridPoly();

	//int width = (int)mSize.Width;
	//int height = (int)mSize.Height;

	//if (width <= 0 && height <= 0) return;

	//Camera *camera = mUICanvasGrid->GetCamera();
	//APoint pos = camera->GetPosition();
	//float dMin = 0.0f;
	//float dMax = 0.0f;
	//float uMin = 0.0f;
	//float uMax = 0.0f;
	//float rMin = 0.0f;
	//float rMax = 0.0f;
	//camera->GetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);
	//if (0.0f == rMax || 0.0f == uMax || 0.0f == dMax) return;

	//float inLength = rMax - rMin;
	//float outLength = uMax - uMin;
	//inLength = Mathf::FAbs(inLength);
	//outLength = Mathf::FAbs(outLength);

	//mPixelOverCamIn = width / inLength;
	//mPixelOverCamOut = height / outLength;

	//int minPixelsPerInGrid = 35;
	//int minPixelsPerOutGrid = 25;
	//float minGridSpacing = 0.001f;

	//int gridNum = 0;
	//float inGridSpacing = minGridSpacing;
	//while (inGridSpacing*mPixelOverCamIn < minPixelsPerInGrid)
	//{
	//	inGridSpacing = minGridSpacing * _GetTimeLineGridSpacing(gridNum);
	//	gridNum++;
	//}
	//mUPerGrid = inGridSpacing;

	//gridNum = 0;
	//float outGridSpacing = minGridSpacing;
	//while (outGridSpacing*mPixelOverCamOut < minPixelsPerOutGrid)
	//{
	//	outGridSpacing = minGridSpacing * _GetTimeLineGridSpacing(gridNum);
	//	gridNum++;
	//}
	//mVPerGrid = outGridSpacing;

	//mXStart = pos.X() + rMin;
	//mXEnd = pos.X() + rMax;
	//mZStart = pos.Z() + uMin;
	//mZEnd = pos.Z() + uMax;

	//Float3 gray(0.62f, 0.62f, 0.62f);
	//int segNum = 0;
	//mFontStrs.clear();
	//int iTemp = 4;
	//int zTemp = 4;

	//VertexBufferAccessor vba(gird);
	//float zPosTemp = 0.0f;
	//while (zPosTemp < mZEnd)
	//{
	//	zPosTemp += mVPerGrid;

	//	vba.Position<Float3>(2 * segNum) = Float3(mXStart, 1.0f, zPosTemp);
	//	vba.Color<Float3>(0, 2 * segNum) = gray;
	//	vba.Position<Float3>(2 * segNum + 1) = Float3(mXEnd, 1.0f, zPosTemp);
	//	vba.Color<Float3>(0, 2 * segNum + 1) = gray;

	//	Vector2f scrv = mUICanvasGrid->WorldPos3DToViewPort(APoint(0.0f, 0.0f, zPosTemp));
	//	FontStr fs;
	//	fs.x = iTemp + (int)mLeftWidth;
	//	fs.y = height - (int)scrv.Y();
	//	fs.str = StringHelp::FloatToString(zPosTemp);
	//	mFontStrs.push_back(fs);

	//	segNum++;
	//}
	//zPosTemp = 0.0f;
	//while (zPosTemp > mZStart)
	//{
	//	zPosTemp -= mVPerGrid;

	//	vba.Position<Float3>(2 * segNum) = Float3(mXStart, 1.0f, zPosTemp);
	//	vba.Color<Float3>(0, 2 * segNum) = gray;
	//	vba.Position<Float3>(2 * segNum + 1) = Float3(mXEnd, 1.0f, zPosTemp);
	//	vba.Color<Float3>(0, 2 * segNum + 1) = gray;

	//	Vector2f scrv = mUICanvasGrid->WorldPos3DToViewPort(APoint(0.0f, 0.0f,
	//		zPosTemp));
	//	FontStr fs;
	//	fs.x = iTemp + (int)mLeftWidth;
	//	fs.y = height - (int)scrv.Y();
	//	fs.str = StringHelp::FloatToString(zPosTemp);
	//	mFontStrs.push_back(fs);

	//	segNum++;
	//}
	//float xPosTemp = 0.0f;
	//while (xPosTemp < mXEnd)
	//{
	//	xPosTemp += mUPerGrid;

	//	vba.Position<Float3>(2 * segNum) = Float3(xPosTemp, 1.0f, mZStart);
	//	vba.Color<Float3>(0, 2 * segNum) = gray;
	//	vba.Position<Float3>(2 * segNum + 1) = Float3(xPosTemp, 1.0f, mZEnd);
	//	vba.Color<Float3>(0, 2 * segNum + 1) = gray;

	//	Vector2f scrv = mUICanvasGrid->WorldPos3DToViewPort(APoint(xPosTemp, 0.0f, 0.0f));
	//	FontStr fs;
	//	fs.x = (int)scrv.X() + iTemp;
	//	fs.y = height - zTemp;
	//	fs.str = StringHelp::FloatToString(xPosTemp);
	//	mFontStrs.push_back(fs);

	//	segNum++;
	//}
	//xPosTemp = 0.0f;
	//while (xPosTemp > mXStart)
	//{
	//	xPosTemp -= mUPerGrid;

	//	vba.Position<Float3>(2 * segNum) = Float3(xPosTemp, 1.0f, mZStart);
	//	vba.Color<Float3>(0, 2 * segNum) = gray;
	//	vba.Position<Float3>(2 * segNum + 1) = Float3(xPosTemp, 1.0f, mZEnd);
	//	vba.Color<Float3>(0, 2 * segNum + 1) = gray;

	//	Vector2f scrv = mUICanvasGrid->WorldPos3DToViewPort(APoint(xPosTemp, 0.0f,
	//		0.0f));
	//	FontStr fs;
	//	fs.x = (int)scrv.X() + iTemp;
	//	fs.y = height - zTemp;
	//	fs.str = StringHelp::FloatToString(xPosTemp);
	//	mFontStrs.push_back(fs);

	//	segNum++;
	//}

	//Float3 zeroLineColorRed = Float3::MakeColor(150, 28, 36);
	//Float3 zeroLineColorBlue = Float3::MakeColor(63, 72, 175);

	//vba.Position<Float3>(2 * segNum) = Float3(mXStart, 1.0f, 0.0f);
	//vba.Color<Float3>(0, 2 * segNum) = zeroLineColorRed;
	//vba.Position<Float3>(2 * segNum + 1) = Float3(mXEnd, 1.0f, 0.0f);
	//vba.Color<Float3>(0, 2 * segNum + 1) = zeroLineColorRed;
	//segNum++;
	//vba.Position<Float3>(2 * segNum) = Float3(0.0f, 1.0f, mZStart);
	//vba.Color<Float3>(0, 2 * segNum) = zeroLineColorBlue;
	//vba.Position<Float3>(2 * segNum + 1) = Float3(0.0f, 1.0f, mZEnd);
	//vba.Color<Float3>(0, 2 * segNum + 1) = zeroLineColorBlue;
	//segNum++;

	//Vector2f scrv = mUICanvasGrid->WorldPos3DToViewPort(APoint::ORIGIN);
	//FontStr fs;
	//fs.x = iTemp + (int)mLeftWidth;
	//fs.y = height - (int)scrv.Y();
	//fs.str = "0.0";
	//mFontStrs.push_back(fs);

	//FontStr fs1;
	//fs1.x = (int)scrv.X() + iTemp;
	//fs1.y = height - zTemp;
	//fs1.str = "0.0";
	//mFontStrs.push_back(fs1);

	//gird->SetNumSegments(segNum);

	//gird->UpdateModelSpace(Renderable::GU_MODEL_BOUND_ONLY);

	//Renderer::UpdateAll(gird->GetVertexBuffer());

	//if (doScale)
	//{
	//	float rWidth = 2.0f * rMax;
	//	float scaleX = rWidth / 22.5f;

	//	float uHeight = 2.0f * uMax;
	//	float scaleZ = uHeight / (((float)mSize.Height / (float)mSize.Width)*22.5f);
	//	PX2_EDIT.GetTimeLineEdit()->SetCtrlsScale(Float2(scaleX, scaleZ));
	//}
}
//----------------------------------------------------------------------------
void EU_CanvasTimeLineEdit::_TrySelectCurveCtrlPoint(const APoint &worldPos)
{
	if (!mUICanvasGrid) return;

	//APoint origin;
	//AVector direction;
	//mUICanvasGrid->GetPickRay(worldPos.X(), worldPos.Z(), origin, direction);

	//Node *gridNode = PX2_EDIT.GetTimeLineEdit()->GetCurveEditNode_Grid();

	//Picker picker;
	//picker.Execute(gridNode, origin, direction, 0.0f, Mathf::MAX_REAL);

	//if ((int)(picker.Records.size()) > 0)
	//{
	//	Movable *mov = picker.GetClosestNonnegative().Intersected;

	//	CurveCtrl *ctrl = PX2_EDIT.GetTimeLineEdit()->TrySelectCurve(mov);

	//	if (ctrl)
	//	{
	//		//UnToggleAllInterps();
	//		//EnableInterps(true);
	//		//ToggleInterp(ctrl->GetInterpCurveMode());

	//		PX2_EDIT.GetTimeLineEdit()->SetSelectedCurveCtrl(ctrl);
	//	}
	//	else
	//	{
	//		PX2_EDIT.GetTimeLineEdit()->SetSelectedCurveCtrl(0);
	//	}
	//}
	//else
	//{
	//	PX2_EDIT.GetTimeLineEdit()->SetSelectedCurveCtrl(0);
	//}
}
//----------------------------------------------------------------------------
void EU_CanvasTimeLineEdit::OnSizeChanged()
{
	FitViewToAll();
}
//----------------------------------------------------------------------------
void EU_CanvasTimeLineEdit::OnUIPicked(const UIInputData &inputData)
{
	if (UIPT_PRESSED == inputData.PickType)
	{
		_TrySelectCurveCtrlPoint(inputData.WorldPos);
	}
	else if (UIPT_WHELLED == inputData.PickType)
	{
		float val = 1.2f * Mathf::Sign(inputData.Wheel);
		ZoomCamera(val, val);
	}
	else if (UIPT_MOVED == inputData.PickType)
	{
		AVector diff = inputData.MoveDelta;
		if (AVector::ZERO == diff) return;

		float movedX = -diff.X() / mPixelOverCamIn;
		float movedZ = -diff.Z() / mPixelOverCamOut;
		
		bool isCtrlDown = PX2_EDIT.IsCtrlDown;
		CurveCtrl *selectedCtrl = PX2_EDIT.GetTimeLineEdit()
			->GetSelectedCurveCtrl();

		if (isCtrlDown && selectedCtrl)
		{

		}

		if (MM_PAN == mMoveMode)
		{
			if (mUICanvasGrid)
			{
				APoint pos = mUICanvasGrid->GetCameraNode()->LocalTransform.GetTranslate();
				pos += AVector(movedX, 0.0f, movedZ);
				mUICanvasGrid->GetCameraNode()->LocalTransform.SetTranslate(pos);
				mUICanvasGrid->GetCameraNode()->Update(Time::GetTimeInSeconds(), 0.0f);

				_RefreshGrid(false);
			}
		}
		else if (MM_ZOOM == mMoveMode)
		{
		}
	}
}
//----------------------------------------------------------------------------
void EU_CanvasTimeLineEdit::OnUINotPicked(const UIInputData &inputData)
{

}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_CanvasTimeLineEdit::EU_CanvasTimeLineEdit(LoadConstructor value) :
UIFrame(value),
mIsInitUpdated(false),
mLeftWidth(200.0f),
mPixelOverCamIn(1.0f),
mPixelOverCamOut(1.0f),
mUPerGrid(0.0f),
mVPerGrid(0.0f),
mXStart(0.0f),
mXEnd(0.0f),
mZStart(0.0f),
mZEnd(0.0f)
{
}
//----------------------------------------------------------------------------
void EU_CanvasTimeLineEdit::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_CanvasTimeLineEdit, source);
}
//----------------------------------------------------------------------------
void EU_CanvasTimeLineEdit::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_CanvasTimeLineEdit::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_CanvasTimeLineEdit::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_CanvasTimeLineEdit::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_CanvasTimeLineEdit, target);
}
//----------------------------------------------------------------------------
int EU_CanvasTimeLineEdit::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------