// PX2ExtendsPre.hpp

#ifndef PX2EXTENDSPRE_HPP
#define PX2EXTENDSPRE_HPP

#include "PX2CorePre.hpp"

#if defined(PX2_EXTENDS_DLL_EXPORT)
// For the DLL library.
#define PX2_EXTENDS_ITEM __declspec(dllexport)
#elif defined(PX2_EXTENDS_DLL_IMPORT)
// For a client of the DLL library.
#define PX2_EXTENDS_ITEM __declspec(dllimport)
#else
// For the static library and for Apple/Linux.
#define PX2_EXTENDS_ITEM
#endif

#endif