// PX2UISkinManager.hpp

#ifndef PX2UISKINMANAGER_HPP
#define PX2UISKINMANAGER_HPP

#include "PX2UIPre.hpp"
#include "PX2Singleton_NeedNew.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM UISkinManager : public Singleton<UISkinManager>
	{
	public:
		UISkinManager();
		virtual ~UISkinManager();

		// aui
	public:
		float Size_Splitter;
		Float3 Color_Splitter;
		Float3 Color_Splitter_Draging;
	};

#define PX2_UISM UISkinManager::GetSingleton()

}

#endif