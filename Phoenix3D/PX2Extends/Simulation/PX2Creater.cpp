// PX2Creater.cpp

#include "PX2Creater.hpp"
#include "PX2Actor.hpp"
#include "PX2EffectActor.hpp"
#include "PX2Scene.hpp"
#include "PX2Controller.hpp"
#include "PX2Animation.hpp"
#include "PX2SimulationEventType.hpp"
#include "PX2RedoUndo.hpp"
#include "PX2Project.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2EffectableController.hpp"
#include "PX2EffectModule.hpp"
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
Object *Creater::New0(const std::string &className)
{
	PX2_UNUSED(className);

	return 0;
}
//----------------------------------------------------------------------------
Actor *Creater::CreateActor_Rectangle(Node *parent, const APoint &pos, 
	bool posIsLocal)
{
	Movable *mov = CreateRectangle(0, APoint::ORIGIN, true);

	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	ActorPtr actor = new0 Actor();
	actor->SetMovable(mov);
	actor->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, actor);

	return actor;
}
//----------------------------------------------------------------------------
Actor *Creater::CreateActor_Box(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	Movable *mov = CreateBox(0, APoint::ORIGIN, true);

	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	ActorPtr actor = new0 Actor();
	actor->SetMovable(mov);
	actor->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, actor);

	return actor;
}
//----------------------------------------------------------------------------
Actor *Creater::CreateActor_Sphere(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	Movable *mov = CreateSphere(0, APoint::ORIGIN, true);

	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	ActorPtr actor = new0 Actor();
	actor->SetMovable(mov);
	actor->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, actor);

	return actor;
}
//----------------------------------------------------------------------------
Actor *Creater::CreateActor_Effect(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	ActorPtr actor = new0 EffectActor();
	actor->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, actor);

	return actor;
}
//----------------------------------------------------------------------------
Actor *Creater::CreateActor_Actor(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	ActorPtr actor = new0 Actor();
	actor->LocalTransform.SetTranslate(localPos);

	VertexFormat *vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PC);
	StandardMesh stdMesh(vf);
	stdMesh.SetVertexColor(Float4::WHITE);
	TriMesh *mesh = stdMesh.Box(0.5f, 0.5f, 0.5f);
	PX2::VertexColor4MaterialPtr mtl = new0 VertexColor4Material();
	mtl->GetWireProperty(0, 0)->Enabled = true;
	mtl->GetCullProperty(0, 0)->Enabled = false;
	mesh->LocalTransform.SetUniformScale(0.5f);
	mesh->SetMaterialInstance(mtl->CreateInstance());
	actor->CreateGetHelpNode()->DetachAllChildren();
	actor->CreateGetHelpNode()->AttachChild(mesh);
	actor->CreateGetHelpNode()->SetParentTransformIngore(false, false, true);

	if (parent)
		AddObject(parent, actor);

	return actor;
}
//----------------------------------------------------------------------------
Character *Creater::CreateActor_Character(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	CharacterPtr chara = new0 Character();
	chara->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, chara);

	return chara;
}
//----------------------------------------------------------------------------
TriggerActor *Creater::CreateActor_Trigger(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	TriggerActor *actor = new0 TriggerActor();
	actor->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, actor);

	return actor;
}
//----------------------------------------------------------------------------
SkyActor *Creater::CreateActor_Sky(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	SkyActor *actor = new0 SkyActor();
	actor->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, actor);

	return actor;
}
//----------------------------------------------------------------------------
LightActor *Creater::CreateActor_Light(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	LightActor *actor = new0 LightActor();
	actor->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, actor);

	return actor;
}
//----------------------------------------------------------------------------
TerrainActor *Creater::CreateActor_Terrain(Node *parent, const APoint &pos,
	const std::string &name, int terrainSize, int pageSize,
	float gridSpacing, bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	TerrainActor *actor = new0 TerrainActor();
	actor->SetName(name);
	actor->LocalTransform.SetTranslate(localPos);

	RawTerrain *rawTerrain = new0 RawTerrain();
	rawTerrain->SetName("RawTerrain");
	rawTerrain->SetSize(pageSize);
	int quantity = terrainSize / (pageSize - 1);
	rawTerrain->SetRowQuantity(quantity);
	rawTerrain->SetColQuantity(quantity);
	rawTerrain->SetSpacing(gridSpacing);
	rawTerrain->AllocateRawTerrainPages();

	actor->SetRawTerrain(rawTerrain);

	if (parent)
		AddObject(parent, actor);

	return actor;
}
//----------------------------------------------------------------------------
Movable *Creater::CreateRectangle(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	APoint localPos = pos;

	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	Texture2D *tex = DynamicCast<Texture2D>(PX2_RM.BlockLoad(
		"Data/engine/default.png"));
	if (!tex) return 0;

	VertexFormat *vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PNT1);

	StandardMesh stdMesh(vf);
	TriMesh *mesh = stdMesh.Rectangle(2, 2, 1.0f, 1.0f, Float2(0.5f, 0.5f));
	mesh->SetName("NoName");

	MaterialInstance *mi = new0 MaterialInstance(
		"Data/engine_mtls/std/std.px2obj", "std_lightshadow", false);
	mesh->SetMaterialInstance(mi);

	mi->SetPixelTexture(0, "SampleBase", tex);

	mesh->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, mesh);

	return mesh;
}
//----------------------------------------------------------------------------
Movable *Creater::CreateBox(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	APoint localPos = pos;

	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	Texture2D *tex = DynamicCast<Texture2D>(PX2_RM.BlockLoad(
		"Data/engine/default.png"));
	if (!tex) return 0;

	VertexFormat *vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PNT1);

	StandardMesh stdMesh(vf);
	Node *meshNode = stdMesh.BoxPieces(2, 1);
	meshNode->SetName("NoName");

	for (int i = 0; i < meshNode->GetNumChildren(); i++)
	{
		TriMesh *mesh = DynamicCast<TriMesh>(meshNode->GetChild(i));

		if (mesh)
		{
			MaterialInstance *mi = new0 MaterialInstance(
				"Data/engine_mtls/std/std.px2obj", "std_lightshadow", false);
			mesh->SetMaterialInstance(mi);

			mi->SetPixelTexture(0, "SampleBase", tex);
		}
	}

	meshNode->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, meshNode);

	return meshNode;
}
//----------------------------------------------------------------------------
Movable *Creater::CreateSphere(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	APoint localPos = pos;

	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	Texture2D *tex = DynamicCast<Texture2D>(PX2_RM.BlockLoad(
		"Data/engine/default.png"));
	if (!tex) return 0;

	VertexFormat *vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PNT1);

	StandardMesh stdMesh(vf);
	TriMesh *mesh = stdMesh.Sphere(32, 32, 1);
	mesh->SetName("NoName");

	MaterialInstance *mi = new0 MaterialInstance(
		"Data/engine_mtls/std/std.px2obj", "std_lightshadow", false);
	mesh->SetMaterialInstance(mi);

	mi->SetPixelTexture(0, "SampleBase", tex);

	mesh->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, mesh);

	return mesh;
}
//----------------------------------------------------------------------------
Movable *Creater::CreateTerrainPage(Terrain *terrain, 
	int indexRow, int indexCol)
{
	RawTerrain *rawTerrain = DynamicCast<RawTerrain>(terrain);
	if (rawTerrain)
	{
		RawTerrainPage *terrainPage = rawTerrain->CreatePage(indexRow, indexCol);
		AddObject(rawTerrain, terrainPage);

		return terrainPage;
	}

	return 0;
}
//----------------------------------------------------------------------------
UIFrame *Creater::CreateUIFrame(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	UIFrame *frame = new0 UIFrame();
	frame->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, frame);

	return frame;
}
//----------------------------------------------------------------------------
UIPicBox *Creater::CreateUIPicBox(Node *parent, const APoint &pos, 
	const std::string &filename, bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	UIPicBox *picBox = new0 UIPicBox(filename);
	picBox->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, picBox);

	return picBox;
}
//----------------------------------------------------------------------------
UIPicBox *Creater::CreateUIPicBox(Node *parent, const APoint &pos,
	const std::string &texPack, const std::string &eleName,
	bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	UIPicBox *picBox = new0 UIPicBox(texPack, eleName, false);
	picBox->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, picBox);

	return picBox;
}
//----------------------------------------------------------------------------
UIText *Creater::CreateUIText(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	UIText *text = new0 UIText();
	text->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, text);

	return text;
}
//----------------------------------------------------------------------------
UIButton *Creater::CreateUIButton(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	UIButton *but = new0 UIButton();
	but->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, but);

	return but;
}
//----------------------------------------------------------------------------
UICheckButton *Creater::CreateUICheckButton(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	UICheckButton *checkButton = new0 UICheckButton();
	checkButton->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, checkButton);

	return checkButton;
}
//----------------------------------------------------------------------------
UIEditBox *Creater::CreateUIEditBox(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	UIEditBox *editBox = new0 UIEditBox();
	editBox->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, editBox);

	return editBox;
}
//----------------------------------------------------------------------------
UIProgressBar *Creater::CreateUIProgressBar(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	UIProgressBar *progBar = new0 UIProgressBar();
	progBar->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, progBar);

	return progBar;
}
//----------------------------------------------------------------------------
ParticleEmitter *Creater::CreateParticleEmitter(Node *parent, 
	const APoint &pos, bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	ParticleEmitter *mov = new0 ParticleEmitter();
	mov->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, mov);

	mov->ResetPlay();

	return mov;
}
//----------------------------------------------------------------------------
Billboard *Creater::CreateBillboard(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	Billboard *mov = new0 Billboard();
	mov->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, mov);

	mov->ResetPlay();

	return mov;
}
//----------------------------------------------------------------------------
BeamEmitter *Creater::CreateBeamEmitter(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	BeamEmitter *mov = new0 BeamEmitter();
	mov->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, mov);

	mov->ResetPlay();

	return mov;
}
//----------------------------------------------------------------------------
RibbonEmitter *Creater::CreateRibbonEmitter(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	RibbonEmitter *mov = new0 RibbonEmitter();
	mov->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, mov);

	mov->ResetPlay();

	return mov;
}
//----------------------------------------------------------------------------
Soundable *Creater::CreateSoundable(Node *parent, const APoint &pos,
	bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	Soundable *mov = new0 Soundable();
	mov->LocalTransform.SetTranslate(localPos);

	if (parent)
		AddObject(parent, mov);

	return mov;
}
//----------------------------------------------------------------------------
InterpCurveAlphaController *Creater::CreateICC_Alpha(Movable *mov)
{
	InterpCurveAlphaController *ctrl = new0 InterpCurveAlphaController();

	if (mov)
		AddObject(mov, ctrl);

	return ctrl;
}
//----------------------------------------------------------------------------
InterpCurveColorController *Creater::CreateICC_Color(Movable *mov)
{
	InterpCurveColorController *ctrl = new0 InterpCurveColorController();

	if (mov)
		AddObject(mov, ctrl);

	return ctrl;
}
//----------------------------------------------------------------------------
InterpCurveUniformScaleController *Creater::CreateICC_UniformScale(Movable *mov)
{
	InterpCurveUniformScaleController *ctrl = new0 InterpCurveUniformScaleController();

	if (mov)
		AddObject(mov, ctrl);

	return ctrl;
}
//----------------------------------------------------------------------------
InterpCurveScaleController *Creater::CreateICC_Scale(Movable *mov)
{
	InterpCurveScaleController *ctrl = new0 InterpCurveScaleController();

	if (mov)
		AddObject(mov, ctrl);

	return ctrl;
}
//----------------------------------------------------------------------------
InterpCurveRotateController *Creater::CreateICC_Rotate(Movable *mov)
{
	InterpCurveRotateController *ctrl = new0 InterpCurveRotateController();

	if (mov)
		AddObject(mov, ctrl);

	return ctrl;
}
//----------------------------------------------------------------------------
InterpCurveTranslateController *Creater::CreateICC_Translate(Movable *mov)
{
	InterpCurveTranslateController *ctrl = new0 InterpCurveTranslateController();

	if (mov)
		AddObject(mov, ctrl);

	return ctrl;
}
//----------------------------------------------------------------------------
Node *Creater::CreateNode(Node *parent)
{
	Node *node = new0 Node();

	if (parent)
		AddObject(parent, node);

	return node;
}
//----------------------------------------------------------------------------
Actor *Creater::CreateActor(Node *parent, const std::string &resPath,
	const APoint &pos, bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	Movable *mov = DynamicCast<Movable>(PX2_RM.BlockLoad(resPath));
	if (mov)
	{
		Actor *actor = new0 Actor();
		actor->SetMovableFilename(resPath);
		actor->LocalTransform.SetTranslate(localPos);

		if (parent)
			AddObject(parent, actor);

		return actor;
	}

	return 0;
}
//----------------------------------------------------------------------------
Character *Creater::CreateCharacter(Node *parent, const std::string &resPath,
	const APoint &pos, bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	Movable *mov = DynamicCast<Movable>(PX2_RM.BlockLoad(resPath));
	if (mov)
	{
		Character *chara = new0 Character();
		chara->SetMovableFilename(resPath);
		chara->LocalTransform.SetTranslate(localPos);

		if (parent)
			AddObject(parent, chara);

		return chara;
	}

	return 0;
}
//----------------------------------------------------------------------------
EffectActor *Creater::CreateEffectActor(Node *parent, const std::string &resPath,
	const APoint &pos, bool posIsLocal)
{
	APoint localPos = pos;
	if (parent && !posIsLocal)
	{
		localPos = parent->WorldTransform.Inverse() * localPos;
	}

	Movable *mov = DynamicCast<Movable>(PX2_RM.BlockLoad(resPath));
	if (mov)
	{
		EffectActor *ea = new0 EffectActor();
		ea->SetMovableFilename(resPath);
		ea->LocalTransform.SetTranslate(localPos);

		if (parent)
			AddObject(parent, ea);

		return ea;
	}

	return 0;
}
//----------------------------------------------------------------------------
void Creater::AddObject(Object *parent, Object *obj, bool command)
{
	Movable *mov = DynamicCast<Movable>(obj);
	Controller *ctrl = DynamicCast<Controller>(obj);
	EffectModule *eftModule = DynamicCast<EffectModule>(obj);

	Node *nodePar = DynamicCast<Node>(parent);
	Controlledable *ctrlablePar = DynamicCast<Controlledable>(parent);
	EffectableController *effectableCtrl = DynamicCast<EffectableController>(parent);

	bool added = false;

	//else if (character && anim3d)
	//{
	//	character->AddAnim(anim3d);

	//	added = true;
	//}
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

	if (added)
	{
		Event *ent = SimuES_E::CreateEventX(SimuES_E::AddObject);
		ent->SetData<Object*>(obj);
		EventWorld::GetSingleton().BroadcastingLocalEvent(ent);
	}

	if (added && command)
	{
		ObjectAddDeleteURDo *cmd = new0 ObjectAddDeleteURDo(true, obj);
		PX2_URDOMAN.PushUnDo(cmd);
	}
}
//----------------------------------------------------------------------------
bool Creater::RemoveObject(Object *obj, bool command)
{
	Movable *mov = DynamicCast<Movable>(obj);
	Controller *ctrl = DynamicCast<Controller>(obj);
	EffectModule *module = DynamicCast<EffectModule>(obj);
	Animation *anim = DynamicCast<Animation>(obj);

	URDoPtr theCommand = new0 ObjectAddDeleteURDo(false, obj);

	bool removed = false;

	if (anim)
	{
		//Character *chara = anim->GetCharacter();
		//if (chara)
		//{
		//	chara->RemoveAnim(anim);

		//	removed = true;
		//}
	}
	else if (mov)
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

	if (removed)
	{
		Event *ent = 0;
		ent = SimuES_E::CreateEventX(SimuES_E::RemoveObject);
		ent->SetData<Object*>(obj);
		EventWorld::GetSingleton().BroadcastingLocalEvent(ent);
	}

	if (removed && command)
	{
		PX2_URDOMAN.PushUnDo(theCommand);
	}

	return true;
}
//----------------------------------------------------------------------------
Controlledable *Creater::ConvertToControlledable(Object *obj)
{
	return DynamicCast<Controlledable>(obj);
}
//----------------------------------------------------------------------------
Movable *Creater::ConvertToMovable(Object *obj)
{
	return DynamicCast<Movable>(obj);
}
//----------------------------------------------------------------------------
Node *Creater::ConvertToNode(Object *obj)
{
	return DynamicCast<Node>(obj);
}
//----------------------------------------------------------------------------
Actor *Creater::ConvertToActor(Object *obj)
{
	return DynamicCast<Actor>(obj);
}
//----------------------------------------------------------------------------
UIPicBox *Creater::ConvertToUIPicBox(Object *obj)
{
	return DynamicCast<UIPicBox>(obj);
}
//----------------------------------------------------------------------------
UIFrame *Creater::ConvertToUIFrame(Object *obj)
{
	return DynamicCast<UIFrame>(obj);
}
//----------------------------------------------------------------------------
InterpCurveController *Creater::ConvertToInterpCurveController(Object *obj)
{
	return DynamicCast<InterpCurveController>(obj);
}
//----------------------------------------------------------------------------
EffectableController *Creater::ConvertToEffectableController(Object *obj)
{
	return DynamicCast<EffectableController>(obj);
}
//----------------------------------------------------------------------------
EffectModule *Creater::ConvertToEffectModule(Object *obj)
{
	return DynamicCast<EffectModule>(obj);
}
//----------------------------------------------------------------------------