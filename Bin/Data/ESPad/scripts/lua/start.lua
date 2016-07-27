-- start.lua

function start()
	PX2_LOGGER:LogInfo("script_lua", "project_start")

	local scene = PX2_PROJ:GetScene()
	if (nil == scene) then
		scene = Scene:New()
		PX2_PROJ:SetScene(scene)
	end

	local nodeBox = PX2_CREATER:CreateNode_Box()
	scene:AttachChild(Cast:ToMovable(nodeBox))
	nodeBox.LocalTransform:SetUniformScale(0.2)
	
	local rotCtrl = InterpCurveRotateController:New();
	nodeBox:AttachController(rotCtrl);
	rotCtrl:AddPoint(5.0, Float3(0.0, 0.0, 3.1415926*2.0), ICM_LINEAR);
	rotCtrl.MaxTime = 5.0
	rotCtrl.Repeat = Controller.RT_WRAP
	rotCtrl:Play()
	
	local luaCtrl = PX2_SC_LUA:CreateScriptController("Data/ESPad/scripts/lua/actorctrl.lua", "Player");
	nodeBox:AttachController(luaCtrl);	
end