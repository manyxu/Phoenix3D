-- start.lua

PX2_SM:CallFile("DataNIRVANA2/scripts/language.lua")
PX2_SM:CallFile("DataNIRVANA2/scripts/menu.lua")
PX2_SM:CallFile("DataNIRVANA2/scripts/menufunction.lua")

local function naStart()
	naAddLanguage()
	naCreateMainMenu()
end
naStart()