-- functions.lua

-- create objs
function n_Create_Geometry_Plane()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateMovable_Rectangle()
		mov.LocalTransform:SetTranslate(pickPos)
		node:AttachChild(mov)
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Geometry_Box()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateMovable_Box()
		mov.LocalTransform:SetTranslate(pickPos)
		node:AttachChild(mov)
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Geometry_Sphere()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateMovable_Sphere()
		mov.LocalTransform:SetTranslate(pickPos)
		node:AttachChild(mov)
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Node()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateNode()
		mov.LocalTransform:SetTranslate(pickPos)
		node:AttachChild(mov)
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_SkyBox()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end

	local selectedResDir = PX2_EDIT:GetSelectedResDir()
	
	if nil~=node and ""~=selectedResDir then
		local mov = PX2_CREATER:CreateSkyBox(selectedResDir)
		node:AttachChild(mov)
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Terrain()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local mov = PX2_CREATER:CreateTerrain()
		node:AttachChild(mov)
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

-- create effect
function n_Create_Particle()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateParticleEmitter()
		mov.LocalTransform:SetTranslate(pickPos)
		node:AttachChild(mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Billboard()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateBillboard()
		mov.LocalTransform:SetTranslate(pickPos)
		node:AttachChild(mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Beam()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateBeamEmitter()
		mov.LocalTransform:SetTranslate(pickPos)
		node:AttachChild(mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Ribbon()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateRibbonEmitter()
		mov.LocalTransform:SetTranslate(pickPos)
		node:AttachChild(mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Sound()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateSoundable()
		mov.LocalTransform:SetTranslate(pickPos)
		node:AttachChild(mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

-- create controller
function n_Create_Controller_ColorController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local ctrlable = Cast:ToControlledable(selectObj)
	if nil~=ctrlable then
		local ctrl = PX2_CREATER:CreateICC_Color()
		PX2_CREATER:AddObject(ctrlable, ctrl)
		ctrl:ResetPlay()
	else
		NirMan:MessageBox(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Controller_BrightnessController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local ctrlable = Cast:ToControlledable(selectObj)
	if nil~=ctrlable then
		local ctrl = PX2_CREATER:CreateICC_Brightness()
		PX2_CREATER:AddObject(ctrlable, ctrl)
		ctrl:ResetPlay()
	else
		NirMan:MessageBox(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Controller_AlphaController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local ctrlable = Cast:ToControlledable(selectObj)
	if nil~=ctrlable then
		local ctrl = PX2_CREATER:CreateICC_Alpha()
		PX2_CREATER:AddObject(ctrlable, ctrl)
		ctrl:ResetPlay()
	else
		NirMan:MessageBox(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Controller_UniformScaleController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local ctrlable = Cast:ToControlledable(selectObj)
	if nil~=ctrlable then
		local ctrl = PX2_CREATER:CreateICC_UniformScale()
		PX2_CREATER:AddObject(ctrlable, ctrl)
		ctrl:ResetPlay()
	else
		NirMan:MessageBox(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Controller_ScaleController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local ctrlable = Cast:ToControlledable(selectObj)
	if nil~=ctrlable then
		local ctrl = PX2_CREATER:CreateICC_Scale()
		PX2_CREATER:AddObject(ctrlable, ctrl)
		ctrl:ResetPlay()
	else
		NirMan:MessageBox(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Controller_RotateController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local ctrlable = Cast:ToControlledable(selectObj)
	if nil~=ctrlable then
		local ctrl = PX2_CREATER:CreateICC_Rotate()
		PX2_CREATER:AddObject(ctrlable, ctrl)
		ctrl:ResetPlay()
	else
		NirMan:MessageBox(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Controller_TranslateController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local ctrlable = Cast:ToControlledable(selectObj)
	if nil~=ctrlable then
		local ctrl = PX2_CREATER:CreateICC_Translate()
		PX2_CREATER:AddObject(ctrlable, ctrl)
		ctrl:ResetPlay()
	else
		NirMan:MessageBox(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

-- create ui
function n_Create_UIFrame()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetUI()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateUIFrame()
		mov:SetAnchorParamHor(pickPos:X(), pickPos:X())
		mov:SetAnchorParamVer(pickPos:Z(), pickPos:Z())
		node:AttachChild(mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_UIFPicBox()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetUI()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateUIFPicBox()
		mov:SetAnchorParamHor(pickPos:X(), pickPos:X())
		mov:SetAnchorParamVer(pickPos:Z(), pickPos:Z())
		node:AttachChild(mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_UIFText()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetUI()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateUIFText()
		mov:SetAnchorParamHor(pickPos:X(), pickPos:X())
		mov:SetAnchorParamVer(pickPos:Z(), pickPos:Z())
		node:AttachChild(mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end

end

function n_Create_UIButton()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetUI()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateUIButton()
		mov:SetAnchorParamHor(pickPos:X(), pickPos:X())
		mov:SetAnchorParamVer(pickPos:Z(), pickPos:Z())
		node:AttachChild(mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_UICheckButton()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetUI()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateUICheckButton()
		mov:SetAnchorParamHor(pickPos:X(), pickPos:X())
		mov:SetAnchorParamVer(pickPos:Z(), pickPos:Z())
		node:AttachChild(mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_UIComboBox()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetUI()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateUIComboBox()
		mov:SetAnchorParamHor(pickPos:X(), pickPos:X())
		mov:SetAnchorParamVer(pickPos:Z(), pickPos:Z())
		node:AttachChild(mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_UIEditBox()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetUI()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateUIEditBox()
		mov:SetAnchorParamHor(pickPos:X(), pickPos:X())
		mov:SetAnchorParamVer(pickPos:Z(), pickPos:Z())
		node:AttachChild(mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_CreateScriptControllerFromRes()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local controlledable = Cast:ToControlledable(selectObj)	
		
	local selResData = PX2_EDIT:GetSelectedResource()
	
	if nil~=controlledable then
	
		local controller = PX2_CREATER:CreateScriptController_FromRes(selResData.ResPathname)
		if nil~= controller then		
			controlledable:AttachController(controller)
		end
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_ImportSelectRes()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)	
		
	local selResData = PX2_EDIT:GetSelectedResource()
	
	if nil==node then
		node = PX2_PROJ:GetUI()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		
		local controller = PX2_CREATER:CreateScriptController_FromRes(selResData.ResPathname)
		if nil~= controller then		
			node:AttachController(controller)
			controller:ResetPlay()		
		end
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_ImportSelect()

end

-- other functions
function n_Copy()
	PX2_EDIT:SetCopyObject(PX2_SELECTM_E:GetFirstObject())
end

function n_Paste()
	PX2_EDIT:PasteCopyedObject()
end

function n_DeleteSelection()
	PX2_EDIT:DeleteSelection()
end

-- anim play
function n_AnimResetPlay()
	PX2_EDIT:AnimResetPlay()
end

function n_AnimPlayStop()
	PX2_EDIT:AnimPlayStop()
end

function n_AnimPlay()
	PX2_EDIT:AnimPlay()
end

function n_AnimStop()
	PX2_EDIT:AnimStop()
end

function n_AnimReset()
	PX2_EDIT:AnimReset()
end

function n_MakeCurve()
	PX2_EDIT:MakeSelectTimeLineEdit();
end

-- time line
function n_TimeLine_Delete()
	PX2_EDIT:TimeLine_UIGroup_Delete();
end

function n_TimeLine_DeleteAll()
	PX2_EDIT:TimeLine_UIGroup_DeleteAll();
end

-- blue print
function n_CreateBPPackage()
	PX2_EDIT:CreateBPPackage()
end

function n_CreateBPFile()
	PX2_EDIT:CreateBPFile()
end

function n_CreateBPModule(className, funName)
	PX2_EDIT:CreateBPModule(className, funName)
end

function n_CreateBPEvent(spaceName, eventName)
	PX2_EDIT:CreateBPEvent(spaceName, eventName)
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