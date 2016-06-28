// PX2Plugin.hpp

#ifndef PX2PLUGIN_HPP
#define PX2PLUGIN_HPP

#include "PX2UnityPre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Plugin
	{
	public:
		Plugin() {}
		virtual ~Plugin() {}

		virtual std::string GetName() = 0;

		virtual void OnInstall() = 0;
		virtual void OnUninstall() = 0;
		virtual void OnUpdate() = 0;
	};

}

#endif