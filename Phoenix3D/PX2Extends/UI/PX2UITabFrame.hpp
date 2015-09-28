// PX2UITabFrame.hpp

#ifndef PX2UITABFRAME_HPP
#define PX2UITABFRAME_HPP

#include "PX2UIFrame.hpp"

namespace PX2
{

	class UIAuiBlockFrame;
	
	class PX2_EXTENDS_ITEM UITabFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UITabFrame);

	public:
		UITabFrame();
		virtual ~UITabFrame();

		void SetTabWidth(float width);
		float GetTabWidth() const;

		void AddTab(const std::string &name, UIFrame *tabFrame);
		bool IsHasTab(const std::string &name);
		UIFrame *GetTab(const std::string &name);
		void RemoveTab(const std::string &name);
		void SetActiveTab(const std::string &name);

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		void _CalTabs();

		bool mIsTabsNeedReCal;

		UIFramePtr mFrame_TitleBar;
		std::map<std::string, UIFramePtr> mTabTitles;
		float mTabWidth;

		UIFramePtr mFrame_Content;
		std::map<std::string, UIFramePtr> mTabFrames;

		UIFramePtr mActiveTabFrame;

public_internal:
		void SetAuiBlockFrame(UIAuiBlockFrame *auiBlockFrame);
		UIAuiBlockFrame *GetAuiBlockFrame();

	protected:
		UIAuiBlockFrame *mAuiBlockFrame;
	};

#include "PX2UITabFrame.inl"
	PX2_REGISTER_STREAM(UITabFrame);
	typedef Pointer0<UITabFrame> UITabFramePtr;

}

#endif