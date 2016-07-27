-- functions.lua

-- create objs
function n_Create_Geometry_Plane()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickPos()
		local mov = PX2_CREATER:CreateRectangle(node, pickPos, false)
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Geometry_Box()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickPos()
		local mov = PX2_CREATER:CreateNode_Box()
		node:AttachChild(mov)
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Geometry_Sphere()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickPos()
		local mov = PX2_CREATER:CreateNode_Box()
		node:AttachChild(mov)
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Effect_Billboard()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickPos()
		local mov = PX2_CREATER:CreateSphere(node, pickPos, false)
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Effect_Particle()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickPos()
		local mov = PX2_CREATER:CreateParticleEmitter(node, pickPos, false)
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Effect_Beam()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickPos()
		local mov = PX2_CREATER:CreateBeamEmitter(node, pickPos, false)
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Effect_Ribbon()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickPos()
		local mov = PX2_CREATER:CreateRibbonEmitter(node, pickPos, false)
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Effect_Sound()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickPos()
		local mov = PX2_CREATER:CreateSoundable(node, pickPos, false)
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Effect_Node()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickPos()
		local mov = PX2_CREATER:CreateNode(node, pickPos, false)
	else
		PX2EU_MAN:PlayTip(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Controller_ColorController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local mov = Cast:ToMovable(selectObj)
	if nil~=mov then
		PX2_CREATER:CreateICC_Color(mov)
	else
		NirMan:MessageBox(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Controller_BrightnessController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local mov = Cast:ToMovable(selectObj)
	if nil~=mov then
		PX2_CREATER:CreateICC_Brightness(mov)
	else
		NirMan:MessageBox(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Controller_AlphaController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local mov = Cast:ToMovable(selectObj)
	if nil~=mov then
		PX2_CREATER:CreateICC_Alpha(mov)
	else
		NirMan:MessageBox(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Controller_UniformScaleController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local mov = Cast:ToMovable(selectObj)
	if nil~=mov then
		PX2_CREATER:CreateICC_UniformScale(mov)
	else
		NirMan:MessageBox(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Controller_ScaleController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local mov = Cast:ToMovable(selectObj)
	if nil~=mov then
		PX2_CREATER:CreateICC_Scale(mov)
	else
		NirMan:MessageBox(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Controller_RotateController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local mov = PX2_CREATER:CreateICC_Rotate(selectObj)
	if nil~=mov then
		PX2_CREATER:CreateICC_Scale(mov)
	else
		NirMan:MessageBox(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
end

function n_Create_Controller_TranslateController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local mov = PX2_CREATER:CreateICC_Translate(selectObj)
	if nil~=mov then
		PX2_CREATER:CreateICC_Translate(mov)
	else
		NirMan:MessageBox(PX2_LM:GetValue("Notice"), PX2_LM:GetValue("TipNotHasParent"))
	end
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
function e_AnimResetPlay()
	PX2_EDIT:AnimResetPlay()
end

function e_AnimPlayStop()
	PX2_EDIT:AnimPlayStop()
end

function e_AnimPlay()
	PX2_EDIT:AnimPlay()
end

function e_AnimStop()
	PX2_EDIT:AnimStop()
end

function e_AnimReset()
	PX2_EDIT:AnimReset()
end