// PX2EditorToAngelScript.cpp

#include <PX2EditorToAngelScript.hpp>
#include "angelscript.h"
using namespace std;

void FakeAddRef(void* ptr) {}
void FakeReleaseRef(void* ptr) {}

template<class A, class B>
B* RefCast(A* a)
{
	if( !a )
		return 0;
	B* b = dynamic_cast<B*>(a);
	return b;
}

int PX2EditorToAngelScript(asIScriptEngine* asEngine)
{
	int r = 0;
	// Enum: SelectMode
	r = asEngine->RegisterEnum("SelectMode");
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("SelectMode", "SM_SINGLE", SelectMode::SM_SINGLE);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("SelectMode", "SM_MULTI", SelectMode::SM_MULTI);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("SelectMode", "SM_MAX_TYPE", SelectMode::SM_MAX_TYPE);
	assert(r >= 0);
	// Enum: RenderViewType
	r = asEngine->RegisterEnum("RenderViewType");
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("RenderViewType", "RVT_SCENEUI", RenderViewType::RVT_SCENEUI);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("RenderViewType", "RVT_LOGIC", RenderViewType::RVT_LOGIC);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("RenderViewType", "RVT_RES", RenderViewType::RVT_RES);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("RenderViewType", "RVT_PREVIEW", RenderViewType::RVT_PREVIEW);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("RenderViewType", "RVT_TIMELINE", RenderViewType::RVT_TIMELINE);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("RenderViewType", "PVT_PROJECT", RenderViewType::PVT_PROJECT);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("RenderViewType", "RVT_MAX_TYPE", RenderViewType::RVT_MAX_TYPE);
	assert(r >= 0);
	r = asEngine->RegisterObjectType("Edit", 0,  asOBJ_REF | asOBJ_NOCOUNT);
	assert(r >= 0);
	r = asEngine->RegisterObjectType("SelectResData", 0,  asOBJ_REF | asOBJ_NOCOUNT);
	assert(r >= 0);
	r = asEngine->RegisterObjectType("TimeLineEdit", 0,  asOBJ_REF | asOBJ_NOCOUNT);
	assert(r >= 0);
	r = asEngine->RegisterObjectType("EU_Manager", 0,  asOBJ_REF | asOBJ_NOCOUNT);
	assert(r >= 0);
	// Class: Edit
	// Enum: EditType
	r = asEngine->RegisterEnum("EditType");
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("EditType", "ET_SCENE", Edit::EditType::ET_SCENE);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("EditType", "ET_TERRAIN", Edit::EditType::ET_TERRAIN);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("EditType", "ET_UI", Edit::EditType::ET_UI);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("EditType", "ET_SIMULATE", Edit::EditType::ET_SIMULATE);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("EditType", "ET_PLAY", Edit::EditType::ET_PLAY);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("EditType", "ET_MAXTYPE", Edit::EditType::ET_MAXTYPE);
	assert(r >= 0);
	// Enum: EditAxisMode
	r = asEngine->RegisterEnum("EditAxisMode");
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("EditAxisMode", "EAM_WORLD", Edit::EditAxisMode::EAM_WORLD);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("EditAxisMode", "EAM_LOCAL", Edit::EditAxisMode::EAM_LOCAL);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("EditAxisMode", "EAM_PARENT", Edit::EditAxisMode::EAM_PARENT);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("EditAxisMode", "EMA_MAX_MODE", Edit::EditAxisMode::EMA_MAX_MODE);
	assert(r >= 0);
	// Enum: EditMode
	r = asEngine->RegisterEnum("EditMode");
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("EditMode", "EM_NONE", Edit::EditMode::EM_NONE);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("EditMode", "EM_SELECT", Edit::EditMode::EM_SELECT);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("EditMode", "EM_TRANSLATE", Edit::EditMode::EM_TRANSLATE);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("EditMode", "EM_ROTATE", Edit::EditMode::EM_ROTATE);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("EditMode", "EM_SCALE", Edit::EditMode::EM_SCALE);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("EditMode", "EM_MAX_MODE", Edit::EditMode::EM_MAX_MODE);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("Edit", "bool Initlize()", asMETHODPR(Edit, Initlize, (), bool), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("Edit", "bool Terminate()", asMETHODPR(Edit, Terminate, (), bool), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("Edit", "void Reset()", asMETHODPR(Edit, Reset, (), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("Edit", "void SetEditType(EditType type)", asMETHODPR(Edit, SetEditType, (Edit::EditType), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("Edit", "EditType GetEditType() const", asMETHODPR(Edit, GetEditType, () const, Edit::EditType), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("Edit", "bool CanDoEdit() const", asMETHODPR(Edit, CanDoEdit, () const, bool), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("Edit", "void SetEditAxisMode(EditAxisMode mode)", asMETHODPR(Edit, SetEditAxisMode, (Edit::EditAxisMode), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("Edit", "EditAxisMode GetEdtiAxisMode() const", asMETHODPR(Edit, GetEdtiAxisMode, () const, Edit::EditAxisMode), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("Edit", "void SetEditMode(EditMode mode)", asMETHODPR(Edit, SetEditMode, (Edit::EditMode), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("Edit", "EditMode GetEditMode() const", asMETHODPR(Edit, GetEditMode, () const, Edit::EditMode), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("Edit", "void SetPickPos(const APoint & pos)", asMETHODPR(Edit, SetPickPos, (const APoint & ), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("Edit", "APoint GetPickPos() const", asMETHODPR(Edit, GetPickPos, () const, APoint), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("Edit", "bool DeleteSelection()", asMETHODPR(Edit, DeleteSelection, (), bool), asCALL_THISCALL);
	assert(r >= 0);
	// Class: SelectResData
	// Enum: SelectResType
	r = asEngine->RegisterEnum("SelectResType");
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("SelectResType", "RT_NORMAL", SelectResData::SelectResType::RT_NORMAL);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("SelectResType", "RT_TEXPACKELEMENT", SelectResData::SelectResType::RT_TEXPACKELEMENT);
	assert(r >= 0);
	r = asEngine->RegisterEnumValue("SelectResType", "RT_MAX_TYPE", SelectResData::SelectResType::RT_MAX_TYPE);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("SelectResData", "SelectResType GetSelectResType() const", asMETHODPR(SelectResData, GetSelectResType, () const, SelectResData::SelectResType), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("SelectResData", "bool IsTheObjectTex() const", asMETHODPR(SelectResData, IsTheObjectTex, () const, bool), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("SelectResData", "Sizef GetTheObjectTexSize() const", asMETHODPR(SelectResData, GetTheObjectTexSize, () const, Sizef), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectProperty("SelectResData", "string ResPathname", asOFFSET(SelectResData, ResPathname));
	assert(r >= 0);
	r = asEngine->RegisterObjectProperty("SelectResData", "string EleName", asOFFSET(SelectResData, EleName));
	assert(r >= 0);
	// Class: TimeLineEdit
	// Class: EU_Manager
	r = asEngine->RegisterObjectMethod("EU_Manager", "void Menu_Main_AddMainItem(const string & name, const string & title)", asMETHODPR(EU_Manager, Menu_Main_AddMainItem, (const std::string & , const std::string & ), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("EU_Manager", "void Menu_Main_AddSubItem(const string & parentName, const string & name, const string & title)", asMETHODPR(EU_Manager, Menu_Main_AddSubItem, (const std::string & , const std::string & , const std::string & ), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("EU_Manager", "void Menu_Main_AddItem(const string & parentName, const string & name, const string & title, const string & script, const string & tag)", asMETHODPR(EU_Manager, Menu_Main_AddItem, (const std::string & , const std::string & , const std::string & , const std::string & , const std::string & ), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("EU_Manager", "void Menu_Main_AddItemSeparater(const string & parentName)", asMETHODPR(EU_Manager, Menu_Main_AddItemSeparater, (const std::string & ), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("EU_Manager", "void Menu_Edit_Begin(const string & whe, const string & name)", asMETHODPR(EU_Manager, Menu_Edit_Begin, (const std::string & , const std::string & ), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("EU_Manager", "void Menu_Edit_AddSubItem(const string & whe, const string & parentName, const string & name, const string & title)", asMETHODPR(EU_Manager, Menu_Edit_AddSubItem, (const std::string & , const std::string & , const std::string & , const std::string & ), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("EU_Manager", "void Menu_Edit_AddItem(const string & whe, const string & parentName, const string & name, const string & title, const string & script, const string & tag)", asMETHODPR(EU_Manager, Menu_Edit_AddItem, (const std::string & , const std::string & , const std::string & , const std::string & , const std::string & , const std::string & ), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("EU_Manager", "void Menu_Edit_AddItemSeparater(const string & whe, const string & parentName)", asMETHODPR(EU_Manager, Menu_Edit_AddItemSeparater, (const std::string & , const std::string & ), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("EU_Manager", "void Menu_Edit_EndPopUp(const string & whe, const APoint & pos)", asMETHODPR(EU_Manager, Menu_Edit_EndPopUp, (const std::string & , const APoint & ), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("EU_Manager", "void NewProject()", asMETHODPR(EU_Manager, NewProject, (), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("EU_Manager", "void OpenProject()", asMETHODPR(EU_Manager, OpenProject, (), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("EU_Manager", "void SaveProject()", asMETHODPR(EU_Manager, SaveProject, (), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("EU_Manager", "void CloseProject()", asMETHODPR(EU_Manager, CloseProject, (), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("EU_Manager", "void NewScene()", asMETHODPR(EU_Manager, NewScene, (), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("EU_Manager", "void OpenScene()", asMETHODPR(EU_Manager, OpenScene, (), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("EU_Manager", "void SaveScene()", asMETHODPR(EU_Manager, SaveScene, (), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("EU_Manager", "void SaveSceneAs()", asMETHODPR(EU_Manager, SaveSceneAs, (), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("EU_Manager", "void CloseScene()", asMETHODPR(EU_Manager, CloseScene, (), void), asCALL_THISCALL);
	assert(r >= 0);
	r = asEngine->RegisterObjectMethod("EU_Manager", "void Exit()", asMETHODPR(EU_Manager, Exit, (), void), asCALL_THISCALL);
	assert(r >= 0);

	return 0;
}