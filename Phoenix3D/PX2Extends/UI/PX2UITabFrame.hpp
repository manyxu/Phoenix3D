// PX2UITabFrame.hpp

#ifndef PX2UITABFRAME_HPP
#define PX2UITABFRAME_HPP

#include "PX2UIFrame.hpp"
#include "PX2UIButton.hpp"

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

		void SetSkinAui(bool isAui);
		bool IsSkinAui() const;

		enum LayoutPosType
		{
			LPT_TOP,
			LPT_BOTTOM,
			LPT_LEFT,
			LPT_RIGHT,
			LPT_MAX_TYPE
		};
		void SetLayoutPos(LayoutPosType posType);
		LayoutPosType GetLayoutPos() const;

		void SetTabWidth(float width);
		float GetTabWidth() const;

		void SetTabHeight(float height);
		float GetTabHeight() const;

		void AddTab(const std::string &name, UIFrame *tabContentFrame);
		bool IsHasTab(const std::string &name);
		UIFrame *GetTab(const std::string &name);
		void RemoveTab(const std::string &name);
		void SetActiveTab(const std::string &name);
		UIButton *GetTabButton(const std::string &name);

	protected:
		void TabCallback(UIFrame *frame, UICallType type);
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		void _CalTabs();

		bool mIsSkinAui;
		LayoutPosType mLayoutPosType;

		bool mIsTabsNeedReCal;

		UIFramePtr mFrame_TitleBar;
		std::vector<UIButtonPtr> mTabButs;
		float mTabWidth;
		float mTabHeight;

		UIFramePtr mFrame_Content;
		std::map<std::string, UIFramePtr> mTabContentFrames;

		UIButtonPtr mActiveTabBut;
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