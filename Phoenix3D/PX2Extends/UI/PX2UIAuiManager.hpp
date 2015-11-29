// PX2UIAuiManager.hpp

#ifndef PX2UIAUIMANAGER_HPP
#define PX2UIAUIMANAGER_HPP

#include "PX2Singleton_NeedNew.hpp"
#include "PX2UIAuiBlockFrame.hpp"
#include "PX2UIAuiFrame.hpp"
#include "PX2UIButton.hpp"

namespace PX2
{

	class UIAuiManager : public Singleton<UIAuiManager>
	{
	public:
		UIAuiManager();
		virtual ~UIAuiManager();

		std::string GenName(const std::string &tag);

		void SetAuiFrame(UIAuiFrame *frame);
		UIAuiFrame *GetAuiFrame();

		// frames collect
		// aui block
		void AddUIAuiBlockFrame(UIAuiBlockFrame *auiBlockFrame);
		void RemoveUIAuiBlockFrame(UIAuiBlockFrame *auiBlockframe);

		// tab frame
		void AddTabFrame(const std::string &name, UITabFrame *tab);
		void RemoveTabFrame(const std::string &name);
		void RemoveTabFrame(UITabFrame *tabFrame);
		UITabFrame *GetTabFrame(const std::string &name);

		// tab content frame
		void AddTabContentFrame(UIButton *tabBut, UIFrame *frame);

		// active
		void SetActiveTableFrame(UIButton *tabBut, UIFrame *tableFrame);
		UIFrame *GetActiveTableFrame();

	protected:
		static int msNameID;

		UIAuiFramePtr mUIAuiFrame;
		std::vector<UIAuiBlockFramePtr> mUIAuiBlockFrames;

		std::vector<UIAuiBlockFramePtr> mAuiBlockFrames;
		std::map<std::string, UITabFramePtr> mAuiTabFrames;

		std::map<UIButton *, UIFramePtr> mAuiContentFrames;

		UIButtonPtr mActiveTableBut;
		UIFramePtr mActiveTableContentFrame;
	};


#define PX2_UIAUIM UIAuiManager::GetSingleton()
#include "PX2UIAuiManager.inl"

}

#endif