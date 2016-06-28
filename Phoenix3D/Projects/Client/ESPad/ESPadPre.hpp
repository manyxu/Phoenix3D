// ESPadPre.hpp

#ifndef ESPADPRE_HPP
#define ESPADPRE_HPP

#include "PX2CorePre.hpp"

#if defined(ESPAD_DLL_EXPORT)
#define ESPAD_DLL_ITEM __declspec(dllexport)
#elif defined(ESPAD_DLL_IMPORT)
#define ESPAD_DLL_ITEM __declspec(dllimport)
#else
#define ESPAD_DLL_ITEM
#endif

const std::string PDP = "Data/ESPad/";

#endif