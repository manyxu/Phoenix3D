// PX2Project.cpp

#include "PX2Project.hpp"
#include "PX2XMLData.hpp"
#include "PX2StringHelp.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2StringTokenizer.hpp"
#include "PX2Renderer.hpp"
#include "PX2UICanvas.hpp"
#include "PX2UIPicBox.hpp"
#include "PX2EngineLoop.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2SimulationEventType.hpp"
#include "PX2ScriptManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, Project);

//----------------------------------------------------------------------------
Project::Project() :
mEdit_UICameraPercent(1.0f),
mScreenOrientation(SO_LANDSCAPE)
{
	PX2_ENGINELOOP.TheProject = this;

	mBackgroundColor = Float4(0.5f, 0.5f, 0.5f, 1.0f);
	mProjBackgroundColor = Float4::WHITE;
	
	mSceneCanvas = new0 SceneCanvas();
	mSceneCanvas->SetName("SceneCanvas");
	SetSceneCanvas(mSceneCanvas);
	mSceneCanvas->ComeInEventWorld();

	mUICanvas = new0 UICanvas();
	mUICanvas->SetBeforeDrawClear(false, true, true);
	mUICanvas->SetName("UICanvas");
	mUICanvas->EnableAnchorLayout(true);
	mUICanvas->SetAnchorHor(0.0f, 1.0f);
	mUICanvas->SetAnchorVer(0.0f, 1.0f);
	mUICanvas->ComeInEventWorld();
	mUICanvas->LocalTransform.SetTranslateY(-1.0f);

	mUIFrame = new0 UIFrame();
	SetUIFrame(mUIFrame);
	mUIFrame->SetName("RootUIFrame");
	mUIFrame->SetAnchorHor(0.0f, 1.0f);
	mUIFrame->SetAnchorVer(0.0f, 1.0f);

	//mBPPackage = new0 BPPackage();
	//SetBPPackage(mBPPackage);
	//mBPPackage->SetName("BPPackage");

	mIsShowShadowBloomEveryPass = false;

	ComeInEventWorld();
}
//----------------------------------------------------------------------------
Project::~Project()
{
	GoOutEventWorld();

	PX2_GR.GetMainWindow()->RemoveCanvas(mSceneCanvas);
	mSceneCanvas = 0;

	PX2_GR.GetMainWindow()->RemoveCanvas(mUICanvas);
	mUICanvas = 0;
}
//----------------------------------------------------------------------------
void Project::Destory()
{
	if (PX2_ENGINELOOP.TheProject)
	{
		PX2_SC_LUA->SetUserTypePointer("PX2_PROJ", "Project", 0);
		PX2_ENGINELOOP.TheProject = 0;
		Project::Set(0);
	}
}
//----------------------------------------------------------------------------
void Project::SetScreenOrientation(ScreenOrientation so)
{
	mScreenOrientation = so;
}
//----------------------------------------------------------------------------
Project::ScreenOrientation Project::_FromSOStr(const std::string &str)
{
	if ("landscape" == str) return SO_LANDSCAPE;
	else if ("portrait" == str) return SO_PORTRAIT;

	return SO_PORTRAIT;
}
//----------------------------------------------------------------------------
std::string Project::_ToSOStr(Project::ScreenOrientation so)
{
	if (SO_LANDSCAPE == so) return "landscape";
	else if (SO_PORTRAIT == so) return "portrait";

	return "portrait";
}
//----------------------------------------------------------------------------
bool Project::Save(const std::string &filename)
{
	if (!SaveConfig(filename))
		return false;

	std::string outPath;
	std::string outBaseName;
	std::string outExt;
	StringHelp::SplitFullFilename(filename, outPath, outBaseName, outExt);

	if (mUIFrame)
	{
		std::string outName = outPath + outBaseName + "_ui.px2obj";

		OutStream output;
		output.Insert(mUIFrame);
		output.Save(outName);
	}

	if (mBPPackage)
	{
		std::string outName = outPath + outBaseName + "_bp.px2obj";

		OutStream output;
		output.Insert(mBPPackage);
		output.Save(outName);
	}

	return false;
}
//----------------------------------------------------------------------------
bool Project::SaveConfig(const std::string &filename)
{
	// xml
	XMLData data;
	data.Create();

	XMLNode projNode = data.NewChild("project");

	// general
	XMLNode generalNode = projNode.NewChild("general");
	generalNode.SetAttributeString("name", GetName().c_str());
	generalNode.SetAttributeString("screenorientation", _ToSOStr(mScreenOrientation));
	generalNode.SetAttributeInt("width", (int)mSize.Width);
	generalNode.SetAttributeInt("height", (int)mSize.Height);
	std::string colorStr =
		StringHelp::IntToString((int)(mBackgroundColor[0] * 255.0f)) + "," +
		StringHelp::IntToString((int)(mBackgroundColor[1] * 255.0f)) + "," +
		StringHelp::IntToString((int)(mBackgroundColor[2] * 255.0f)) + "," +
		StringHelp::IntToString((int)(mBackgroundColor[3] * 255.0f));
	generalNode.SetAttributeString("backcolor", colorStr);
	std::string projColorStr =
		StringHelp::IntToString((int)(mProjBackgroundColor[0] * 255.0f)) + "," +
		StringHelp::IntToString((int)(mProjBackgroundColor[1] * 255.0f)) + "," +
		StringHelp::IntToString((int)(mProjBackgroundColor[2] * 255.0f)) + "," +
		StringHelp::IntToString((int)(mProjBackgroundColor[3] * 255.0f));
	generalNode.SetAttributeString("projcolor", projColorStr);

	// scene
	XMLNode sceneNode = projNode.NewChild("scene");
	sceneNode.SetAttributeString("filename", mSceneFilename.c_str());

	// render setting
	XMLNode renderNode = projNode.NewChild("render_setting");

	// language
	XMLNode languageNode = projNode.NewChild("language");

	// publish
	XMLNode publish = projNode.NewChild("publish");

	// setting
	XMLNode settingNode = projNode.NewChild("edit_setting");
	settingNode.SetAttributeFloat("uicamerapercent", mEdit_UICameraPercent);

	if (data.SaveFile(filename))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool Project::Load(const std::string &filename)
{
	std::string name;
	int height = 0;
	std::string sceneFilename;
	int width = 0;
	std::string uiFilename;
	std::string languageFilename;

	char *buffer = 0;
	int bufferSize = 0;
	if (PX2_RM.LoadBuffer(filename, bufferSize, buffer))
	{
		XMLData data;
		if (data.LoadBuffer(buffer, bufferSize))
		{
			XMLNode rootNode = data.GetRootNode();

			// general
			XMLNode generalNode = rootNode.GetChild("general");
			if (!generalNode.IsNull())
			{
				name = generalNode.AttributeToString("name");

				SetScreenOrientation(_FromSOStr(generalNode.AttributeToString("screenorientation")));

				width = generalNode.AttributeToInt("width");
				height = generalNode.AttributeToInt("height");

				std::string colorStr = generalNode.AttributeToString("backcolor");
				StringTokenizer stk(colorStr, ",");
				Float4 color = Float4::MakeColor(
					StringHelp::StringToInt(stk[0]),
					StringHelp::StringToInt(stk[1]),
					StringHelp::StringToInt(stk[2]),
					StringHelp::StringToInt(stk[3]));

				std::string projcolorStr = generalNode.AttributeToString("projcolor");
				StringTokenizer stkprojcolor(projcolorStr, ",");
				Float4 projcolor = Float4::MakeColor(
					StringHelp::StringToInt(stkprojcolor[0]),
					StringHelp::StringToInt(stkprojcolor[1]),
					StringHelp::StringToInt(stkprojcolor[2]),
					StringHelp::StringToInt(stkprojcolor[3]));

				Sizef size = Sizef((float)width, (float)height);
				SetName(name);
				SetSize(size);
				mViewRect = Rectf(0.0f, 0.0f, size.Width, size.Height);
				SetBackgroundColor(color);
				SetProjBackgroundColor(projcolor);
			}

			// scene
			XMLNode sceneNode = rootNode.GetChild("scene");
			if (!sceneNode.IsNull())
			{
				sceneFilename = sceneNode.AttributeToString("filename");
				SetSceneFilename(sceneFilename);
			}

			XMLNode renderNode = rootNode.GetChild("render_setting");
			if (!renderNode.IsNull())
			{
			}

			// language
			XMLNode languageNode = rootNode.GetChild("language");

			// publish
			XMLNode publishNode = rootNode.GetChild("publish");

			// setting
			XMLNode settingNode = rootNode.GetChild("setting");
			if (!settingNode.IsNull())
			{
				if (settingNode.HasAttribute("uicamerapercent"))
					mEdit_UICameraPercent = settingNode.AttributeToFloat("uicamerapercent");
			}

			// split file names
			std::string outPath;
			std::string outBaseName;
			std::string outExt;
			StringHelp::SplitFullFilename(filename, outPath, outBaseName, outExt);

			// ui
			mUIFilename = outPath + outBaseName + "_ui.px2obj";

			// bp
			mBPFilename = outPath + outBaseName + "_bp.px2obj";
		}
	}
	else
	{
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------
void Project::SetScene(Scene *scene)
{
	if (mScene)
	{
		mScene->GoOutEventWorld();
		mSceneCanvas->DetachChild(mScene);
		mScene = 0;
		mSceneCanvas->SetCamera(0);
		PX2_GR.SetCurEnvirParam(0);

		Event *ent = SimuES::CreateEventX(SimuES::CloseScene);
		EventWorld::GetSingleton().BroadcastingLocalEvent(ent);
	}

	mScene = scene;

	if (mScene)
	{
		mSceneCanvas->AttachChild(mScene);
		mScene->ComeInEventWorld();

		PX2_GR.SetCurEnvirParam(mScene->GetEnvirParam());

		CameraActor *camActor = mScene->GetUseCameraActor();

		if (camActor)
		{
			Camera *camera = camActor->GetCamera();
			Renderer::GetDefaultRenderer()->SetCamera(camera);
			mSceneCanvas->SetCamera(camera);
		}
		else
		{
			mSceneCanvas->SetCamera(0);
		}

		Event *ent = SimuES::CreateEventX(SimuES::NewScene);
		PX2_EW.BroadcastingLocalEvent(ent);
	}
	else
	{
		mSceneCanvas->SetCamera(0);
	}
}
//----------------------------------------------------------------------------
void Project::SetSceneFilename(const std::string &scenefilename)
{
	mSceneFilename = scenefilename;
}
//----------------------------------------------------------------------------
void Project::SetSceneCanvas(SceneCanvas *sceneCanvas)
{
	mSceneCanvas = sceneCanvas;
}
//----------------------------------------------------------------------------
void Project::SetUIFrame(UIFrame *ui)
{
	if (mUIFrame)
	{
		mUICanvas->DetachChild(mUIFrame);

		Event *ent = SimuES::CreateEventX(SimuES::CloseUI);
		EventWorld::GetSingleton().BroadcastingLocalEvent(ent);
	}

	mUIFrame = ui;

	if (mUIFrame)
	{
		mUICanvas->AttachChild(mUIFrame);

		Event *ent = SimuES::CreateEventX(SimuES::NewUI);
		EventWorld::GetSingleton().BroadcastingLocalEvent(ent);
	}
}
//----------------------------------------------------------------------------
UIFrame *Project::GetUIFrame()
{
	return (UIFrame*)mUIFrame;
}
//----------------------------------------------------------------------------
UICanvas *Project::GetUICanvas()
{
	return mUICanvas;
}
//----------------------------------------------------------------------------
void Project::SetBPPackage(BPPackage *package)
{
	mBPPackage = package;
}
//----------------------------------------------------------------------------
void Project::SetSize(float width, float height)
{
	SetSize(Sizef(width, height));
}
//----------------------------------------------------------------------------
void Project::SetSize(const Sizef &size)
{
	mSize = size;

	if (mSceneCanvas)
	{
		mUICanvas->SetSize(mSize);
	}

	if (mUICanvas)
	{
		mUICanvas->SetSize(mSize);
		mUICanvas->LocalTransform.SetTranslateXZ(mSize.Width / 2.0f,
			mSize.Height / 2.0f);
	}
}
//----------------------------------------------------------------------------
void Project::SetBackgroundColor(const Float4 &color)
{
	mBackgroundColor = color;
}
//----------------------------------------------------------------------------
const Float4 &Project::GetBackgroundColor() const
{
	return mBackgroundColor;
}
//----------------------------------------------------------------------------
void Project::SetProjBackgroundColor(const Float4 &color)
{
	mProjBackgroundColor = color;
}
//----------------------------------------------------------------------------
const Float4 &Project::GetProjBackgroundColor() const
{
	return mProjBackgroundColor;
}
//----------------------------------------------------------------------------
void Project::SetViewRect(const Rectf &viewRect)
{
	if (mUICanvas)
	{
		mUICanvas->SetViewPort(viewRect);
	}
}
//----------------------------------------------------------------------------
void Project::DoExecute(Event *event)
{
	if (SimuES::IsEqual(event, SimuES::GeneralString))
	{
		std::string eventStr = event->GetData<std::string>();

		std::string funStr = std::string("OnEventGeneralString('")
			+ eventStr+ "')";

		PX2_SC_LUA->CallString(funStr);
	}
}
//----------------------------------------------------------------------------
void Project::SetShowShadowBloomEveryPass(
	bool isShowShadowBloomEveryPass)
{
	mIsShowShadowBloomEveryPass = isShowShadowBloomEveryPass;

	if (mSceneCanvas)
	{
		mSceneCanvas->SetShowShadowBloomEveryPass(
			isShowShadowBloomEveryPass);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void Project::RegistProperties()
{
	Object::RegistProperties();

	AddPropertyClass("Project");

	std::vector<std::string> screenOrientations;
	screenOrientations.push_back("SO_LANDSCAPE");
	screenOrientations.push_back("SO_PORTRAIT");

	AddPropertyEnum("ScreenOrientation", (int)mScreenOrientation,
		screenOrientations);

	AddProperty("Size", PT_SIZE, mSize);

	Float3 color(mBackgroundColor[0], mBackgroundColor[1], mBackgroundColor[1]);
	AddProperty("BackgroundColor", PT_COLOR3FLOAT3, color);

	Float3 colorProj(mProjBackgroundColor[0], mProjBackgroundColor[1],
		mProjBackgroundColor[2]);
	AddProperty("ProjBackgroundColor", PT_COLOR3FLOAT3, colorProj);

	AddProperty("ViewRect", PT_RECT, mViewRect, false);

	AddProperty("IsShowShadowBloomEveryPass", PT_BOOL,
		mIsShowShadowBloomEveryPass);
}
//----------------------------------------------------------------------------
void Project::OnPropertyChanged(const PropertyObject &obj)
{
	Object::OnPropertyChanged(obj);

	if ("ScreenOrientation" == obj.Name)
	{
		SetScreenOrientation((ScreenOrientation)PX2_ANY_AS(obj.Data, int));
	}
	else if ("Size" == obj.Name)
	{
		SetSize(PX2_ANY_AS(obj.Data, Sizef));
	}
	else if ("BackgroundColor" == obj.Name)
	{
		Float3 backColor = PX2_ANY_AS(obj.Data, Float3);
		mBackgroundColor = Float4(backColor[0], backColor[1], backColor[2],
			1.0f);
	}
	else if ("ProjBackgroundColor" == obj.Name)
	{
		Float3 progBackColor = PX2_ANY_AS(obj.Data, Float3);
		mProjBackgroundColor = Float4(progBackColor[0], progBackColor[1],
			progBackColor[2], 1.0f);
	}
	else if ("IsShowShadowBloomEveryPass" == obj.Name)
	{
		SetShowShadowBloomEveryPass(PX2_ANY_AS(obj.Data, bool));
	}
}
//----------------------------------------------------------------------------