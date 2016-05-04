-- blueprint.lua

function n_CreateBPPackage()
	PX2_EDIT:CreateBPPackage()
end

function n_CreateBPFile()
	PX2_EDIT:CreateBPFile()
end

function n_CreateBPModule(className, funName)
	PX2_EDIT:CreateBPModule(className, funName)
end

function n_CreateBPEvent(eventName)
	PX2_EDIT:CreateBPEvent(eventName)
end

function n_CreateBPOption(optionName)
	PX2_EDIT:CreateBPOption(optionName)
end

function n_CreateBPOperator(operatorName)
	PX2_EDIT:CreateBPOperator(operatorName)
end

function n_CreateBPParam(paramName)
	PX2_EDIT:CreateBPParam(paramName)
end

function n_BPFileCompile()
	PX2_EDIT:CompileBP()
end

function n_DisconnectParam()	
	PX2_EDIT:DisconnectParam()
end