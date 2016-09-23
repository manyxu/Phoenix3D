// PX2UI.cpp

#include "PX2UI.hpp"
#include "PX2Creater.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2GraphicsEventData.hpp"
#include "PX2Float2.hpp"
#include "PX2UICanvas.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UI);
PX2_IMPLEMENT_STREAM(UI);
PX2_IMPLEMENT_FACTORY(UI);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, UI);

//----------------------------------------------------------------------------
float UI::msUICameraY = -2000.0f;
//----------------------------------------------------------------------------
UI::UI()
{
	SetName("UI");

	ComeInEventWorld();

	mDefaultCamera = new0 Camera(false);
	mDefaultCamera->SetFrame(APoint(0.0f, msUICameraY, 0.0f), AVector::UNIT_Y,
		AVector::UNIT_Z, AVector::UNIT_X);
	mDefaultCamera->SetClearFlag(false, true, true);
	mDefaultCamera->SetClearColor(Float4::RED);

	mDefaultCameraNode = new0 CameraNode();
	mDefaultCameraNode->SetCamera(mDefaultCamera);
	AttachChild(mDefaultCameraNode);
	mDefaultCameraNode->SetName("DefaultCameraNode");

	SetAnchorHor(0.0f, 1.0f);
	SetAnchorVer(0.0f, 1.0f);
	EnableAnchorLayout(true);

	SetColorSelfCtrled(true);
	SetAlphaSelfCtrled(true);
	SetBrightnessSelfCtrled(true);
}
//----------------------------------------------------------------------------
UI::~UI()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
bool UI::AddCamera(Camera *camera)
{
	if (!camera)
		return false;

	for (int i = 0; i < (int)mCameras.size(); i++)
	{
		if (mCameras[i] == camera)
			return false;
	}

	mCameras.push_back(camera);

	return true;
}
//----------------------------------------------------------------------------
void UI::OnSizeChanged()
{
	_UpdateCamera();

	UIFrame::OnSizeChanged();
}
//----------------------------------------------------------------------------
void UI::UpdateLeftBottomCornerOffset(Movable *parent)
{
	UIFrame::UpdateLeftBottomCornerOffset(parent);

	_UpdateCamera();
}
//----------------------------------------------------------------------------
void UI::_UpdateCamera()
{
	float helfWidth = mSize.Width / 2.0f;
	float helfHeight = mSize.Height / 2.0f;
	mDefaultCamera->SetFrustum(0.1f, Mathf::FAbs(msUICameraY) + 1000.0f,
		-helfHeight, helfHeight, -helfWidth, helfWidth);

	mDefaultCameraNode->LocalTransform.SetTranslate(
		helfWidth + LeftBottomCornerOffset.X(), msUICameraY,
		helfHeight + LeftBottomCornerOffset.Z());
}
//----------------------------------------------------------------------------
void UI::OnEvent(Event *ent)
{
	if (GraphicsES::IsEqual(ent, GraphicsES::AddObject))
	{
		AddObjectData data = ent->GetData<AddObjectData>();
		CameraNode *camNode = DynamicCast<CameraNode>(data.Obj);
		if (camNode)
		{
			UI *ui = camNode->GetFirstParentDerivedFromType<UI>();
			if (ui)
			{
				ui->AddCamera(camNode->GetCamera());
			}
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void UI::RegistProperties()
{
	UIFrame::RegistProperties();

	AddPropertyClass("UI");
}
//----------------------------------------------------------------------------
void UI::OnPropertyChanged(const PropertyObject &obj)
{
	UIFrame::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UI::UI(LoadConstructor value) :
UIFrame(value)
{
	ComeInEventWorld();
}
//----------------------------------------------------------------------------
void UI::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mDefaultCamera);
	source.ReadPointer(mDefaultCameraNode);

	PX2_END_DEBUG_STREAM_LOAD(UI, source);
}
//----------------------------------------------------------------------------
void UI::Link(InStream& source)
{
	UIFrame::Link(source);

	source.ResolveLink(mDefaultCamera);
	source.ResolveLink(mDefaultCameraNode);
}
//----------------------------------------------------------------------------
void UI::PostLink()
{
	UIFrame::PostLink();

	if (mDefaultCamera)
		mDefaultCamera->PostLink();

	if (mDefaultCameraNode)
		mDefaultCameraNode->PostLink();
}
//----------------------------------------------------------------------------
bool UI::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		if (mDefaultCamera)
		{
			target.Register(mDefaultCamera);
		}

		if (mDefaultCameraNode)
		{
			target.Register(mDefaultCameraNode);
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UI::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointer(mDefaultCamera);
	target.WritePointer(mDefaultCameraNode);

	PX2_END_DEBUG_STREAM_SAVE(UI, target);
}
//----------------------------------------------------------------------------
int UI::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_POINTERSIZE(mDefaultCamera);
	size += PX2_POINTERSIZE(mDefaultCameraNode);

	return size;
}
//----------------------------------------------------------------------------