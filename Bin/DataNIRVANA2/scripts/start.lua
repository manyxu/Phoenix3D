-- start.lua

PX2_SC_LUA:CallFile("DataNIRVANA2/scripts/language.lua")
PX2_SC_LUA:CallFile("DataNIRVANA2/scripts/functions.lua")
PX2_SC_LUA:CallFile("DataNIRVANA2/scripts/main/menu.lua")
PX2_SC_LUA:CallFile("DataNIRVANA2/scripts/main/menufunction.lua")
PX2_SC_LUA:CallFile("DataNIRVANA2/scripts/project/project.lua")
PX2_SC_LUA:CallFile("DataNIRVANA2/scripts/blueprint/blueprint.lua")

function n_EditOnEvent(entType)
	n_project:onEvent(entType)
end

function naStart()
	n_AddLanguage()
	n_CreateMainMenu()
end