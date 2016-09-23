// PX2Creater.cpp

#include "PX2Creater.hpp"
#include "PX2Texture2D.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2StandardMesh.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2ModelController.hpp"
#include "PX2EffectModule.hpp"
#include "PX2EffectableController.hpp"
#include "PX2RedoUndo.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2StringHelp.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2RawTerrain.hpp"
#include "PX2LogicManager.hpp"
#include "PX2LuaScriptController.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Creater::Creater()
{
}
//----------------------------------------------------------------------------
Creater::~Creater()
{
}
//----------------------------------------------------------------------------
Movable *Creater::CreateMovable_Box()
{
	Texture2D *tex = DynamicCast<Texture2D>(PX2_RM.BlockLoad(
		"Data/engine/default.png"));
	if (!tex) return 0;

	VertexFormat *vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PNT1);

	StandardMesh stdMesh(vf);
	Node *meshNode = stdMesh.BoxPieces(2, 1);
	meshNode->SetName("NodeObject");

	for (int i = 0; i < meshNode->GetNumChildren(); i++)
	{
		TriMesh *mesh = DynamicCast<TriMesh>(meshNode->GetChild(i));

		if (mesh)
		{
			MaterialInstance *mi = new0 MaterialInstance(
				"Data/engine_mtls/std/std.px2obj", "std_light", false);
			mesh->SetMaterialInstance(mi);

			mi->SetPixelTexture(0, "SampleBase", tex);
		}
	}

	meshNode->SetReceiveShadow(false);

	return meshNode;
}
//----------------------------------------------------------------------------
Movable *Creater::CreateMovable_Sphere()
{
	Texture2D *tex = DynamicCast<Texture2D>(PX2_RM.BlockLoad(
		"Data/engine/default.png"));
	if (!tex) return 0;

	VertexFormat *vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PNT1);

	StandardMesh stdMesh(vf);
	TriMesh *mesh = stdMesh.Sphere(32, 32, 1);

	MaterialInstance *mi = new0 MaterialInstance(
		"Data/engine_mtls/std/std.px2obj", "std_light", false);
	mesh->SetMaterialInstance(mi);

	mi->SetPixelTexture(0, "SampleBase", tex);

	return mesh;
}
//----------------------------------------------------------------------------
Movable *Creater::CreateMovable_Rectangle()
{
	Texture2D *tex = DynamicCast<Texture2D>(PX2_RM.BlockLoad(
		"Data/engine/default.png"));
	if (!tex) return 0;

	VertexFormat *vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PNT1);

	StandardMesh stdMesh(vf);
	TriMesh *mesh = stdMesh.Rectangle(2, 2, 1.0f, 1.0f, Float2(0.5f, 0.5f));

	MaterialInstance *mi = new0 MaterialInstance(
		"Data/engine_mtls/std/std.px2obj", "std_light", false);
	mesh->SetMaterialInstance(mi);

	mi->SetPixelTexture(0, "SampleBase", tex);

	return mesh;
}
//----------------------------------------------------------------------------
Node *Creater::CreateNode()
{
	return new0 Node();
}
//----------------------------------------------------------------------------
CameraNode *Creater::CreateNode_Camera(bool createCamera)
{
	CameraNode *camNode = new0 CameraNode();
	camNode->EnableUpdate(true);

	if (createCamera)
	{
		Camera *camera = new0 Camera();
		camNode->SetCamera(camera);
	}

	return camNode;
}
//----------------------------------------------------------------------------
Node *Creater::CreateNode_Model(const std::string &filename)
{
	Node *node = new0 Node();
	node->SetName("NodeModel");
	
	ModelController *mc = new0 ModelController();
	mc->SetMovableFilename(filename);
	node->AttachController(mc);

	return node;
}
//----------------------------------------------------------------------------
Movable *Creater::CreateMovable_FromRes(const std::string &filename)
{
	std::string outPath;
	std::string outBaseName;
	std::string outExtention;
	StringHelp::SplitFullFilename(filename, outPath, outBaseName, 
		outExtention);

	if ("fbx" == outExtention || "FBX" == outExtention)
	{
		Object *obj = PX2_RM.BlockLoadShareCopy(filename, true, true, true);
		Movable *mov = DynamicCast<Movable>(obj);
		return mov;
	}
	else if ("px2obj" == outExtention)
	{
		Object *obj = PX2_RM.BlockLoadShareCopy(filename, true, true, true);
		Movable *mov = DynamicCast<Movable>(obj);
		return mov;
	}

	return 0;
}
//----------------------------------------------------------------------------
ScriptController *Creater::CreateScriptController_FromRes(
	const std::string &filename)
{
	std::string outPath;
	std::string outBaseName;
	std::string outExtention;
	StringHelp::SplitFullFilename(filename, outPath, outBaseName,
		outExtention);

	ScriptController *sc = 0;
	if ("lua" == outExtention || "LUA" == outExtention)
	{
		sc = new0 LuaScriptController();
		sc->SetFileClass(filename, outBaseName);
	}

	return sc;
}
//----------------------------------------------------------------------------
ParticleEmitter *Creater::CreateParticleEmitter()
{
	ParticleEmitter *pe = new0 ParticleEmitter();
	return pe;
}
//----------------------------------------------------------------------------
Billboard *Creater::CreateBillboard()
{
	Billboard *bd = new0 Billboard();
	return bd;
}
//----------------------------------------------------------------------------
BeamEmitter *Creater::CreateBeamEmitter()
{
	BeamEmitter *be = new0 BeamEmitter();
	return be;
}
//----------------------------------------------------------------------------
RibbonEmitter *Creater::CreateRibbonEmitter()
{
	RibbonEmitter *re = new0 RibbonEmitter();
	return re;
}
//----------------------------------------------------------------------------
Soundable *Creater::CreateSoundable()
{
	Soundable *soundable = new0 Soundable();
	return soundable;
}
//----------------------------------------------------------------------------
Movable *Creater::CreateSkyBox(const std::string &skyDirPath)
{
	if (skyDirPath.find(".") != std::string::npos)
		return 0;

	float skyBoxSize = 500.0f;

	Node *node = new0 Node();
	node->SetName("Sky");

	VertexBuffer* vbuffer;
	TriMesh* wall;
	std::string textureName;

	// Index buffer shared by the room walls.
	IndexBuffer* ibuffer = new0 IndexBuffer(6, sizeof(int));
	int* indices = (int*)ibuffer->GetData();
	indices[0] = 0;  indices[1] = 1;  indices[2] = 3;
	indices[3] = 0;  indices[4] = 3;  indices[5] = 2;

	// The vertex format shared by the room walls.
	VertexFormat* vformat = VertexFormat::Create(2,
		VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
		VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
	int vstride = vformat->GetStride();
	VertexBufferAccessor vba;

	// +x wall
	vbuffer = new0 VertexBuffer(4, vstride);
	vba.ApplyTo(vformat, vbuffer);
	vba.Position<Float3>(0) = Float3(+skyBoxSize, +skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(1) = Float3(+skyBoxSize, -skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(2) = Float3(+skyBoxSize, +skyBoxSize, +skyBoxSize);
	vba.Position<Float3>(3) = Float3(+skyBoxSize, -skyBoxSize, +skyBoxSize);
	vba.TCoord<Float2>(0, 0) = Float2(0.0f, 0.0f);
	vba.TCoord<Float2>(0, 1) = Float2(1.0f, 0.0f);
	vba.TCoord<Float2>(0, 2) = Float2(0.0f, 1.0f);
	vba.TCoord<Float2>(0, 3) = Float2(1.0f, 1.0f);
	wall = new0 TriMesh(vformat, vbuffer, ibuffer);
	node->AttachChild(wall);

	std::string texPath = skyDirPath + "right.png";
	Object *tex2DObj = PX2_RM.BlockLoad(texPath);
	MaterialInstance *mi = new0 MaterialInstance("Data/engine_mtls/tex2d/tex2d.px2obj", 
		"tex2d", false);
	wall->SetMaterialInstance(mi);
	mi->SetPixelTexture(0, "SampleBase", DynamicCast<Texture2D>(tex2DObj));

	// -x wall
	vbuffer = new0 VertexBuffer(4, vstride);
	vba.ApplyTo(vformat, vbuffer);
	vba.Position<Float3>(0) = Float3(-skyBoxSize, -skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(1) = Float3(-skyBoxSize, +skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(2) = Float3(-skyBoxSize, -skyBoxSize, +skyBoxSize);
	vba.Position<Float3>(3) = Float3(-skyBoxSize, +skyBoxSize, +skyBoxSize);
	vba.TCoord<Float2>(0, 0) = Float2(0.0f, 0.0f);
	vba.TCoord<Float2>(0, 1) = Float2(1.0f, 0.0f);
	vba.TCoord<Float2>(0, 2) = Float2(0.0f, 1.0f);
	vba.TCoord<Float2>(0, 3) = Float2(1.0f, 1.0f);
	wall = new0 TriMesh(vformat, vbuffer, ibuffer);
	node->AttachChild(wall);

	texPath = skyDirPath + "left.png";
	tex2DObj = PX2_RM.BlockLoad(texPath);
	mi = new0 MaterialInstance(
		"Data/engine_mtls/tex2d/tex2d.px2obj", "tex2d", false);
	wall->SetMaterialInstance(mi);
	mi->SetPixelTexture(0, "SampleBase", DynamicCast<Texture2D>(tex2DObj));

	// +y wall
	vbuffer = new0 VertexBuffer(4, vstride);
	vba.ApplyTo(vformat, vbuffer);
	vba.Position<Float3>(0) = Float3(-skyBoxSize, +skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(1) = Float3(+skyBoxSize, +skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(2) = Float3(-skyBoxSize, +skyBoxSize, +skyBoxSize);
	vba.Position<Float3>(3) = Float3(+skyBoxSize, +skyBoxSize, +skyBoxSize);
	vba.TCoord<Float2>(0, 0) = Float2(0.0f, 0.0f);
	vba.TCoord<Float2>(0, 1) = Float2(1.0f, 0.0f);
	vba.TCoord<Float2>(0, 2) = Float2(0.0f, 1.0f);
	vba.TCoord<Float2>(0, 3) = Float2(1.0f, 1.0f);
	wall = new0 TriMesh(vformat, vbuffer, ibuffer);
	node->AttachChild(wall);

	texPath = skyDirPath + "front.png";
	tex2DObj = PX2_RM.BlockLoad(texPath);
	mi = new0 MaterialInstance("Data/engine_mtls/tex2d/tex2d.px2obj", "tex2d",
		false);
	wall->SetMaterialInstance(mi);
	mi->SetPixelTexture(0, "SampleBase", DynamicCast<Texture2D>(tex2DObj));

	// -y wall
	vbuffer = new0 VertexBuffer(4, vstride);
	vba.ApplyTo(vformat, vbuffer);
	vba.Position<Float3>(0) = Float3(+skyBoxSize, -skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(1) = Float3(-skyBoxSize, -skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(2) = Float3(+skyBoxSize, -skyBoxSize, +skyBoxSize);
	vba.Position<Float3>(3) = Float3(-skyBoxSize, -skyBoxSize, +skyBoxSize);
	vba.TCoord<Float2>(0, 0) = Float2(0.0f, 0.0f);
	vba.TCoord<Float2>(0, 1) = Float2(1.0f, 0.0f);
	vba.TCoord<Float2>(0, 2) = Float2(0.0f, 1.0f);
	vba.TCoord<Float2>(0, 3) = Float2(1.0f, 1.0f);
	wall = new0 TriMesh(vformat, vbuffer, ibuffer);
	node->AttachChild(wall);

	texPath = skyDirPath + "back.png";
	tex2DObj = PX2_RM.BlockLoad(texPath);
	mi = new0 MaterialInstance("Data/engine_mtls/tex2d/tex2d.px2obj", "tex2d", 
		false);
	wall->SetMaterialInstance(mi);
	mi->SetPixelTexture(0, "SampleBase", DynamicCast<Texture2D>(tex2DObj));

	// +z wall
	vbuffer = new0 VertexBuffer(4, vstride);
	vba.ApplyTo(vformat, vbuffer);
	vba.Position<Float3>(0) = Float3(-skyBoxSize, +skyBoxSize, +skyBoxSize);
	vba.Position<Float3>(1) = Float3(+skyBoxSize, +skyBoxSize, +skyBoxSize);
	vba.Position<Float3>(2) = Float3(-skyBoxSize, -skyBoxSize, +skyBoxSize);
	vba.Position<Float3>(3) = Float3(+skyBoxSize, -skyBoxSize, +skyBoxSize);
	vba.TCoord<Float2>(0, 0) = Float2(0.0f, 0.0f);
	vba.TCoord<Float2>(0, 1) = Float2(1.0f, 0.0f);
	vba.TCoord<Float2>(0, 2) = Float2(0.0f, 1.0f);
	vba.TCoord<Float2>(0, 3) = Float2(1.0f, 1.0f);
	wall = new0 TriMesh(vformat, vbuffer, ibuffer);
	node->AttachChild(wall);

	texPath = skyDirPath + "up.png";
	tex2DObj = PX2_RM.BlockLoad(texPath);
	mi = new0 MaterialInstance("Data/engine_mtls/tex2d/tex2d.px2obj", "tex2d",
		false);
	wall->SetMaterialInstance(mi);
	mi->SetPixelTexture(0, "SampleBase", DynamicCast<Texture2D>(tex2DObj));

	// -z wall
	vbuffer = new0 VertexBuffer(4, vstride);
	vba.ApplyTo(vformat, vbuffer);
	vba.Position<Float3>(0) = Float3(-skyBoxSize, -skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(1) = Float3(+skyBoxSize, -skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(2) = Float3(-skyBoxSize, +skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(3) = Float3(+skyBoxSize, +skyBoxSize, -skyBoxSize);
	vba.TCoord<Float2>(0, 0) = Float2(0.0f, 0.0f);
	vba.TCoord<Float2>(0, 1) = Float2(1.0f, 0.0f);
	vba.TCoord<Float2>(0, 2) = Float2(0.0f, 1.0f);
	vba.TCoord<Float2>(0, 3) = Float2(1.0f, 1.0f);
	wall = new0 TriMesh(vformat, vbuffer, ibuffer);
	node->AttachChild(wall);

	texPath = skyDirPath + "down.png";
	tex2DObj = PX2_RM.BlockLoad(texPath);
	mi = new0 MaterialInstance("Data/engine_mtls/tex2d/tex2d.px2obj", "tex2d",
		false);
	wall->SetMaterialInstance(mi);
	mi->SetPixelTexture(0, "SampleBase", DynamicCast<Texture2D>(tex2DObj));

	return node;
}
//----------------------------------------------------------------------------
Movable *Creater::CreateTerrain()
{
	RawTerrain *rawTerrain = new0 RawTerrain();
	rawTerrain->SetSize(129);
	rawTerrain->SetRowQuantity(4);
	rawTerrain->SetColQuantity(4);
	rawTerrain->SetSpacing(2);
	rawTerrain->AllocateRawTerrainPages();

	return rawTerrain;
}
//----------------------------------------------------------------------------
UIFrame *Creater::CreateUIFrame()
{
	UIFrame *frame = new0 UIFrame();	
	return frame;
}
//----------------------------------------------------------------------------
UIFPicBox *Creater::CreateUIFPicBox()
{
	UIFPicBox *fPicBox = new0 UIFPicBox();
	return fPicBox;
}
//----------------------------------------------------------------------------
UIFText *Creater::CreateUIFText()
{
	UIFText *fText = new0 UIFText();
	return fText;
}
//----------------------------------------------------------------------------
UIButton *Creater::CreateUIButton()
{
	UIButton *but = new0 UIButton();
	return but;
}
//----------------------------------------------------------------------------
UICheckButton *Creater::CreateUICheckButton()
{
	UICheckButton *checkButton = new0 UICheckButton();
	return checkButton;
}
//----------------------------------------------------------------------------
UIComboBox *Creater::CreateUIComboBox()
{
	UIComboBox *comboBox = new0 UIComboBox();
	return comboBox;
}
//----------------------------------------------------------------------------
UIEditBox *Creater::CreateUIEditBox()
{
	UIEditBox *editBox = new0 UIEditBox();
	return editBox;
}
//----------------------------------------------------------------------------
UIProgressBar *Creater::CreateUIProgressBar()
{
	UIProgressBar *pb = new0 UIProgressBar();
	return pb;
}
//----------------------------------------------------------------------------
InterpCurveAlphaController *Creater::CreateICC_Alpha()
{
	InterpCurveAlphaController *ctrl = new0 InterpCurveAlphaController();
	return ctrl;
}
//----------------------------------------------------------------------------
InterpCurveColorController *Creater::CreateICC_Color()
{
	InterpCurveColorController *ctrl = new0 InterpCurveColorController();
	return ctrl;
}
//----------------------------------------------------------------------------
InterpCurveUniformScaleController *Creater::CreateICC_UniformScale()
{
	InterpCurveUniformScaleController *ctrl =
		new0 InterpCurveUniformScaleController();
	return ctrl;
}
//----------------------------------------------------------------------------
InterpCurveScaleController *Creater::CreateICC_Scale()
{
	InterpCurveScaleController *ctrl = new0 InterpCurveScaleController();
	return ctrl;
}
//----------------------------------------------------------------------------
InterpCurveRotateController *Creater::CreateICC_Rotate()
{
	InterpCurveRotateController *ctrl = new0 InterpCurveRotateController();
	return ctrl;
}
//----------------------------------------------------------------------------
InterpCurveTranslateController *Creater::CreateICC_Translate()
{
	InterpCurveTranslateController *ctrl = new0 InterpCurveTranslateController();
	return ctrl;
}
//----------------------------------------------------------------------------
TriggerController *Creater::CreateTriggerController()
{
	TriggerController *ctrl = new0 TriggerController();
	return ctrl;
}
//----------------------------------------------------------------------------
BPPackage *Creater::CreateBPPackage()
{
	BPPackage *lg = new0 BPPackage();
	return lg;
}
//----------------------------------------------------------------------------
BPFile *Creater::CreateBPFile()
{
	BPFile *lf = new0 BPFile();
	return lf;
}
//----------------------------------------------------------------------------
BPModule *Creater::CreateBPModule(const std::string &className, 
	const std::string &funName)
{
	FunObject *funObjObject = FunObjectManager::GetSingleton()
		.GetObjectFunObject();
	FunObject *funObj = funObjObject->GetFunObject(className, funName);
	if (funObj)
	{
		BPModule *logicModule = new0 BPModule(BPModule::MT_FUNCTION_OBJECT);
		logicModule->RegistFunObj(*funObj);

		return logicModule;
	}

	return 0;
}
//----------------------------------------------------------------------------
BPModule *Creater::CreateBPModuleGeneral(const std::string &genFunName)
{
	std::string scriptName = PX2_LOGICM.GetGFScript(genFunName);
	FunObject *funObj = PX2_LOGICM.GetGF(genFunName);

	if (!scriptName.empty() && funObj)
	{
		BPModule *logicModule = new0 BPModule(BPModule::MT_FUNCTION_GENERAL);
		logicModule->RegistFunObj(*funObj);

		return logicModule;
	}

	return 0;
}
//----------------------------------------------------------------------------
BPModule *Creater::CreateBPModuleFunctionStart()
{
	BPModule *logicModule = new0 BPModule(BPModule::MT_FUNCTION_START);
	logicModule->RegistFunObj(*PX2_LOGICM.GetStartGF());

	return logicModule;
}
//----------------------------------------------------------------------------
BPModule *Creater::CreateBPEvent(const std::string &spaceName,
	const std::string &eventName)
{
	FunObject *eventObj = PX2_LOGICM.GetEvent(spaceName, eventName);
	if (eventObj)
	{
		BPModule *logicModule = new0 BPModule(BPModule::MT_EVENT);
		logicModule->RegistFunObj(*eventObj);

		return logicModule;
	}

	return 0;
}
//----------------------------------------------------------------------------
BPModule *Creater::CreateBPOption(const std::string &optionName)
{
	FunObject *eventObj = PX2_LOGICM.GetOption(optionName);
	if (eventObj)
	{
		BPModule *logicModule = new0 BPModule(BPModule::MT_OPTION);
		logicModule->RegistFunObj(*eventObj);

		return logicModule;
	}

	return 0;
}
//----------------------------------------------------------------------------
BPModule *Creater::CreateBPOperator(const std::string &operatorName)
{
	FunObject *eventObj = PX2_LOGICM.GetOperator(operatorName);
	if (eventObj)
	{
		BPModule *logicModule = new0 BPModule(BPModule::MT_FUNCTION_OPERATOR);
		logicModule->RegistFunObj(*eventObj);

		return logicModule;
	}

	return 0;
}
//----------------------------------------------------------------------------
BPModule *Creater::CreateBPParam(const std::string &paramName)
{
	FunObject *paramObj = PX2_LOGICM.GetParam(paramName);
	if (paramObj)
	{
		BPModule *logicModule = new0 BPModule(BPModule::MT_PARAM);
		logicModule->RegistFunObj(*paramObj);

		return logicModule;
	}

	return 0;
}
//----------------------------------------------------------------------------
void Creater::AddObject(Object *parent, Object *obj,
	bool command)
{
	Movable *mov = DynamicCast<Movable>(obj);
	Controller *ctrl = DynamicCast<Controller>(obj);
	EffectModule *eftModule = DynamicCast<EffectModule>(obj);

	Node *nodePar = DynamicCast<Node>(parent);
	Controlledable *ctrlablePar = DynamicCast<Controlledable>(parent);
	EffectableController *effectableCtrl = DynamicCast<EffectableController>(parent);

	bool added = false;
	if (mov && nodePar)
	{
		nodePar->AttachChild(mov);
		added = true;
	}
	else if (ctrlablePar && ctrl)
	{
		ctrlablePar->AttachController(ctrl);
		added = true;
	}
	else if (effectableCtrl && eftModule)
	{
		effectableCtrl->AddModule(eftModule);
		added = true;
	}

	if (added && command)
	{
		ObjectAddDeleteURDo *cmd = new0 ObjectAddDeleteURDo(true, obj);
		PX2_URDOM.PushUnDo(cmd);
	}
}
//----------------------------------------------------------------------------
bool Creater::RemoveObject(Object *obj, bool command)
{
	Movable *mov = DynamicCast<Movable>(obj);
	Controller *ctrl = DynamicCast<Controller>(obj);
	EffectModule *module = DynamicCast<EffectModule>(obj);
	Animation *anim = DynamicCast<Animation>(obj);
	PX2_UNUSED(anim);

	URDoPtr theCommand = new0 ObjectAddDeleteURDo(false, obj);

	bool removed = false;

	if (mov)
	{
		Node *parent = DynamicCast<Node>(mov->GetParent());
		if (parent)
		{
			int ret = parent->DetachChild(mov);
			if (-1 != ret)
			{
				removed = true;
			}
		}
	}
	else if (ctrl)
	{
		Controlledable *ctrlAble = ctrl->GetControlledable();
		ctrlAble->DetachController(ctrl);

		removed = true;
	}
	else if (module)
	{
		EffectableController *effCtrl = module->GetEffectableController();
		effCtrl->RemoveModule(module);

		removed = true;
	}

	if (removed && command)
	{
		PX2_URDOM.PushUnDo(theCommand);
	}

	return true;
}
//----------------------------------------------------------------------------