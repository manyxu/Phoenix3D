// PX2UIAuiManager.hpp

#ifndef PX2UIAUIMANAGER_HPP
#define PX2UIAUIMANAGER_HPP

#include "PX2Singleton_NeedNew.hpp"
#include "PX2UIAuiBlockFrame.hpp"
#include "PX2UIAuiFrame.hpp"
#include "PX2UIButton.hpp"
#include "PX2RenderWindow.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIAuiManager : public Singleton<UIAuiManager>
	{
	public:
		UIAuiManager();
		virtual ~UIAuiManager();

		void Clear();

		std::string GenName(const std::string &tag);

		void AddAuiFrame(RenderWindow *rw, UIAuiFrame *frame);
		UIAuiFrame *GetAuiFrame(RenderWindow *rw);

		// tab frame
		void AddTabFrame(const std::string &name, UITabFrame *tab);
		void RemoveTabFrame(const std::string &name);
		void RemoveTabFrame(UITabFrame *tabFrame);
		UITabFrame *GetTabFrame(const std::string &name);

		// tab content frame
		void AddTabContentFrame(UIButton *tabBut, UIFrame *frame);
		void RemoveTabContentFrame(const std::string &tabName);

		// active
		void SetActiveTableFrame(const std::string &tabName);
		UIFrame *GetActiveTableFrame();

		void SetCaptureBlockFrame(UIAuiBlockFrame *blockFrame);
		UIAuiBlockFrame *GetCaptureBlockFrame();

	protected:
		static int msNameID;

		std::map<RenderWindow *, UIAuiFramePtr> mUIAuiFrames;

		std::map<std::string, UITabFramePtr> mAuiTabFrames;
		std::map<std::string, UIButtonPtr> mAuiTabButs;
		std::map<UIButton *, UIFramePtr> mAuiContentFrames;

		UIButtonPtr mActiveTableBut;
		UIFramePtr mActiveTableContentFrame;

		UIAuiBlockFramePtr mCaptureBlockFrame;
	};


#define PX2_UIAUIM UIAuiManager::GetSingleton()
#include "PX2UIAuiManager.inl"

}

#endif