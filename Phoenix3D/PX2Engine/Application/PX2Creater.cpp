// PX2Creater.cpp

#include "PX2Creater.hpp"
#include "PX2Texture2D.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2StandardMesh.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2ModelController.hpp"
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
Node *Creater::CreateNode_Box()
{
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

	meshNode->SetReceiveShadow(false);

	return meshNode;
}
//----------------------------------------------------------------------------
Node *Creater::CreateNode_Model(const std::string &filename)
{
	Node *node = new0 Node();

	ModelController *mc = new0 ModelController();
	mc->SetMovableFilename(filename);
	node->AttachController(mc);

	return node;
}
//----------------------------------------------------------------------------