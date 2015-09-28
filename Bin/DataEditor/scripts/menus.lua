-- menus.lua

local function e_CreateEditMenu_SceneObjects(editMenu, isForceSetPosBeOrigin)
	local appendStr = "false"
	if isForceSetPosBeOrigin then
		appendStr = "true"
	end

	local createMenu = NirMan:AddSubMenu(editMenu, (PX2_LM:GetValue("Create")..PX2_LM:GetValue("SceneActor")))
	
	local createSceneGeometryActorMenu = NirMan:AddSubMenu(createMenu, (PX2_LM:GetValue("Geometry")))
	NirMan:AddMenuItem(createSceneGeometryActorMenu, PX2_LM:GetValue("RectPiece"), "e_SceneActor_CreateRectangle(false,"..appendStr..")")
	NirMan:AddMenuItem(createSceneGeometryActorMenu, PX2_LM:GetValue("Box"), "e_SceneActor_CreateBox(false,"..appendStr..")")
	NirMan:AddMenuItem(createSceneGeometryActorMenu, PX2_LM:GetValue("Sphere"), "e_SceneActor_CreateSphere(false,"..appendStr..")")
			
	NirMan:AddMenuItem(createMenu, PX2_LM:GetValue("Actor"), "e_SceneActor_CreateActor(false,"..appendStr..")")
	NirMan:AddMenuItem(createMenu, PX2_LM:GetValue("Character"), "e_SceneActor_CreateCharacter(false,"..appendStr..")")
	NirMan:AddMenuItem(createMenu, PX2_LM:GetValue("Light"), "e_SceneActor_CreateLight(false,"..appendStr..")")
	NirMan:AddMenuItem(createMenu, PX2_LM:GetValue("Effect"), "e_SceneActor_CreateEffect(false,"..appendStr..")")
	NirMan:AddMenuItem(createMenu, PX2_LM:GetValue("Trigger"), "e_SceneActor_CreateTrigger(false,"..appendStr..")")
	NirMan:AddMenuItem(createMenu, PX2_LM:GetValue("Terrain"), "e_SceneActor_CreateTerrain(false,"..appendStr..")")
	NirMan:AddMenuItem(createMenu, PX2_LM:GetValue("Sky"), "e_SceneActor_CreateSky(false,"..appendStr..")")
end

local function e_CreateEditMenu_ChildSceneActor(editMenu, isForceSetPosBeOrigin)
	local appendStr = "false"
	if isForceSetPosBeOrigin then
		appendStr = "true"
	end

	local createMenu = NirMan:AddSubMenu(editMenu, (PX2_LM:GetValue("Create")..PX2_LM:GetValue("ChildSceneActor")))
	
	local createSceneGeometryActorMenu = NirMan:AddSubMenu(createMenu, (PX2_LM:GetValue("Geometry")))
	NirMan:AddMenuItem(createSceneGeometryActorMenu, PX2_LM:GetValue("RectPiece"), "e_SceneActor_CreateRectangle(true,"..appendStr..")")
	NirMan:AddMenuItem(createSceneGeometryActorMenu, PX2_LM:GetValue("Box"), "e_SceneActor_CreateBox(true,"..appendStr..")")
	NirMan:AddMenuItem(createSceneGeometryActorMenu, PX2_LM:GetValue("Sphere"), "e_SceneActor_CreateSphere(true,"..appendStr..")")
			
	NirMan:AddMenuItem(createMenu, PX2_LM:GetValue("Actor"), "e_SceneActor_CreateActor(true,"..appendStr..")")
	NirMan:AddMenuItem(createMenu, PX2_LM:GetValue("Character"), "e_SceneActor_CreateCharacter(true,"..appendStr..")")
	NirMan:AddMenuItem(createMenu, PX2_LM:GetValue("Light"), "e_SceneActor_CreateLight(true,"..appendStr..")")
	NirMan:AddMenuItem(createMenu, PX2_LM:GetValue("Effect"), "e_SceneActor_CreateEffect(true,"..appendStr..")")
	NirMan:AddMenuItem(createMenu, PX2_LM:GetValue("Trigger"), "e_SceneActor_CreateTrigger(true,"..appendStr..")")
end

local function e_CreateEditMenu_ChildObjects(editMenu, isForceSetPosBeOrigin)
	local appendStr = "false"
	if isForceSetPosBeOrigin then
		appendStr = "true"
	end

	-- create child menu
	local createMenu = NirMan:AddSubMenu(editMenu, (PX2_LM:GetValue("Create")..PX2_LM:GetValue("ChildObject")))
	
	local createGeometryMenu = NirMan:AddSubMenu(createMenu, (PX2_LM:GetValue("Geometry")))
	NirMan:AddMenuItem(createGeometryMenu, PX2_LM:GetValue("RectPiece"), "e_CreateRectangle("..appendStr..")")
	NirMan:AddMenuItem(createGeometryMenu, PX2_LM:GetValue("Box"), "e_CreateBox("..appendStr..")")
	NirMan:AddMenuItem(createGeometryMenu, PX2_LM:GetValue("Sphere"), "e_CreateSphere("..appendStr..")")

	local createUIMenu = NirMan:AddSubMenu(createMenu, (PX2_LM:GetValue("UI")))
	NirMan:AddMenuItem(createUIMenu, PX2_LM:GetValue("UIPicBox"), "e_CreateUIPicBox("..appendStr..")")
	NirMan:AddMenuItem(createUIMenu, PX2_LM:GetValue("UITest"), "e_CreateUIText("..appendStr..")")
	NirMan:AddSeparater(createUIMenu)
	NirMan:AddMenuItem(createUIMenu, PX2_LM:GetValue("UIFrame"), "e_CreateUIFrame("..appendStr..")")
	NirMan:AddMenuItem(createUIMenu, PX2_LM:GetValue("UIButton"), "e_CreateUIButton("..appendStr..")")
	NirMan:AddMenuItem(createUIMenu, PX2_LM:GetValue("UICheckButton"), "e_CreateUICheckButton("..appendStr..")")
	NirMan:AddMenuItem(createUIMenu, PX2_LM:GetValue("UIEditBox"), "e_CreateUIEditBox("..appendStr..")")
	NirMan:AddMenuItem(createUIMenu, PX2_LM:GetValue("UIProgressBar"), "e_CreateUIProgressBar("..appendStr..")")
				
	local createEffect = NirMan:AddSubMenu(createMenu, (PX2_LM:GetValue("Effect")))
	NirMan:AddMenuItem(createEffect, PX2_LM:GetValue("Particles"), "e_CreateParticles("..appendStr..")")
	NirMan:AddMenuItem(createEffect, PX2_LM:GetValue("Billboard"), "e_CreateBillboard("..appendStr..")")
	NirMan:AddMenuItem(createEffect, PX2_LM:GetValue("Beam"), "e_CreateBeam("..appendStr..")")
	NirMan:AddMenuItem(createEffect, PX2_LM:GetValue("Robbion"), "e_CreateRobbion("..appendStr..")")
	NirMan:AddMenuItem(createEffect, PX2_LM:GetValue("Soundable"), "e_CreateSoundable("..appendStr..")")
				
	NirMan:AddSeparater(createMenu)
	NirMan:AddMenuItem(createMenu, PX2_LM:GetValue("Node"), "e_CreateNode()")
	
	-- create ctrl menu
	local createCtrlMenu = NirMan:AddSubMenu(editMenu, (PX2_LM:GetValue("Create")..PX2_LM:GetValue("Control")))
	NirMan:AddMenuItem(createCtrlMenu, PX2_LM:GetValue("ColorController"), "e_CreateColorController()")
	NirMan:AddMenuItem(createCtrlMenu, PX2_LM:GetValue("AlphaController"), "e_CreateAlphaController()")
	NirMan:AddSeparater(createCtrlMenu)
	NirMan:AddMenuItem(createCtrlMenu, PX2_LM:GetValue("UniformScaleController"), "e_CreateUniformScaleController()")
	NirMan:AddMenuItem(createCtrlMenu, PX2_LM:GetValue("ScaleController"), "e_CreateScaleController()")
	NirMan:AddMenuItem(createCtrlMenu, PX2_LM:GetValue("RotateController"), "e_CreateRotateController()")
	NirMan:AddMenuItem(createCtrlMenu, PX2_LM:GetValue("TranslateController"), "e_CreateTranslateController()")
end

function e_CreateEditMenu(tag)
	local selObj = PX2_SELECTION:GetFirstObject()
	local selControlledable = PX2_CREATER:ConvertToControlledable(selObj)
	local selMovable = PX2_CREATER:ConvertToMovable(selObj)
	local selNode = PX2_CREATER:ConvertToNode(selObj)
	local selActor = PX2_CREATER:ConvertToActor(selObj)
	local selEftCtrlor = PX2_CREATER:ConvertToEffectableController(selObj)
	local selEftModule = PX2_CREATER:ConvertToEffectModule(selObj)
	local selIterpCurveCtrl = PX2_CREATER:ConvertToInterpCurveController(selObj)
	local copyObject = PX2_EDIT:GetCopyObject()

	local editMenu = NirMan:GetCurMenu()
	
	local isAddedSomthing = false
	local isCreateMenuAddSomthing = false
	
	if PVT_PROJECT==tag or RVT_SCENEUI==tag then
		
		if PVT_PROJECT==tag then
			if nil == selObj then
				e_CreateEditMenu_SceneObjects(editMenu, true)
				isAddedSomthing = true
				isCreateMenuAddSomthing = true
				
				NirMan:AddSeparater(editMenu)
				NirMan:AddMenuItem(editMenu, "->"..PX2_LM:GetValue("ImportSelectedRes"), "e_OnImportSelectedRes()")
				NirMan:AddMenuItem(editMenu, "->"..PX2_LM:GetValue("Import"), "e_OnImport()")
			else
				if nil ~= selNode then
					e_CreateEditMenu_SceneObjects(editMenu, true)
					
					if nil ~= selActor then
						e_CreateEditMenu_ChildSceneActor(editMenu, true)
					end
					
					e_CreateEditMenu_ChildObjects(editMenu, true)
								
					NirMan:AddSeparater(editMenu)
					NirMan:AddMenuItem(editMenu, "->"..PX2_LM:GetValue("ImportSelectedRes"), "e_OnImportSelectedRes()")
					NirMan:AddMenuItem(editMenu, "->"..PX2_LM:GetValue("Import"), "e_OnImport()")
					NirMan:AddMenuItem(editMenu, PX2_LM:GetValue("Export").."...", "e_OnExport()")
					
					isAddedSomthing = true
					isCreateMenuAddSomthing = true	
				else
					if nil~=selEftCtrlor then
						local addEffectModuleMenu = NirMan:AddSubMenu(editMenu, (PX2_LM:GetValue("AddEffectModule")))
						isAddedSomthing = true
						NirMan:RefreshEffectableControllerModules(addEffectModuleMenu, selEftCtrl)
					end	
				end
			end
		else 
		
			if nil == selObj then
				e_CreateEditMenu_SceneObjects(editMenu, false)	
				isAddedSomthing = true
				isCreateMenuAddSomthing = true
				
				NirMan:AddSeparater(editMenu)
				NirMan:AddMenuItem(editMenu, "->"..PX2_LM:GetValue("ImportSelectedRes"), "e_OnImportSelectedRes()")
				NirMan:AddMenuItem(editMenu, "->"..PX2_LM:GetValue("Import"), "e_OnImport()")
			else			
				if nil ~= selNode then
					e_CreateEditMenu_SceneObjects(editMenu, false)
					if nil ~= selActor then
						e_CreateEditMenu_ChildSceneActor(editMenu, false)
					end
					e_CreateEditMenu_ChildObjects(editMenu, false)
								
					NirMan:AddSeparater(editMenu)
					NirMan:AddMenuItem(editMenu, "->"..PX2_LM:GetValue("ImportSelectedRes"), "e_OnImportSelectedRes()")
					NirMan:AddMenuItem(editMenu, "->"..PX2_LM:GetValue("Import"), "e_OnImport()")
					NirMan:AddMenuItem(editMenu, PX2_LM:GetValue("Export").."...", "e_OnExport()")
					
					isAddedSomthing = true
					isCreateMenuAddSomthing = true	
				else
					if nil~=selEftCtrlor then
						local addEffectModuleMenu = NirMan:AddSubMenu(editMenu, (PX2_LM:GetValue("AddEffectModule")))
						isAddedSomthing = true
						NirMan:RefreshEffectableControllerModules(addEffectModuleMenu, selEftCtrl)
					end	
				end
			end
		end
		
		if true then
		
		-- InterpCurve
		if nil~=selEftModule or nil~=selIterpCurveCtrl then
			NirMan:AddMenuItem(editMenu, PX2_LM:GetValue("MakeTimeLine"), "e_OnMakeTimeLine()")
			isAddedSomthing = true
		end
		
		-- copy
		if nil~=selObj then
			if isAddedSomthing then
				NirMan:AddSeparater(editMenu)
			end
			NirMan:AddMenuItem(editMenu, PX2_LM:GetValue("Copy"), "e_OnCopy()")
			isAddedSomthing = true
		end
		
		if nil~=copyObject then
			if isAddedSomthing then
				NirMan:AddSeparater(editMenu)
			end
			NirMan:AddMenuItem(editMenu, PX2_LM:GetValue("Paste"), "e_OnPaste()")
			isAddedSomthing = true
		end
		
		-- delete
		if nil~=selObj then
			if isAddedSomthing then
				NirMan:AddSeparater(editMenu)
			end
			NirMan:AddMenuItem(editMenu, PX2_LM:GetValue("Delete"), "e_OnDelete()")
			isAddedSomthing = true
		end
		
		-- controll anim play
		if nil~=selControlledable then
			if isAddedSomthing then
				NirMan:AddSeparater(editMenu)
			end
			NirMan:AddMenuItem(editMenu, PX2_LM:GetValue("ResetPlay"), "e_AnimResetPlay()")
			NirMan:AddMenuItem(editMenu, PX2_LM:GetValue("Play"), "e_AnimPlay()")
			NirMan:AddMenuItem(editMenu, PX2_LM:GetValue("Stop"), "e_AnimStop()")
			NirMan:AddMenuItem(editMenu, PX2_LM:GetValue("Reset"), "e_AnimReset()")
			isAddedSomthing = true
		end
		
		-- show in level
		if PVT_PROJECT==tag then
			if isAddedSomthing then
				NirMan:AddSeparater(editMenu)
			end
			local showLevelMenu = NirMan:AddSubMenu(editMenu, (PX2_LM:GetValue("ShowLevel")))
			NirMan:AddMenuItem(showLevelMenu, PX2_LM:GetValue("ShowGeneral"), "e_ProjectView_ShowLevel(0)")
			NirMan:AddMenuItem(showLevelMenu, PX2_LM:GetValue("ShowChildren"), "e_ProjectView_ShowLevel(1)")
			NirMan:AddMenuItem(showLevelMenu, PX2_LM:GetValue("ShowControl"), "e_ProjectView_ShowLevel(2)")
			NirMan:AddMenuItem(showLevelMenu, PX2_LM:GetValue("ShowMaterial"), "e_ProjectView_ShowLevel(3)")
			NirMan:AddMenuItem(showLevelMenu, PX2_LM:GetValue("ShowDetail"), "e_ProjectView_ShowLevel(4)")
		end
		
		-- find
		if nil~=selObj then
			NirMan:AddSeparater(editMenu)
			if RVT_SCENEUI==tag then
				NirMan:AddMenuItem(editMenu, PX2_LM:GetValue("FindInSceneTree"), "e_FindInSceneTree()")
			end	
			NirMan:AddMenuItem(editMenu, PX2_LM:GetValue("FindInResTree"), "e_FindInResTree()")
		end
		
		end
	end
end

function e_CreateResMenu()
	local editMenu = NirMan:GetCurMenu()
	NirMan:AddMenuItem(editMenu, PX2_LM:GetValue("Open"), "e_ResView_Open()")
	NirMan:AddMenuItem(editMenu, PX2_LM:GetValue("CopyResPath"), "e_ResView_CopyResPath()")
end

function e_CreateTimeLineMenu_Left()
	local editMenu = NirMan:GetCurMenu()
	NirMan:AddMenuItem(editMenu, PX2_LM:GetValue("Delete"), "e_DeleteCurveGroup()")
	NirMan:AddMenuItem(editMenu, PX2_LM:GetValue("DeleteAll"), "e_DeleteAllCurveGroup()")
end

function e_CreateTimeLineMenu_Right()
	local editMenu = NirMan:GetCurMenu()
	NirMan:AddMenuItem(editMenu, PX2_LM:GetValue("SetInValue"), "e_SetInValue()")
	NirMan:AddMenuItem(editMenu, PX2_LM:GetValue("SetOutValue"), "e_SetOutValue()")
	NirMan:AddSeparater(editMenu)
	NirMan:AddMenuItem(editMenu, PX2_LM:GetValue("Delete"), "e_DeleteCurveKeyPoint()")
end