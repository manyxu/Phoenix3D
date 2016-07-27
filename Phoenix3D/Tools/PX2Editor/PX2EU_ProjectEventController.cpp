//PX2EU_ProjectEventController.cpp

#include "PX2EU_ProjectEventController.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2Scene.hpp"
#include "PX2Canvas.hpp"
#include "PX2UIFrame.hpp"
#include "PX2EU_CanvasStage.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
EU_ProjectEventController::EU_ProjectEventController()
{
	ComeInEventWorld();
}
//----------------------------------------------------------------------------
EU_ProjectEventController::~EU_ProjectEventController()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void EU_ProjectEventController::OnEvent(Event *event)
{
	Canvas *canvas = DynamicCast<Canvas>(GetControlledable());
	if (canvas)
	{
		const std::string &name = canvas->GetName();

		if ("SceneCanvas" == name)
		{
			if (ProjectES::IsEqual(event, ProjectES::NewScene))
			{
				Scene *scene = event->GetData<Scene*>();

				canvas->RemoveAllCameras();
				std::vector<CameraPtr> &cams = scene->GetCameras();
				for (int i = 0; i < (int)cams.size(); i++)
				{
					canvas->AddCamera(cams[i]);
				}

				canvas->AttachChild(scene);

				EU_CanvasStage::GetSingleton().Enable(true);
			}
			else if (ProjectES::IsEqual(event, ProjectES::CloseScene))
			{
				Scene *scene = event->GetData<Scene*>();

				canvas->DetachChild(scene);
				canvas->SetRenderNode(0);
				canvas->RemoveAllCameras();
			}
		}
		else if ("EU_CanvasStageSimu" == name)
		{
			if (ProjectES::IsEqual(event, ProjectES::NewScene))
			{
				Scene *scene = event->GetData<Scene*>();

				canvas->RemoveAllCameras();
				std::vector<CameraPtr> &cams = scene->GetCameras();
				for (int i = 0; i < (int)cams.size(); i++)
				{
					canvas->AddCamera(cams[i]);
				}

				canvas->SetRenderNode(scene);

				EU_CanvasStage::GetSingleton().Enable(true);
			}
			else if (ProjectES::IsEqual(event, ProjectES::CloseScene))
			{
				Scene *scene = event->GetData<Scene*>();

				canvas->DetachChild(scene);
				canvas->SetRenderNode(0);
				canvas->RemoveAllCameras();

				EU_CanvasStage::GetSingleton().Enable(false);
			}
		}
		else if ("ProjectUICanvas" == name)
		{
			if (ProjectES::IsEqual(event, ProjectES::NewUI))
			{
				UIFrame *uiFrame = event->GetData<UIFrame*>();

				canvas->AttachChild(uiFrame);
			}
			else if (ProjectES::IsEqual(event, ProjectES::CloseUI))
			{
				UIFrame *uiFrame = event->GetData<UIFrame*>();

				canvas->DetachChild(uiFrame);
				canvas->SetRenderNode(0);
			}
		}
	}
}
//----------------------------------------------------------------------------