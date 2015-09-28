// PX2UIPanel.hpp

#ifndef PX2UIPANEL_HPP
#define PX2UIPANEL_HPP

#include "PX2UIFrame.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM UIPanel : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(UIPanel);

	public:
		enum MenuType
		{
			MT_HOR,
			MT_VER,
			MT_MAX_TYPE
		};

		UIPanel(MenuType mt);
		virtual ~UIPanel();

		MenuType GetMenuType() const;

	protected:
		MenuType mMenuType;
	};

	PX2_REGISTER_STREAM(UIPanel);
	typedef Pointer0<UIPanel> UIPanelPtr;

}

#endif