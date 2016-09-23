-- BPCreatePanel.lua

BPCreatePanel = class(nil,
{
	_name = "BPCreatePanel",
});

function BPCreatePanel:OnAttached()
	PX2_LOGGER:LogInfo("script_lua", "BPCreatePanel:OnAttached")
end

function BPCreatePanel:OnInitUpdate()
	PX2_LOGGER:LogInfo("script_lua", "BPCreatePanel:OnInitUpdate")
	
	if nil~= self then	
		PX2_LOGGER:LogInfo("script_lua", "self is not nil")	
		
		PX2_LOGGER:LogInfo("script_lua", type(self))		

	else
		PX2_LOGGER:LogInfo("script_lua", "self is nil")	
	end
end