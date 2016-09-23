-- start.lua

function start() 
	PX2_SC_LUA:CallFile("Data/ScriptTest/scripts/lua/ActorCtrl.lua")
	
	local objSphere = PX2_PROJ:GetScene():GetObjectByName("Sphere");
	objSphere:RegistToScriptSystemAll()
	
	local ctrlSphere = ActorCtrl:New({val = 1000, _name="Sphere"})
	ctrlSphere._val = 1000
	objSphere:AttachController(Cast:ToSC(ctrlSphere.__object))	
	
	local objBox = PX2_PROJ:GetScene():GetObjectByName("Box");
	objBox:RegistToScriptSystemAll()
	
	local ctrlBox = ActorCtrl:New({val = 1001, _name="Box"})
	ctrlBox._val = 1001	
	objBox:AttachController(Cast:ToSC(ctrlBox.__object))
	
	local effect = PX2_PROJ:GetScene():GetObjectByName("Effect")
	effect:RegistToScriptSystemAll()
	local lsc = effect:GetObjectByName("LuaScriptController")
	
	for k, v in pairs(gScriptTable) do
		--PX2_LOGGER:LogInfo("script_lua", ""..k)
	end
	
	local scTable = gScriptTable[lsc:SelfP()]
	scTable._val = 1002
	
	--lsc:SetVal("_val", 1002)
	--lsc:GetSelf():SetInteger("_val", 1002)
end