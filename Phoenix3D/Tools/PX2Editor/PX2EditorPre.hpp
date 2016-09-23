// PX2EditorPre.hpp

#ifndef PX2EDITORPRE_HPP
#define PX2EDITORPRE_HPP

#include "PX2CorePre.hpp"

#if defined(PX2_EDITOR_DLL_EXPORT)
#define PX2_EDITOR_ITEM __declspec(dllexport)
#elif defined(PX2_EDITOR_DLL_IMPORT)
#define PX2_EDITOR_ITEM __declspec(dllimport)
#else
#define PX2_EDITOR_ITEM
#endif

#endif