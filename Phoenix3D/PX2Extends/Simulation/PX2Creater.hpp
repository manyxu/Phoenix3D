// PX2Creater.hpp

#ifndef PX2CREATER_HPP
#define PX2CREATER_HPP

#include "PX2SimulationPre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2Object.hpp"
#include "PX2APoint.hpp"
#include "PX2Actor.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIPicBox.hpp"
#include "PX2UIText.hpp"
#include "PX2UIButton.hpp"
#include "PX2UIEditBox.hpp"
#include "PX2UICheckButton.hpp"
#include "PX2UIProgressBar.hpp"
#include "PX2ParticleEmitter.hpp"
#include "PX2Billboard.hpp"
#include "PX2BeamEmitter.hpp"
#include "PX2RibbonEmitter.hpp"
#include "PX2Soundable.hpp"
#include "PX2Character.hpp"
#include "PX2LightActor.hpp"
#include "PX2TerrainActor.hpp"
#include "PX2BPPackage.hpp"
#include "PX2InterpCurveBrightnessCtrl.hpp"
using namespace PX2;

namespace PX2
{

	class Scene;

	class PX2_EXTENDS_ITEM Creater : public Singleton<Creater>
	{
	public:
		Creater();
		virtual ~Creater();

		Actor *CreateActor_Rectangle(Node *parent, const APoint &pos, bool posIsLocal);
		Actor *CreateActor_Box(Node *parent, const APoint &pos, bool posIsLocal);
		Actor *CreateActor_Sphere(Node *parent, const APoint &pos, bool posIsLocal);
		Actor *CreateActor_Actor(Node *parent, const APoint &pos, bool posIsLocal);
		Character *CreateActor_Character(Node *parent, const APoint &pos, bool posIsLocal);
		TerrainActor *CreateActor_Terrain(Node *parent, const APoint &pos,
			const std::string &name, int terrainSize, int pageSize, float gridSpacing, bool posIsLocal);
		LightActor *CreateActor_Light(Node *scene, const APoint &pos, bool posIsLocal);

		Movable *CreateRectangle(Node *parent, const APoint &pos, bool posIsLocal);
		Movable *CreateBox(Node *parent, const APoint &pos, bool posIsLocal);
		Movable *CreateSphere(Node *parent, const APoint &pos, bool posIsLocal);
		Movable *CreateTerrainPage(Terrain *terrain, int indexRow, int indexCol);

		UIFrame *CreateUIFrame(Node *parent, const APoint &pos, bool posIsLocal);
		UIPicBox *CreateUIPicBox(Node *parent, const APoint &pos, const std::string &filename, bool posIsLocal);
		UIPicBox *CreateUIPicBox(Node *parent, const APoint &pos, const std::string &texPack,
			const std::string &eleName, bool posIsLocal);
		UIText *CreateUIText(Node *parent, const APoint &pos, bool posIsLocal);
		UIButton *CreateUIButton(Node *parent, const APoint &pos, bool posIsLocal);
		UICheckButton *CreateUICheckButton(Node *parent, const APoint &pos, bool posIsLocal);
		UIEditBox *CreateUIEditBox(Node *parent, const APoint &pos, bool posIsLocal);
		UIProgressBar *CreateUIProgressBar(Node *parent, const APoint &pos, bool posIsLocal);

		ParticleEmitter *CreateParticleEmitter(Node *parent, const APoint &pos, bool posIsLocal);
		Billboard *CreateBillboard(Node *parent, const APoint &pos, bool posIsLocal);
		BeamEmitter *CreateBeamEmitter(Node *parent, const APoint &pos, bool posIsLocal);
		RibbonEmitter *CreateRibbonEmitter(Node *parent, const APoint &pos, bool posIsLocal);
		Soundable *CreateSoundable(Node *parent, const APoint &pos, bool posIsLocal);

		InterpCurveAlphaController *CreateICC_Alpha(Movable *mov);
		InterpCurveColorController *CreateICC_Color(Movable *mov);
		InterpCurveBrightnessController *CreateICC_Brightness(Movable *mov);
		InterpCurveUniformScaleController *CreateICC_UniformScale(Movable *mov);
		InterpCurveScaleController *CreateICC_Scale(Movable *mov);
		InterpCurveRotateController *CreateICC_Rotate(Movable *mov);
		InterpCurveTranslateController *CreateICC_Translate(Movable *mov);

		Node *CreateNode(Node *parent, const APoint &pos, bool posIsLocal);

		Actor *CreateActor(Node *parent, const std::string &resPath, const APoint &pos, bool posIsLocal);
		Character *CreateCharacter(Node *parent, const std::string &resPath, const APoint &pos, bool posIsLocal);

		BPPackage *CreateBPPackage(Node *parent, const APoint &pos, bool posIsLocal);
		BPFile *CreateBPFile(Node *parent, const APoint &pos, bool posIsLocal);
		BPModule *CreateBPModule(Node *parent,
			const std::string &className, const std::string &funName, const APoint &pos, bool posIsLocal);
		BPModule *CreateBPModuleGeneral(Node *parent,
			const std::string &genFunName, const APoint &pos, bool posIsLocal);
		BPModule *CreateBPModuleFunctionStart(Node *parent, const APoint &pos, bool posIsLocal);
		BPModule *CreateBPEvent(Node *parent, const std::string &eventName, const APoint &pos, bool posIsLocal);
		BPModule *CreateBPOption(Node *parent, const std::string &optionName, const APoint &pos, bool posIsLocal);
		BPModule *CreateBPOperator(Node *parent, const std::string &operatorName, const APoint &pos, bool posIsLocal);
		BPModule *CreateBPParam(Node *parent, const std::string &paramName, const APoint &pos, bool posIsLocal);

		void AddObject(Object *parent, Object *obj, bool command = true);
		bool RemoveObject(Object *obj, bool command = true);

	public:
		Controlledable *ConvertToControlledable(Object *obj);
		Movable *ConvertToMovable(Object *obj);
		Node *ConvertToNode(Object *obj);
		Actor *ConvertToActor(Object *obj);
		UIPicBox *ConvertToUIPicBox(Object *obj);
		UIFrame *ConvertToUIFrame(Object *obj);
		InterpCurveController *ConvertToInterpCurveController(Object *obj);
		EffectableController *ConvertToEffectableController(Object *obj);
		EffectModule *ConvertToEffectModule(Object *obj);
	};

#define PX2_CREATER Creater::GetSingleton()

}

#endif