// PX2LP_Pre.hpp

#ifndef PX2LP_PRE_HPP
#define PX2LP_PRE_HPP

#include "PX2SimulationPre.hpp"

#if defined(PX2LP_DLL_EXPORT)
#define PX2LP_DLL_ITEM __declspec(dllexport)
#elif defined(PX2LP_DLL_IMPORT)
#define PX2LP_DLL_ITEM __declspec(dllimport)
#else
#define PX2LP_DLL_ITEM
#endif

const std::string PDP = "DataLauncher/";

#endif