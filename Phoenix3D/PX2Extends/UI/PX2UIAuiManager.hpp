// PX2UIAuiManager.hpp

#ifndef PX2UIAUIMANAGER_HPP
#define PX2UIAUIMANAGER_HPP

#include "PX2Singleton_NeedNew.hpp"
#include "PX2UIAuiBlockFrame.hpp"
#include "PX2UIAuiFrame.hpp"

namespace PX2
{

	class UIAuiManager : public Singleton<UIAuiManager>
	{
	public:
		UIAuiManager();
		virtual ~UIAuiManager();

		void SetAuiFrame(UIAuiFrame *frame);
		UIAuiFrame *GetAuiFrame();

		void SetActiveTableName(const std::string &tabName);
		const std::string &GetActiveTableName() const;

		void AddTabItemFrame(const std::string &name, UIFrame *frame);

	protected:
		UIAuiFramePtr mUIAuiFrame;
		std::map<std::string, UIFramePtr> mAuiItemFrames;

		std::string mActiveTableName;
	};


#define PX2_UIAUIM UIAuiManager::GetSingleton()
#include "PX2UIAuiManager.inl"

}

#endif