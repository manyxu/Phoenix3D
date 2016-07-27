// PX2EU_ProjectItem.cpp

#include "PX2EU_ProjectItem.hpp"
#include "PX2Project.hpp"
#include "PX2EffectableController.hpp"
#include "PX2EU_ProjectTree.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIItem, EU_ProjectItem);
PX2_IMPLEMENT_STREAM(EU_ProjectItem);
PX2_IMPLEMENT_FACTORY(EU_ProjectItem);
PX2_IMPLEMENT_DEFAULT_NAMES(UIItem, EU_ProjectItem);

//----------------------------------------------------------------------------
EU_ProjectItem::EU_ProjectItem():
mShowType(ST_GENERAL),
mEU_ProjectTree(0)
{
}
//----------------------------------------------------------------------------
EU_ProjectItem::~EU_ProjectItem()
{
}
//----------------------------------------------------------------------------
void EU_ProjectItem::SetShowType(ShowType st)
{
	mShowType = st;

	Project *proj = DynamicCast<Project>(mObject);
	Scene *scene = DynamicCast<Scene>(mObject);
	if (proj || scene) return;

	Movable *mov = DynamicCast<Movable>(mObject);
	Node *node = DynamicCast<Node>(mObject);
	Renderable *renderable = DynamicCast<Renderable>(mObject);
	EffectableController *effectableCtrl = DynamicCast<EffectableController>(mObject);
	Material *mtl = DynamicCast<Material>(mObject);
	MaterialTechnique *mtlTechnique = DynamicCast<MaterialTechnique>(mObject);
	MaterialPass *mtlPass = DynamicCast<MaterialPass>(mObject);

	RemoveAllChildItems();

	bool addCtrl = false;
	bool addModule = false;
	bool addNode = false;
	bool addMaterial = false;

	if (ST_GENERAL == mShowType)
	{
		/*_*/
	}
	else if (ST_CHILDREN == mShowType)
	{
		if (!proj && !scene && node)
		{
			addNode = true;
		}
	}
	else if (ST_CONTROLS == mShowType)
	{
		if (!scene && !proj && mov)
		{
			addCtrl = true;
		}

		if (!proj && !scene && node)
		{
			addNode = true;
		}

		if (effectableCtrl)
		{
			addModule = true;
		}
	}
	else if (ST_MATERIAL == mShowType)
	{
		if (!proj && !scene && mov)
		{
			addCtrl = true;
		}

		if (!proj && !scene && node)
		{
			addNode = true;
		}

		if (renderable)
		{
			addMaterial = true;
		}
	}
	else if (ST_DETAIL == mShowType)
	{
		if (!scene && !proj && mov)
		{
			addCtrl = true;
		}

		if (!scene && !proj && node)
		{
			addNode = true;
		}

		if (renderable)
		{
			addMaterial = true;
		}
	}

	if (addCtrl)
	{
		int numCtrls = mov->GetNumControllers();
		for (int i = 0; i < numCtrls; i++)
		{
			Controller *ctrl = mov->GetController(i);

			const std::string &name = ctrl->GetName();

			mEU_ProjectTree->AddItem(this, name, name, ctrl);
		}
	}

	if (addMaterial)
	{
		MaterialInstance *mtlInst = renderable->GetMaterialInstance();
		Material *mtl = mtlInst->GetMaterial();

		const std::string &name = mtl->GetName();
		
		mEU_ProjectTree->AddItem(this, name, name, mtl);
	}
	if (mtl)
	{
		int numTechniques = mtl->GetNumTechniques();
		for (int i = 0; i < numTechniques; i++)
		{
			MaterialTechnique *mtlTechnique = mtl->GetTechnique(i);
			
			const std::string &name = mtlTechnique->GetName();

			mEU_ProjectTree->AddItem(this, name, name, mtlTechnique);
		}
	}
	if (mtlTechnique)
	{
		int numPasses = mtlTechnique->GetNumPasses();
		for (int i = 0; i < numPasses; i++)
		{
			MaterialPass *mtlPass = mtlTechnique->GetPass(i);
			
			const std::string &name = mtlPass->GetName();

			mEU_ProjectTree->AddItem(this, name, name, mtlPass);
		}
	}
	if (mtlPass)
	{
		VertexShader *vs = mtlPass->GetVertexShader();
		PixelShader *ps = mtlPass->GetPixelShader();
		mEU_ProjectTree->AddItem(this, vs->GetName(), vs->GetName(), vs);
		mEU_ProjectTree->AddItem(this, ps->GetName(), ps->GetName(), ps);

		AlphaProperty *ap = mtlPass->GetAlphaProperty();
		CullProperty *cp = mtlPass->GetCullProperty();
		DepthProperty *dp = mtlPass->GetDepthProperty();
		OffsetProperty *op = mtlPass->GetOffsetProperty();
		WireProperty *wp = mtlPass->GetWireProperty();

		mEU_ProjectTree->AddItem(this, ap->GetName(), ap->GetName(), ap);
		mEU_ProjectTree->AddItem(this, cp->GetName(), cp->GetName(), cp);
		mEU_ProjectTree->AddItem(this, dp->GetName(), dp->GetName(), dp);
		mEU_ProjectTree->AddItem(this, op->GetName(), op->GetName(), op);
		mEU_ProjectTree->AddItem(this, wp->GetName(), wp->GetName(), wp);

	}

	if (addModule)
	{
		int numModules = effectableCtrl->GetNumModules();
		for (int i = 0; i < numModules; i++)
		{
			EffectModule *module = effectableCtrl->GetModule(i);
			const std::string &name = module->GetName();

			mEU_ProjectTree->AddItem(this, name, name, mtlPass);
		}
	}

	if (addNode)
	{
		int numChildren = node->GetNumChildren();
		for (int i = 0; i < numChildren; i++)
		{
			Movable *child = node->GetChild(i);
			if (child)
			{
				const std::string &name = child->GetName();

				mEU_ProjectTree->AddItem(this, name, name, child);
			}
		}
	}
}
//----------------------------------------------------------------------------
ShowType EU_ProjectItem::GetShowType() const
{
	return mShowType;
}
//----------------------------------------------------------------------------
void EU_ProjectItem::SetEU_ProjectTree(EU_ProjectTree *tree)
{
	mEU_ProjectTree = tree;
}
//----------------------------------------------------------------------------
EU_ProjectTree *EU_ProjectItem::GetEU_ProjectTree()
{
	return mEU_ProjectTree;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_ProjectItem::EU_ProjectItem(LoadConstructor value) :
UIItem(value)
{
}
//----------------------------------------------------------------------------
void EU_ProjectItem::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIItem::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_ProjectItem, source);
}
//----------------------------------------------------------------------------
void EU_ProjectItem::Link(InStream& source)
{
	UIItem::Link(source);

	if (mIPTCtrl)
		source.ResolveLink(mIPTCtrl);
}
//----------------------------------------------------------------------------
void EU_ProjectItem::PostLink()
{
	UIItem::PostLink();
}
//----------------------------------------------------------------------------
bool EU_ProjectItem::Register(OutStream& target) const
{
	if (UIItem::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_ProjectItem::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIItem::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIItem, target);
}
//----------------------------------------------------------------------------
int EU_ProjectItem::GetStreamingSize(Stream &stream) const
{
	int size = UIItem::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------